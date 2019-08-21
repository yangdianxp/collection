#include <iostream>
#include "log.h"
#include "asio_generic_server.h"
#include "chat_handler.h"

int main()
{

	try {
		SLog::InitLog("collection");
		SLog::SetLevel(slog_debug);
		SLOG_INFO << "collection start";
		asio_generic_server<chat_handler> server;
		server.init_modbus();
		server.start_server(8888);
	}
	catch (std::exception& err)
	{
		SLOG_ERROR << "Exception Error: " << err.what();
	}

	return 0;
}

#if 0
void fun()
{
	for (int i = 0; i < 1000; ++i)
	{
		SLOG_INFO << "AAAAAAAAAAAAAA";
	}
}

void fun1()
{
	for (int i = 0; i < 1000; ++i)
	{
		SLOG_INFO << "BBBBBBBBBBBBBB";
	}
}

SLog::InitLog("collection", false);
std::thread t(fun);
std::thread t1(fun1);
t.join();
t1.join();

#endif

#if 0

vector<serial_port_config> serial_port_configs;
config c;
c.load_file("config.json", serial_port_configs);

#include <iostream>

#include "asio_generic_server.h"
#include "chat_handler.h"
#include "modbus_master.h"

using namespace std;

int main()
{
	try {
		asio_generic_server<chat_handler> server;
		asio::io_context& service = server.get_service();
		modbus_master mm(service, "COM6");
		mm.init_serial_port();
		mm.run();
		server.start_server(8888);
	}
	catch (std::exception& err)
	{
		cout << "Exception Error: " << err.what() << endl;
	}

	::system("pause");
	return 0;
}

#endif

#if 0
uint8_register uint8_reg;
uint8_reg.set_value("8");
uint8_reg.print();

uint16_register uint16_reg;
uint16_reg.set_value("50000");
uint16_reg.print();

#endif