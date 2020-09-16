
cmake_minimum_required(VERSION 3.10)

# set the project name
project(cpp_encrypter )


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lcryptopp")

# add the executable
add_executable(cpp_encrypter /home/jonas/ClionProjects/cpp_encrypter/src/encrypter.cpp)

#cant find exec
#add_executable(cpp_encrypter ${PROJECT_SOURCE_DIR}/../src/encrypter.cpp)

# set the project name and version
project(cpp_encrypter VERSION 0.1.0)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK "gtkmm-3.0")
if (GTK_FOUND)
    #target_link_libraries(encrypter.cpp ${GTK_LIBRARIES})
    add_definitions(${GTK_CFLAGS} ${GTK_CFLAGS_OTHER})
endif()




