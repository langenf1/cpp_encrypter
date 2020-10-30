#ifndef CPP_ENCRYPTER_ENCRYPTER_H
#define CPP_ENCRYPTER_ENCRYPTER_H

#include <iostream>
#include <string>

using namespace std;

/*!
 *
 * Encryption class
 * TODO - ADD detailed description
 * 
 */
class Encrypter {

public:
    string& inFile;     /*!< Contains the file location of the decrypted input file. */

    explicit Encrypter(string &inFile) : inFile(inFile) {
        this->inFile = inFile;
    }

    /*! TODO */
    Encrypter(string &inFile, string &outFile) : inFile(inFile) {
        this->inFile = inFile;
    }

    string encrypt();   /*!< TODO */
};

/*!
 *  
 * Decryption class
 * TODO -  Detailed discription
 * 
 */
class Decrypter {

public:
    string &encrFile;   /*!< Contains the content of the decrypted file */
    string inFile;      /*!< Contains the file location of the encrypted input file*/

    explicit Decrypter(string &encrFile) : encrFile(encrFile) {
        this->encrFile = encrFile;
        this->inFile = (string) encrFile.substr(0, encrFile.size() - 4);
    }

    string decrypt();   /*!< TODO */
};
#endif //CPP_ENCRYPTER_ENCRYPTER_H

