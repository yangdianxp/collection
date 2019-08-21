#include <iostream>
#include "log.h"
#include "serial_port_config.h"

serial_port_config::serial_port_config(string serial_port_name, uint32_t baud_rate, uint8_t parity,
	uint8_t data_bit, uint8_t stop_bit) :
	serial_port_name_(serial_port_name), baud_rate_(baud_rate), parity_(parity),
	data_bit_(data_bit), stop_bit_(stop_bit)
{
}

void serial_port_config::push_slave_config(slave_config s)
{
	slave_configs_.push_back(s);
}

void serial_port_config::print()
{
	SLOG_NOTICE << "===============serial port config==================";
	SLOG_NOTICE << "serial_port_name_ = " << serial_port_name_;
	SLOG_NOTICE << "baud_rate_ = " << baud_rate_;
	SLOG_NOTICE << "parity_ = " << int(parity_);
	SLOG_NOTICE << "data_bit_ = " << int(data_bit_);
	SLOG_NOTICE << "stop_bit_ = " << int(stop_bit_);
	SLOG_NOTICE << "***************";
	for (auto s : slave_configs_)
	{
		s.print();
	}
	SLOG_NOTICE << "***************";
	SLOG_NOTICE << "==================================================";
}

string serial_port_config::get_serial_port_name()
{
	return serial_port_name_;
}

const vector<slave_config>& serial_port_config::get_slave_configs()
{
	return slave_configs_;
}

tuple<uint32_t, uint8_t, uint8_t, uint8_t> serial_port_config::get_serial_port_info()
{
	return make_tuple(baud_rate_, parity_, data_bit_, stop_bit_);
}
