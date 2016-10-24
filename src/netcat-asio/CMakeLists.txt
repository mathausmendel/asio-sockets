add_executable(netcat-asio netcat-asio.cpp)

target_link_libraries(netcat-asio
	${CMAKE_THREAD_LIBS_INIT}
	${Boost_PROGRAM_OPTIONS_LIBRARY}
	${Boost_SYSTEM_LIBRARY})
