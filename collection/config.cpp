#include <iostream>
#include "config.h"

using namespace std;

void config::load_file(string file_path, vector<serial_port_config>& serial_port_configs)
{
	try {
		boost::property_tree::ptree root;
		boost::property_tree::read_json<boost::property_tree::ptree>(file_path, root);
		auto all_serial_port = root.get_child("serial_port");
		load_serial_port_config(all_serial_port, serial_port_configs);
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		exit(1);
	}
}

void config::load_serial_port_config(boost::property_tree::ptree& all_serial_port, 
	vector<serial_port_config>& serial_port_configs)
{
	for (auto serial_it = all_serial_port.begin(); serial_it != all_serial_port.end(); ++serial_it)
	{
		auto port_name = serial_it->second.get<string>("port_name");
		auto baud_rate = serial_it->second.get<uint32_t>("baud_rate");
		auto parity = serial_it->second.get<uint8_t>("parity");
		auto data_bit = serial_it->second.get<uint8_t>("data_bit");
		auto stop_bit = serial_it->second.get<uint8_t>("stop_bit");
		serial_port_config serial_port_config(port_name, baud_rate, parity, data_bit, stop_bit);
		auto all_register_config = serial_it->second.get_child("register");
		load_register_config(all_register_config, serial_port_config);
		serial_port_configs.push_back(serial_port_config);
	}
	for (auto s : serial_port_configs)
	{
		s.print();
	}
}

void config::load_register_config(boost::property_tree::ptree& all_register_config, serial_port_config& serial_port_config)
{
	for (auto register_it = all_register_config.begin(); register_it != all_register_config.end(); ++register_it)
	{
		auto type_i = register_it->second.get<uint8_t>("type");
		auto type = serial_port_config::register_type(type_i);
		auto bytes = register_it->second.get<uint8_t>("bytes");
		auto addr = register_it->second.get<uint32_t>("addr");
		serial_port_config::register_config register_config;
		register_config.type = type;
		register_config.bytes = bytes;
		register_config.addr = addr;
		auto repeat = register_it->second.get<uint8_t>("repeat");
		for (auto i = 0; i < repeat; ++i)
		{
			register_config.addr += i * register_config.bytes;
			serial_port_config.push_register_config(register_config);
		}
	}
}
