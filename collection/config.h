#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "serial_port_config.h"
using namespace std;

class config
{
public:
	void load_file(string file_path, vector<serial_port_config>& serial_port_configs);
	void load_serial_port_config(boost::property_tree::ptree& all_serial_port, 
		vector<serial_port_config>& serial_port_configs);
	void load_register_config(boost::property_tree::ptree& all_register_config, serial_port_config& serial_port_config);
};

#endif

