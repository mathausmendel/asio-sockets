add_executable(thread_pinning thread_pinning.cpp)

target_link_libraries(thread_pinning
	${CMAKE_THREAD_LIBS_INIT}
	${Boost_THREAD_LIBRARY}
	${Boost_SYSTEM_LIBRARY})
