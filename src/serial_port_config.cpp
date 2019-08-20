#include <iostream>
#include "serial_port_config.h"


serial_port_config::serial_port_config(string serial_port_name, uint32_t baud_rate, uint8_t parity,
	uint8_t data_bit, uint8_t stop_bit) :
	serial_port_name_(serial_port_name), baud_rate_(baud_rate), parity_(parity),
	data_bit_(data_bit), stop_bit_(stop_bit)
{
}

void serial_port_config::push_slave_config(slave_config s)
{
	slave_config_.push_back(s);
}

void serial_port_config::print()
{
	cout << "===============serial port config==================" << endl;
	cout << "serial_port_name_ = " << serial_port_name_ << endl;
	cout << "baud_rate_ = " << baud_rate_ << endl;
	cout << "parity_ = " << int(parity_) << endl;
	cout << "data_bit_ = " << int(data_bit_) << endl;
	cout << "stop_bit_ = " << int(stop_bit_) << endl;
	cout << "***************" << endl;
	for (auto s : slave_config_)
	{
		s.print();
	}
	cout << "***************" << endl;
	cout << "==================================================" << endl;
}
