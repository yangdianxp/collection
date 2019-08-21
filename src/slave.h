#ifndef __SLAVE_H__
#define __SLAVE_H__
#include <vector>
#include <memory>

#include "slave_config.h"
#include "register.h"

using namespace std;

class slave
{
public:
	struct read_packet_index
	{
		uint16_t start_reg;
		uint16_t count;
	};

	void init(slave_config& sc);
	vector<slave::read_packet_index> create_read_packet_index();

	void print();


private:
	uint32_t addr_;
	vector<std::shared_ptr<base_register>> registers_;
};

#endif

