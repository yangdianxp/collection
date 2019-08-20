#ifndef __SLAVE_CONFIG_H__
#define __SLAVE_CONFIG_H__
#include <vector>

using namespace std;

class slave_config
{
public:
	enum register_type {
		register_uint8,
		register_uint16,
		register_uint32,
		register_float,
		register_double,
		register_string,
		register_last,
	};

	struct register_config
	{
		register_type type;
		uint8_t bytes;
		uint32_t addr;
	};

	slave_config(uint32_t addr);
	void push_register_config(register_config r);

	void print();

private:
	uint32_t addr_;
	vector<register_config> register_config_;
};

#endif
