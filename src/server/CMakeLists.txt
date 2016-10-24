add_executable(server server.cpp)
target_link_libraries(server
	${Boost_SYSTEM_LIBRARY}
	${Boost_THREAD_LIBRARY})
