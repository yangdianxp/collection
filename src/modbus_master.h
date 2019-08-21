#ifndef __MODBUS_MASTER_H__
#define __MODBUS_MASTER_H__
#include <vector>

#include <boost/asio.hpp>

#include "slave_config.h"
#include "slave.h"

using namespace boost;
using namespace std;

class modbus_master : 
	public std::enable_shared_from_this<modbus_master>
{
	//接收缓冲区的总长度
	static const uint16_t RECEIVE_BUUFER_LEN = 256;
	//消息头的长度
	static const uint16_t RECEIVE_HEADER_LEN = 3;
	// 应接收消息体长度在消息中的下标位置
	static const uint16_t RECEIVE_BODY_LEN_INDEX = 2;
	// 校验码长度
	static const uint16_t CRC_LEN = 2;
public:
	modbus_master(asio::io_context& service, std::string port);

	void init_serial_port(std::tuple<uint32_t, uint8_t, uint8_t, uint8_t> serial_port_info);
	void init_slave(vector<slave_config>& slave_configs);
	void run();

	void print();
	
private:
	// 向从设备发送读寄存器命令中的值
	void send_read_holding_registers(uint8_t addr, uint16_t start_reg, uint16_t count);
	// 读返回的值
	void receive_read_holding_registers();
	// 读头部
	void receive_header();
	// 读消息体
	void receive_body();
	// 接收消息体完成
	void receive_body_complete(uint32_t recv_len);
	
	// 组装读寄存器的modbus adu
	std::vector<uint8_t> assembly_read_holding_registers_modbus_adu(uint8_t addr, uint8_t func_code, uint16_t start_reg, uint16_t count);

	uint8_t low_byte(uint16_t half_word);
	uint8_t high_byte(uint16_t half_word);
	uint16_t calc_crc(const std::vector<uint8_t>& modbus_adu);
	uint16_t crc16_update(uint16_t crc, uint8_t a);
	bool check_message(const std::vector<uint8_t>& msg);
	

	static const uint8_t READ_HOLDING_REGISTER = 0x03; ///< Modbus function 0x03 Read Holding Registers

	asio::serial_port sp_;

	std::vector<uint8_t> modbus_adu_;
	std::vector<uint8_t> modbus_receive_;

	std::vector<slave> slaves_;
};

#endif