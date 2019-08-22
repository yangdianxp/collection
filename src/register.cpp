#include <iostream>
#include "log.h"
#include "register.h"

base_register::base_register(uint8_t bytes, uint32_t addr)
	: bytes_(bytes), addr_(addr)
{
}

void base_register::set_value(string v)
{
	value_ = v;
}

uint16_t base_register::get_addr()
{
	return addr_;
}

uint8_t base_register::get_bytes()
{
	return bytes_;
}

string base_register::get_value()
{
	return value_;
}

uint8_register::uint8_register(uint8_t bytes, uint32_t addr) :
	base_register(bytes, addr)
{
}

void uint8_register::print()
{
	SLOG_DEBUG << "uint8_register" << " "
		<< "bytes_ = " << get_bytes() << " "
		<< "addr_ = " << get_addr() << " "
		<< "value_ = " << get_value();
}

uint16_register::uint16_register(uint8_t bytes, uint32_t addr) :
	base_register(bytes, addr)
{
}

void uint16_register::print()
{
	SLOG_DEBUG << "uint16_register" << " "
		<< "bytes_ = " << get_bytes() << " "
		<< "addr_ = " << get_addr() << " "
		<< "value_ = " << get_value();
}

uint32_register::uint32_register(uint8_t bytes, uint32_t addr) :
	base_register(bytes, addr)
{
}

void uint32_register::print()
{
	SLOG_DEBUG << "uint32_register" << " "
		<< "bytes_ = " << get_bytes() << " "
		<< "addr_ = " << get_addr() << " "
		<< "value_ = " << get_value();
}

std::shared_ptr<base_register> make_register(slave_config::register_type type,
	uint8_t bytes, uint16_t addr)
{
	switch (type)
	{
	case slave_config::register_uint8:
		return std::make_shared<uint8_register>(bytes, addr);
	case slave_config::register_uint16:
		return std::make_shared<uint16_register>(bytes, addr);
	case slave_config::register_uint32:
		return std::make_shared<uint32_register>(bytes, addr);
	case slave_config::register_float:
		break;
	case slave_config::register_double:
		break;
	case slave_config::register_string:
		break;
	case slave_config::register_last:
		break;
	}
	return std::shared_ptr<uint8_register>(nullptr);
}
