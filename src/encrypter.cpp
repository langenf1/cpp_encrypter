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

int main(int argc, char* argv[]) {

    byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv[ CryptoPP::AES::BLOCKSIZE ];
    memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );


    string inFile;
    string outFile;

    //Take command line arguments
    if( argc == 3 ) {
        inFile = argv[1];
        outFile = argv[2];
    }
    else {
        cout << "Usage: ./AesEncrypt InputFile OutputFile\n";
        return 1;
    }



    string plaintext;
    string decryptedtext;
    string ciphertext ;

    //Read file and store in buffer
    ifstream t(inFile);
    stringstream bufferIn;
    bufferIn << t.rdbuf();
    plaintext = bufferIn.str();


    //
    // Dump Plain Text
    //
    cout << "Plain Text (" << plaintext.size() << " bytes)" << endl;
    cout << plaintext;
    cout << endl << endl;

    //
    // encrypt Text
    //
    CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv );

    CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink( ciphertext ) );
    stfEncryptor.Put( reinterpret_cast<const unsigned char*>( plaintext.c_str() ), plaintext.length() + 1 );
    stfEncryptor.MessageEnd();

    //write to new file
    ofstream o(outFile + string(".encrypt"));

    o << ciphertext;
    o.close();
    //
    // Dump Cipher Text
    //
    cout << "Cipher Text (" << ciphertext.size() << " bytes)" << endl;
    cout << ciphertext << endl;
    for( int i = 0; i < ciphertext.size(); i++ ) {

        cout << "0x" << hex << (0xFF & static_cast<byte>(ciphertext[i])) << " ";
    }

    cout << endl << endl;

//test for checking if it can convert back encrypted file
//    ifstream test("testOUT.txt.encrypt");
//    stringstream buffertest;
//    buffertest<< test.rdbuf();
//    ciphertext = buffertest.str();

    //
    // Decrypt
    //
    CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, iv );

    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink( decryptedtext ) );
    stfDecryptor.Put( reinterpret_cast<const unsigned char*>( ciphertext.c_str() ), ciphertext.size() );
    stfDecryptor.MessageEnd();

    //
    // Dump Decrypted Text
    //
    cout << "Decrypted Text: " << endl;
    cout << decryptedtext;
    cout << endl << endl;

    return 0;
}
