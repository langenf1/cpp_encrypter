#ifndef CPP_ENCRYPTER_ENCRYPTER_H
#define CPP_ENCRYPTER_ENCRYPTER_H

#include <iostream>
#include <string>

using namespace std;

class Encrypter {

public:
    string& inFile;

    explicit Encrypter(string &inFile) : inFile(inFile) {
        this->inFile = inFile;
    }

    Encrypter(string &inFile, string &outFile) : inFile(inFile) {
        this->inFile = inFile;
    }

    string encrypt();
};

class Decrypter {

public:
    string &encrFile;
    string inFile;

    explicit Decrypter(string &encrFile) : encrFile(encrFile) {
        this->encrFile = encrFile;
        this->inFile = (string) encrFile.substr(0, encrFile.size() - 4);
    }

    string decrypt();
};
#endif //CPP_ENCRYPTER_ENCRYPTER_H
