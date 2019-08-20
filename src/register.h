#ifndef __REGISTER_H__
#define __REGISTER_H__
#include <string>
using namespace std;

class base_register
{
public:
	virtual void print() = 0;
	virtual void set_value(string v) = 0;
};

// 1�ֽ����ͼĴ�����
class uint8_register : public base_register
{
public:
	void print();
	void set_value(string v);
private:
	uint8_t value;
};

// 2�ֽ����ͼĴ�����
class uint16_register : public base_register
{
public:
	void print();
	void set_value(string v);
private:
	uint16_t value;
};


#endif
