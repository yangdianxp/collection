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
	//���ջ��������ܳ���
	static const uint16_t RECEIVE_BUUFER_LEN = 256;
	//��Ϣͷ�ĳ���
	static const uint16_t RECEIVE_HEADER_LEN = 3;
	// Ӧ������Ϣ�峤������Ϣ�е��±�λ��
	static const uint16_t RECEIVE_BODY_LEN_INDEX = 2;
	// У���볤��
	static const uint16_t CRC_LEN = 2;
public:
	modbus_master(asio::io_context& service, std::string port);

	void init_serial_port(std::tuple<uint32_t, uint8_t, uint8_t, uint8_t> serial_port_info);
	void init_slave(vector<slave_config>& slave_configs);
	void run();

	void print();
	
private:
	// ����豸���Ͷ��Ĵ��������е�ֵ
	void send_read_holding_registers(uint8_t addr, uint16_t start_reg, uint16_t count);
	// �����ص�ֵ
	void receive_read_holding_registers();
	// ��ͷ��
	void receive_header();
	// ����Ϣ��
	void receive_body();
	// ������Ϣ�����
	void receive_body_complete(uint32_t recv_len);
	
	// ��װ���Ĵ�����modbus adu
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