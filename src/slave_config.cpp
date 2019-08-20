#include <iostream>
#include "slave_config.h"

slave_config::slave_config(uint32_t addr)
	: addr_(addr)
{
}

void slave_config::push_register_config(register_config r)
{
	register_config_.push_back(r);
}

void slave_config::print()
{
	cout << "salve addr = " << addr_ << endl;
	cout << "-----------------------" << endl;
	for (auto r : register_config_)
	{
		cout << "type = " << r.type << " "
			<< "bytes = " << int(r.bytes) << " "
			<< "addr = " << r.addr << endl;
	}
	cout << "-----------------------" << endl;
}
