#ifndef __SERIAL_PORT_CONFIG_H__
#define __SERIAL_PORT_CONFIG_H__
#include <string>
#include <vector>
#include <tuple>

#include "serial_port_config.h"
#include "slave_config.h"
using namespace std;

class serial_port_config
{
public:
	serial_port_config(string serial_port_name, uint32_t baud_rate = 9600, uint8_t parity = 1,
		uint8_t data_bit = 8, uint8_t stop_bit = 1);
	void push_slave_config(slave_config s);
	void print();
	string get_serial_port_name();
	const vector<slave_config>& get_slave_configs();
	tuple<uint32_t, uint8_t, uint8_t, uint8_t> get_serial_port_info();

private:
	string serial_port_name_;
	uint32_t baud_rate_;
	uint8_t parity_;
	uint8_t data_bit_;
	uint8_t stop_bit_;
	vector<slave_config> slave_configs_;
};

#endif

