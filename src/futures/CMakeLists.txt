add_executable(futures futures.cpp)

target_link_libraries(futures
	${CMAKE_THREAD_LIBS_INIT}
	${Boost_THREAD_LIBRARY}
	${Boost_SYSTEM_LIBRARY})
