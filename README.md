# File Encrypter and Decrypter

[![version](https://img.shields.io/badge/version-v1.0.0-brightgreen)]()

This repository was created to provide a public inFile encrypter and decrypter written
in C++. This code was created in 3 weeks with the purpose of completing an assignment for
by the HvA Technische Informatica Specifiek Themasemester.

## Getting Started

#### Installation

To be able to run the code in this repository, the required packages must be installed:   

* CryptoPP   
* GTKMM 3.0
* CMake

#### Building
First of all you must create the Makefile using CMake. To do this head over to the build directory and run the command `cmake .`.

On Windows make sure you have 'Cygwin make' installed or another program that is capable of handling Makefiles. 
Type `make -f Makefile` in your command prompt.

On other OS's make sure you have another program installed that can handle Makefiles, on Linux
you will most likely have to install the build essentials such as 
gcc, gcc-c++, make, automake, kernel-devel. The package names differ per OS. Once again head over to the build
directory in your terminal and simply type `make`.

#### Launching

You can launch the program by executing the following command in the build directory:   
`./cpp_encrypter`
