#include <iostream>
#include <vector>
#include "register.h"
#include "config.h"
#include "serial_port_config.h"

int main()
{
	vector<serial_port_config> serial_port_configs;
	config c;
	c.load_file("config.json", serial_port_configs);

	system("pause");
	return 0;
}

#if 0
uint8_register uint8_reg;
uint8_reg.set_value("8");
uint8_reg.print();

uint16_register uint16_reg;
uint16_reg.set_value("50000");
uint16_reg.print();

#endif