#include <iostream>
#include <tuple>
#include "log.h"
#include "modbus_master.h"

using namespace std;

modbus_master::modbus_master(asio::io_context& service, std::string port_name)
	: sp_(service, port_name), port_name_(port_name), modbus_receive_(RECEIVE_BUUFER_LEN)
{
}

void modbus_master::init_serial_port(std::tuple<uint32_t, uint8_t, uint8_t, uint8_t> serial_port_info)
{
	auto baud_rate = std::get<0>(serial_port_info);
	sp_.set_option(asio::serial_port::baud_rate(baud_rate));
	sp_.set_option(asio::serial_port::flow_control());
	auto parity = std::get<1>(serial_port_info);
	sp_.set_option(asio::serial_port::parity(asio::serial_port::parity::type(parity)));
	auto data_bit = std::get<2>(serial_port_info);
	sp_.set_option(asio::serial_port::character_size(data_bit));
	auto stop_bit = std::get<3>(serial_port_info);
	sp_.set_option(asio::serial_port::stop_bits(asio::serial_port::stop_bits::type(stop_bit)));
	SLOG_DEBUG << "baud_rate = " << baud_rate << " "
		<< "parity = " << parity << " "
		<< "data_bit = " << data_bit << " "
		<< "stop_bit = " << stop_bit;
}

void modbus_master::init_slave(vector<slave_config>& slave_configs)
{
	for (auto it = slave_configs.begin(); it != slave_configs.end(); ++it)
	{
		slave s;
		s.init(*it);
		slaves_.push_back(s);
	}
	if (slaves_.empty())
	{
		SLOG_ERROR << "no slave";
		exit(-1);
	}
}

void modbus_master::run()
{
	slave& s = slaves_[slave_index_];
	send_read_holding_registers(1, 20000, 5);
}

void modbus_master::print()
{
	SLOG_DEBUG << "port_name_ = " << port_name_;
	for (auto it = slaves_.begin(); it != slaves_.end(); ++it)
	{
		it->print();
	}
}

void modbus_master::send_read_holding_registers(uint8_t addr, uint16_t start_reg, uint16_t count)
{
	modbus_adu_ =
		assembly_read_holding_registers_modbus_adu(addr, READ_HOLDING_REGISTER, start_reg, count);

	for (auto n : modbus_adu_)
	{
		SLOG_DEBUG << std::to_string(n);
	}
	asio::async_write(sp_, asio::buffer(modbus_adu_),
		[this] (boost::system::error_code ec, std::size_t /*length*/)
	{
		if (ec)
		{
			SLOG_ERROR << "error:" << ec.message();
		}
		SLOG_DEBUG << "send complete!";
		receive_read_holding_registers();
	});
}

void modbus_master::receive_read_holding_registers()
{
	receive_header();
}

void modbus_master::receive_header()
{
	asio::async_read(sp_, asio::buffer(modbus_receive_.data(), RECEIVE_HEADER_LEN),
		[this](boost::system::error_code ec, std::size_t /*length*/)
	{
		if (ec)
		{
			SLOG_ERROR << "error:" << ec.message();
		}
		SLOG_DEBUG << "receive header success";
		receive_body();
	});
}

void modbus_master::receive_body()
{
	uint8_t body_len = modbus_receive_[RECEIVE_BODY_LEN_INDEX];
	uint8_t recv_len = body_len + CRC_LEN;
	asio::async_read(sp_, asio::buffer(&modbus_receive_[RECEIVE_HEADER_LEN], recv_len),
		[this](boost::system::error_code ec, std::size_t trans_len)
	{
		if (ec)
		{
			SLOG_ERROR << "error:" << ec.message();
		}
		SLOG_DEBUG << "receive body success";
		receive_body_complete(trans_len + RECEIVE_HEADER_LEN);
	});
}

void modbus_master::receive_body_complete(uint32_t recv_len)
{
	SLOG_DEBUG << "recv_len = " << recv_len;
	SLOG_DEBUG << "receive body complete";
	for (uint32_t i = 0; i < recv_len; ++i)
	{
		SLOG_DEBUG << std::to_string(modbus_receive_[i]);
	}
	std::vector<uint8_t> msg(&modbus_receive_[0], &modbus_receive_[recv_len]);
	if (!check_message(msg))
	{
		SLOG_ERROR << "message check error";
	}
	SLOG_DEBUG << "receive message success";
}

std::vector<uint8_t> modbus_master::assembly_read_holding_registers_modbus_adu(uint8_t addr, uint8_t func_code, uint16_t start_reg, uint16_t count)
{
	std::vector<uint8_t> modbus_adu;
	modbus_adu.push_back(addr);
	modbus_adu.push_back(func_code);
	modbus_adu.push_back(high_byte(start_reg));
	modbus_adu.push_back(low_byte(start_reg));
	modbus_adu.push_back(high_byte(count));
	modbus_adu.push_back(low_byte(count));
	uint16_t crc = calc_crc(modbus_adu);
	modbus_adu.push_back(low_byte(crc));
	modbus_adu.push_back(high_byte(crc));
	return modbus_adu;
}

uint8_t modbus_master::low_byte(uint16_t half_word)
{
	return (uint8_t)((half_word) & 0xFF);
}

uint8_t modbus_master::high_byte(uint16_t half_word)
{
	return (uint8_t)((half_word) >> 8);
}

uint16_t modbus_master::calc_crc(const std::vector<uint8_t>& modbus_adu)
{
	uint16_t crc = 0xFFFF;
	for (uint32_t i = 0; i < modbus_adu.size(); i++)
	{
		crc = crc16_update(crc, modbus_adu[i]);
	}
	return crc;
}

uint16_t modbus_master::crc16_update(uint16_t crc, uint8_t a)
{
	int i;
	crc ^= a;
	for (i = 0; i < 8; ++i)
	{
		if (crc & 1)
			crc = (crc >> 1) ^ 0xA001;
		else
			crc = (crc >> 1);
	}
	return crc;
}

bool modbus_master::check_message(const std::vector<uint8_t>& msg)
{
	std::vector<uint8_t> recv_msg(msg.begin(), msg.end() - 2);
	uint16_t crc_calc = calc_crc(recv_msg);
	uint8_t crc_low = *(msg.end() - CRC_LEN);
	uint8_t crc_high = *(msg.end() - 1);
	return (low_byte(crc_calc) == crc_low &&
		high_byte(crc_calc) == crc_high);
}



