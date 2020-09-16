/*
 *  g++ encrypter.cpp -o AesEncrypt -lcryptopp
 */
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "cryptopp/modes.h"
#include "cryptopp/aes.h"
#include "cryptopp/filters.h"
#include "encrypter.h"

using namespace std;
using namespace CryptoPP;

// AES key
byte key[AES::DEFAULT_KEYLENGTH], iv[AES::BLOCKSIZE];


/*
 * Function for reading data from a chosen file.
 *
 * @param
 * string file: Filename of file which is read
 */
string readFile(const string& file){
    string data;

    // Read file and store in buffer
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
 * string file: Filename to which the data should be writen to
 * string data: Data that must be put into the file
 */
string writeFile(string file, const string& data){

    ofstream o(file);
    o << data;
    o.close();
    return file;
}

/*
 * Function used to encrypt data of a file.
 *
 * @param
 * string inFile: Filename of original file which needs to be encrypted
 * string outFile: Filename of output file chosen by user
 */
string Encrypter::encrypt(){
    string plaintext, ciphertext, encrFile, outFile;

    plaintext = readFile(inFile);

    // Dump Plain Text
    cout << "Plain Text (" << plaintext.size() << " bytes)" << endl;
    cout << plaintext;
    cout << endl << endl;

    // Encrypt Text
    AES::Encryption aesEncryption(key, AES::DEFAULT_KEYLENGTH);
    CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv );

    StreamTransformationFilter stfEncryptor(cbcEncryption, new StringSink( ciphertext));
    stfEncryptor.Put(reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.length() + 1);
    stfEncryptor.MessageEnd();

    // Write to new file
    outFile = inFile + string(".enc");
    encrFile = writeFile(outFile, ciphertext);

    return encrFile;
}

/*
 * Function used to decrypt encrypted data of a file.
 *
 * @param
 * string encrFile: Filename of encrypted data
 * string outFile: Filename of output file chosen by user
 */
string Decrypter::decrypt(){
    string decryptedtext, ciphertext, decrFile;

    ciphertext = readFile(encrFile);

    // Dump Cipher Text
    cout << "Cipher Text" << endl;
    cout << ciphertext;
    cout << endl << endl;

    // Decrypt
    AES::Decryption aesDecryption(key, AES::DEFAULT_KEYLENGTH);
    CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

    StreamTransformationFilter stfDecryptor(cbcDecryption, new StringSink(decryptedtext));
    stfDecryptor.Put(reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size());
    stfDecryptor.MessageEnd();

    // Dump Decrypted Text
    cout << "Decrypted Text: " << endl;
    cout << decryptedtext;
    cout << endl << endl;

    // Remove empty bits of string
    const char* str = decryptedtext.c_str();
    decrFile = writeFile(inFile, str);

    return decrFile;
}

int main(int argc, char* argv[]) {
    // Set memory for AES key
    memset(key, 0x00, AES::DEFAULT_KEYLENGTH);
    memset(iv, 0x00, AES::BLOCKSIZE);

    string inFile, encrFile, decrFile;

    // Take command line arguments
    if (argc == 2) {
        inFile = argv[1];
    } else {
        cout << "Usage: ./AesEncrypt <InputFile> \n";
        return 1;
    }

    auto enc = Encrypter(inFile);
    // Encrypt data
    encrFile = enc.encrypt();

    auto dec = Decrypter(encrFile);
    // Decrypt data
    decrFile = dec.decrypt();

    return 0;
}