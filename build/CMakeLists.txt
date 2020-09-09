cmake_minimum_required(VERSION 3.10)

# set the project name
project(cpp_encrypter)

# add the executable
add_executable(cpp_encrypter ${PROJECT_SOURCE_DIR}/../src/encrypter.cpp)

# set the project name and version
project(cpp_encrypter VERSION 0.1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lcryptopp")
