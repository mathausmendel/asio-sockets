#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/range/irange.hpp>
#include <memory>

int main(int argc, char** argv)
{
	// Setting up the io_service
	boost::asio::io_service io_service;
	boost::asio::io_service::work work(io_service);
	boost::thread_group threads;

	const auto concurrency = boost::thread::physical_concurrency();
	for(const auto i : boost::irange<decltype(concurrency)>(0, concurrency))
	{
		threads.create_thread([&]{ io_service.run(); });
	}

	// Setting up the server to listen on port 1515
	boost::asio::ip::tcp::acceptor acceptor(
		io_service,
		boost::asio::ip::tcp::endpoint(
			boost::asio::ip::tcp::v6(), 1515));

	const std::function<void()> accept = [&]() -> void
	{
		auto socket = std::make_shared<boost::asio::ip::tcp::socket>(io_service);
		acceptor.async_accept(
			*socket,
			[&, socket](auto error)
			{
				io_service.post(accept);

				if(!error)
				{
					char buffer[1024] = { 0 };
					socket->read_some(boost::asio::buffer(buffer));
					std::cout << "Socket connected. Data: " << buffer << std::endl;
				}
			});
	};

	// Start the server
	io_service.post(accept);

	threads.join_all();

	return 0;
}
