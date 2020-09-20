cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(cpp_encrypter VERSION 0.1.0)

# Set C++ defaults and flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lcryptopp -lstdc++fs")

# Find necessary libraries
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)
pkg_check_modules(CRYPTOPP cryptopp)

# Link library directories
link_directories(${GTKMM_LIBRARY_DIRS})
link_directories(${CRYPTOPP_LIBRARY_DIRS})

# Include library directories
include_directories(include ${GTKMM_INCLUDE_DIRS})
include_directories(include ${CRYPTOPP_INCLUDE_DIRS})

# Add the executable file to the project
add_executable(cpp_encrypter ${PROJECT_SOURCE_DIR}/../src/encrypter.cpp)

# Link libraries (CryptoPP, GTKMM, STDC++FS) to the project
target_link_libraries(cpp_encrypter ${GTKMM_LIBRARIES})
target_link_libraries(cpp_encrypter ${CRYPTOPP_LIBRARIES})
target_link_libraries(cpp_encrypter stdc++fs)
