#ifndef __ASIO_GENERIC_SERVER_H__
#define __ASIO_GENERIC_SERVER_H__
#include <vector>
#include <boost/asio.hpp>
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
};

#endif
