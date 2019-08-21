#include "log.h"
#include "slave.h"

void slave::init(slave_config& slave_config)
{
	addr_ = slave_config.get_addr();
	auto register_configs = slave_config.get_register_configs();
	for (auto it = register_configs.begin(); it != register_configs.end(); ++it)
	{
		auto reg = make_register(it->type, it->bytes, it->addr);
		registers_.push_back(reg);
	}
}

vector<slave::read_packet_index> slave::create_read_packet_index()
{
	if (registers_.empty())
	{
		return vector<slave::read_packet_index>();
	}
	//uint16_t start_reg = registers_.begin()->get_addr();
	//for (auto it = registers_.begin(); it != registers_.end(); ++it)
	//{

	//}
}

void slave::print()
{
	SLOG_DEBUG << "addr = " << addr_;
	for (auto it = registers_.begin(); it != registers_.end(); ++it)
	{
		(*it)->print();
	}
}

