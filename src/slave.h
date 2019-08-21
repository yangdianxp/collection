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
	void init(slave_config& sc);
	void print();

private:
	uint32_t addr_;
	vector<std::shared_ptr<base_register>> registers_;
};

#endif

