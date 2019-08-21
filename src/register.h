#ifndef __REGISTER_H__
#define __REGISTER_H__
#include <string>

#include "slave_config.h"
using namespace std;

class base_register
{
public:
	virtual void print() = 0;
	virtual void set_value(string v) = 0;
};

// 1字节整型寄存器类
class uint8_register : public base_register
{
public:
	uint8_register(uint8_t bytes, uint32_t addr);
	void print();
	void set_value(string v);
private:
	uint8_t bytes_;
	uint32_t addr_;
	string value_;
};

// 2字节整型寄存器类
class uint16_register : public base_register
{
public:
	uint16_register(uint8_t bytes, uint32_t addr);

	void print();
	void set_value(string v);
private:
	uint8_t bytes_;
	uint32_t addr_;
	string value_;
};

// 4字节整型寄存器类
class uint32_register : public base_register
{
public:
	uint32_register(uint8_t bytes, uint32_t addr);

	void print();
	void set_value(string v);
private:
	uint8_t bytes_;
	uint32_t addr_;
	string value_;
};

std::shared_ptr<base_register> make_register(slave_config::register_type type,
	uint8_t bytes, uint32_t addr);


#endif
