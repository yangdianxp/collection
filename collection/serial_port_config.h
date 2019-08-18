#ifndef __SERIAL_PORT_CONFIG_H__
#define __SERIAL_PORT_CONFIG_H__
#include <string>
#include <vector>

#include "serial_port_config.h"
using namespace std;

class serial_port_config
{
public:
	enum register_type {
		register_uint8,
		register_uint16,
		register_uint32,
		register_float,
		register_double,
		register_string,
		register_last,
	};

	struct register_config
	{
		register_type type;
		uint8_t bytes;
		uint32_t addr;
	};

	serial_port_config(string serial_port_name, uint32_t baud_rate = 9600, uint8_t parity = 1,
		uint8_t data_bit = 8, uint8_t stop_bit = 1);
	void push_register_config(register_config c);
	void print();

private:
	string serial_port_name_;
	uint32_t baud_rate_;
	uint8_t parity_;
	uint8_t data_bit_;
	uint8_t stop_bit_;
	vector<register_config> register_config_;
};

#endif

