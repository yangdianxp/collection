#ifndef __ASIO_GENERIC_SERVER_H__
#define __ASIO_GENERIC_SERVER_H__
#include <vector>
#include <boost/asio.hpp>

#include "config.h"
#include "modbus_master.h"

using namespace boost;
using namespace std;
using namespace asio::ip;

template <typename ConnectionHandler>
class asio_generic_server
{
	using shared_handler_t = std::shared_ptr< ConnectionHandler>;
public:
	asio_generic_server(int thread_count = 1)
		: thread_count_(thread_count)
		, acceptor_(io_service_)
	{}

	void start_server(uint16_t port)
	{
		auto handler
			= std::make_shared< ConnectionHandler>(io_service_);
		asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), port);
		acceptor_.open(endpoint.protocol());
		acceptor_.set_option(tcp::acceptor::reuse_address(true));
		acceptor_.bind(endpoint);
		acceptor_.listen();

		acceptor_.async_accept(handler->socket()
			, [=](auto ec)
		{
			handle_new_connection(handler, ec);
		}
		);
		for (int i = 1; i < thread_count_; ++i)
		{
			thread_pool_.emplace_back(
				[=]
			{
				io_service_.run();
			}
			);
		}
		io_service_.run();
	}

	asio::io_context& get_service()
	{
		return io_service_;
	}

	void init_modbus()
	{
		SLOG_DEBUG << "start init modbus";
		config config;
		config.load_file("config.json", serial_port_configs_);
		for (auto it = serial_port_configs_.begin(); it != serial_port_configs_.end(); ++it)
		{
			string port_name = it->get_serial_port_name();
			modbus_master modbus_master(io_service_, port_name);
			auto serial_port_info = it->get_serial_port_info();
			modbus_master.init_serial_port(serial_port_info);
			auto slave_configs = it->get_slave_configs();
			modbus_master.init_slave(slave_configs);
			modbus_masters_.push_back(std::move(modbus_master));
		}
	}

private:
	void handle_new_connection(shared_handler_t handler
		, system::error_code const & error)
	{
		if (error) { return; }
		handler->start();

		auto new_handler
			= std::make_shared< ConnectionHandler>(io_service_);

		acceptor_.async_accept(new_handler->socket()
			, [=](auto ec)
		{
			handle_new_connection(new_handler, ec);
		}
		);
	}

	int thread_count_;
	std::vector<std::thread> thread_pool_;
	asio::io_context io_service_;
	asio::ip::tcp::acceptor acceptor_;

	vector<serial_port_config> serial_port_configs_;
	vector<modbus_master> modbus_masters_;
};

#endif
