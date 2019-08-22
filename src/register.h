#ifndef __REGISTER_H__
#define __REGISTER_H__
#include <string>

#include "slave_config.h"
using namespace std;

class base_register
{
public:
	base_register(uint8_t bytes, uint32_t addr);
	virtual void print() = 0;
	virtual void set_value(string v);
	virtual uint16_t get_addr();
	virtual uint8_t get_bytes();
	virtual string get_value();
private:
	uint8_t bytes_;
	uint16_t addr_;
	string value_;
};

// 1�ֽ����ͼĴ�����
class uint8_register : public base_register
{
public:
	uint8_register(uint8_t bytes, uint32_t addr);
	void print();
};

// 2�ֽ����ͼĴ�����
class uint16_register : public base_register
{
public:
	uint16_register(uint8_t bytes, uint32_t addr);
	void print();
};

// 4�ֽ����ͼĴ�����
class uint32_register : public base_register
{
public:
	uint32_register(uint8_t bytes, uint32_t addr);
	void print();
};

std::shared_ptr<base_register> make_register(slave_config::register_type type,
	uint8_t bytes, uint16_t addr);


#endif
