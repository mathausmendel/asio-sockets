#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/range/irange.hpp>
#include <memory>

void set_affinity(std::size_t cpu, pthread_t thread)
{
	cpu_set_t cpuset{};
	CPU_ZERO(&cpuset);
	CPU_SET(cpu, &cpuset);

	pthread_setaffinity_np(thread, sizeof(cpuset), &cpuset);
}

int main()
{
	// Setting up the io_service
	boost::asio::io_service io_service;
	boost::asio::io_service::work work(io_service);
	boost::thread_group threads;
	boost::mutex mutex;

	const auto concurrency = boost::thread::physical_concurrency();
	boost::barrier barrier(concurrency);

	for(const auto i : boost::irange<decltype(concurrency)>(0, concurrency))
	{
		boost::thread* thread = threads.create_thread([&]
		{
			barrier.wait();
			io_service.run();
		});

		set_affinity(i, thread->native_handle());
	}

	threads.join_all();

	return EXIT_SUCCESS;
}
