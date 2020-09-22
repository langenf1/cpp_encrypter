# File Encrypter and Decrypter

[![version](https://img.shields.io/badge/version-v1.0.0-brightgreen)]()

This repository was created to provide a public file encrypter and decrypter written
in C++. This code was created in 3 weeks with the purpose of completing an assignment for
by the HvA Technische Informatica Specifiek Themasemester.

## Getting Started

#### Installation

To be able to run the code in this repository, the required packages must be installed:   

* Crypto++  
* GTKMM 3.0
* CMAKE

#### Building

On windows open cmd and head over to the build directory and make sure you have Cygwin make installed or another
program that is capable of handling Makefiles. Type `make -f Makefile` in your command prompt.

On other OS's make sure you have a program installed that can handle Makefiles, on Linux
you will most likely have to install the build essentials such as 
gcc, gcc-c++, make, automake, kernel-devel. The package names differ per OS. Head over to the build
directory in your terminal and simply type `make`.

#### Launching

You can launch the program by executing the following command:   
`./build/cpp_encrypter`


#### Generating documentation
To generate documentation for this program first install DoxyGen using:   
`sudo apt-get install doxygen`

Followed by executing the terminal command from the directory *cpp_encrypter/docs*:   
`doxygen Doxyfile`

By editing the *doxygen* configuration file one can change the release version, documented files to be parsed and more.

