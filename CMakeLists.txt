cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(asio-sockets)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CPACK_PACKAGE_CONTACT "Math�us Mendel")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Boost.Asio sockets tests and examples")
set(CPACK_PACKAGE_VERSION_MAJOR 1)
set(CPACK_PACKAGE_VERSION_MINOR 0)
set(CPACK_PACKAGE_VERSION_PATCH 0)
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

include(CPack)

find_package(Boost REQUIRED COMPONENTS
	program_options
	system
	thread)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads)

if(CMAKE_USE_PTHREADS_INIT)
	set(CMAKE_THREAD_LIBS_INIT "pthread")
endif()

add_subdirectory(src)
