#include <iostream>
#include "log.h"
#include "slave_config.h"

slave_config::slave_config(uint32_t addr)
	: addr_(addr)
{
}

void slave_config::push_register_config(register_config r)
{
	register_configs_.push_back(r);
}

uint32_t slave_config::get_addr()
{
	return addr_;
}

const vector<slave_config::register_config>& slave_config::get_register_configs()
{
	return register_configs_;
}

void slave_config::print()
{
	SLOG_NOTICE << "salve addr = " << addr_;
	SLOG_NOTICE << "-----------------------";
	for (auto r : register_configs_)
	{
		SLOG_NOTICE << "type = " << r.type << " "
			<< "bytes = " << int(r.bytes) << " "
			<< "addr = " << r.addr;
	}
	SLOG_NOTICE << "-----------------------";
}
