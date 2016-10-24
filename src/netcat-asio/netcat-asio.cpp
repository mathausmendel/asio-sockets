#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/asio.hpp>

std::string readInput()
{
	std::string input;

	std::string buffer;
	while(std::getline(std::cin, buffer))
		input += buffer;

	return input;
}

void sendToHost(const std::string& input,
				const std::string& hostname,
				const std::string& port)
{
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::socket socket(io_service);

	boost::asio::ip::tcp::resolver resolver(io_service);
	boost::asio::connect(socket, resolver.resolve({hostname, port}));

	boost::asio::write(socket, boost::asio::buffer(input));
}

int main(int argc, char** argv)
{
	namespace po = boost::program_options;
	po::options_description desc("options");
	desc.add_options()
			("help,h", "this cruft");

	po::variables_map vm;
	try
	{
		po::parsed_options parsed =
			po::command_line_parser(argc, argv)
				.options(desc)
				.allow_unregistered()
				.run();

		po::store(parsed,
				  vm);

		if(vm.count("help"))
		{
			std::cout << desc << std::endl;
			return EXIT_SUCCESS;
		}

		const std::vector<std::string> arguments =
				po::collect_unrecognized(parsed.options, po::include_positional);

		if(arguments.size() < 2)
		{
			std::cout << "no port[s] to connect to" << std::endl;
			return EXIT_FAILURE;
		}

		auto it = arguments.cbegin();
		const std::string& hostname = *(it++);

		std::string input = readInput();

		for(auto port = it; port != arguments.end(); ++port)
		{
			sendToHost(input, hostname, *port);
		}

		po::notify(vm);
	}
	catch(const po::error&)
	{
		return EXIT_FAILURE;
	}
	catch(const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
