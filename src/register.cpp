#include <iostream>
#include "register.h"

void uint8_register::print()
{
	cout << "value = " << int(value) << endl;
}

void uint8_register::set_value(string v)
{
	value = stoi(v);
}

void uint16_register::print()
{
	cout << "value = " << int(value) << endl;
}

void uint16_register::set_value(string v)
{
	value = stoi(v);
}