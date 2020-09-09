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
string encrypt(const string& inFile, string outFile){
    string plaintext, ciphertext, encrFile;

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
    outFile = outFile + string(".enc");
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
string decrypt(const string& encrFile, const string& outFile){
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

    decrFile = writeFile(outFile, decryptedtext);

    // Remove empty bits of string
    const char* str = decryptedtext.c_str();
    decrFile = writeFile(outFile, str);

    return decrFile;
}

int main(int argc, char* argv[]) {
    // Set memory for AES key
    memset(key, 0x00, AES::DEFAULT_KEYLENGTH);
    memset(iv, 0x00, AES::BLOCKSIZE);

    string inFile, outFile, encrFile, decrFile;

    // Take command line arguments
    if (argc == 3) {
        inFile = argv[1];
        outFile = argv[2];
    }
    else {
        cout << "Usage: ./AesEncrypt <InputFile> <OutputFile>\n";
        return 1;
    }

    // Encrypt data
    encrFile = encrypt(inFile, outFile);

    // Decrypt data
    decrFile = decrypt(encrFile, outFile);

    return 0;
}
