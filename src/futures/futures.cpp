#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/thread.hpp>
#include <boost/range/irange.hpp>
#include <memory>

class Socket
{
	struct ScopedTimer
	{
		ScopedTimer(boost::asio::deadline_timer& timer,
			boost::asio::ip::tcp::socket& socket,
			int msec = 5000)
			: m_timer(timer)
			, m_socket(socket)
		{
			m_timer.expires_from_now(boost::posix_time::millisec(msec));
			m_timer.async_wait([&](auto ec)
			{
				if(ec != boost::asio::error::operation_aborted)
				{
					m_socket.cancel(ec);
				}
			});
		}

		~ScopedTimer()
		{
			boost::system::error_code ec;
			m_timer.cancel(ec);
			(void)ec;
		}

		boost::asio::deadline_timer& m_timer;
		boost::asio::ip::tcp::socket& m_socket;
	};

	template<typename R>
	R get_from_future(std::future<R>& future, boost::system::error_code& ec)
	{
		ec.clear();
		try
		{
			return future.get();
		}
		catch(const boost::system::system_error& se)
		{
			ec = se.code();
		}

		return {};
	}

	void get_from_future(std::future<void>& future, boost::system::error_code& ec)
	{
		ec.clear();
		try
		{
			future.get();
		}
		catch(const boost::system::system_error& se)
		{
			ec = se.code();
		}
	}

public:
	Socket(boost::asio::io_service& io_service)
		: m_io_service(io_service)
		, m_socket(m_io_service)
		, m_timer(m_io_service)
	{

	}

	bool connect(std::string ip, std::uint16_t port)
	{
		boost::asio::ip::tcp::endpoint endpoint(
			boost::asio::ip::address::from_string(ip),
			port);

		auto future = m_socket.async_connect(endpoint, boost::asio::use_future);

		boost::system::error_code ec;

		ScopedTimer timer(m_timer, m_socket);
		get_from_future(future, ec);

		if(!ec)
		{
			boost::asio::write(m_socket, boost::asio::buffer("Test"));
		}

		m_io_service.stop();
	}

private:
	boost::asio::io_service& m_io_service;
	boost::asio::ip::tcp::socket m_socket;
	boost::asio::deadline_timer m_timer;
};

int main()
{
	try
	{
		boost::asio::io_service io_service;
		boost::asio::io_service::work work(io_service);
		boost::thread thread([&]{ io_service.run(); });

		Socket s(io_service);
		s.connect("127.0.0.1", 1515);

		thread.join();
	}
	catch(const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
