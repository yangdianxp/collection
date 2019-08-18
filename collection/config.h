#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <string>
#include <vector>

#include "serial_port_config.h"
using namespace std;

class config
{
public:
	bool load_file(string file_path, vector<serial_port_config>& serial_port_configs);
};

#endif

