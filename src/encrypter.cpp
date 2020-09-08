/*
 *  g++ encrypter.cpp -o AesEncrypt -lcryptopp
 */
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <streambuf>
#include "cryptopp/modes.h"
#include "cryptopp/aes.h"
#include "cryptopp/filters.h"

using namespace std;

//AES key
byte key[CryptoPP::AES::DEFAULT_KEYLENGTH], iv[CryptoPP::AES::BLOCKSIZE];


/*
 * Function for reading data from a chosen file.
 *
 * @param
 * string file: filename of file which is read
 */
string readFile(string file){
    string data;

    //Read file and store in buffer
    ifstream t(file);
    stringstream bufferIn;
    bufferIn << t.rdbuf();
    data = bufferIn.str();

    return data;
}

/*
 * Function for writing data to a chosen file.
 *
 * @param
 * string file: filename to which the data should be writen to
 * string data: data that must be put into the file
 */
string writeFile(string file, string data){

    ofstream o(file);
    o << data;
    o.close();
    return file;
}

/*
 * Function used to encrypt data of a file.
 *
 * @param
 * string inFile: filename of orignal file which needs to be encrypted
 * string outFile: filename of outputfile chosen by user
 */
string encrypt(string inFile, string outFile){

    string plaintext;
    string ciphertext;
    string encrFile;

    plaintext = readFile(inFile);

    // Dump Plain Text
    cout << "Plain Text (" << plaintext.size() << " bytes)" << endl;
    cout << plaintext;
    cout << endl << endl;

    // encrypt Text
    CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv );

    CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink( ciphertext ) );
    stfEncryptor.Put( reinterpret_cast<const unsigned char*>( plaintext.c_str() ), plaintext.length() + 1 );
    stfEncryptor.MessageEnd();

    //write to new file
    outFile = outFile + string(".enc");
    encrFile = writeFile(outFile, ciphertext);

    return encrFile;
}

/*
 * Function used to decrypt encrypted data of a file.
 *
 * @param
 * string encrFile: filename of encrypted data
 * string outFile: filename of outputfile chosen by user
 */
string decrypt(string encrFile, string outFile){

    string decryptedtext;
    string ciphertext;
    string decrFile;

    ciphertext = readFile(encrFile);

    //Dump Cipher Text
    cout << "Cipher Text " << endl;
    cout << ciphertext;
    cout << endl << endl;

    // Decrypt
    CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, iv );

    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink( decryptedtext ) );
    stfDecryptor.Put( reinterpret_cast<const unsigned char*>( ciphertext.c_str() ), ciphertext.size() );
    stfDecryptor.MessageEnd();


    // Dump Decrypted Text
    cout << "Decrypted Text: " << endl;
    cout << decryptedtext;
    cout << endl << endl;


    decrFile = writeFile(outFile, decryptedtext);

    const char* str = decryptedtext.c_str(); //remove empty bits of string
    decrFile = writeFile(outFile, str);

    return decrFile;
}

int main(int argc, char* argv[]) {
    //set memory for AES key
    memset(key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
    memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);

    string inFile;
    string outFile;
    string encrFile;
    string decrFile;

    //Take command line arguments
    if(argc == 3) {
        inFile = argv[1];
        outFile = argv[2];
    }
    else {
        cout << "Usage: ./AesEncrypt InputFile OutputFile\n";
        return 1;
    }

    //encrypt data
    encrFile = encrypt(inFile, outFile);

    //decrypt data
    decrFile = decrypt(encrFile,outFile);

    return 0;
}
