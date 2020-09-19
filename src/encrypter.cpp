/*
 *  g++ encrypter.cpp -o AesEncrypt -lcryptopp -lstdc++fs `pkg-config gtkmm-3.0 --cflags --libs`
 */
#include <iostream>
#include <string>
#include <fstream>
#include <experimental/filesystem>
#include "cryptopp/modes.h"
#include "cryptopp/aes.h"
#include "cryptopp/filters.h"
#include "encrypter.h"
#include "gui.h"

using namespace std;
using namespace CryptoPP;

// AES 16 bytes key
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
    cout << "Cipher Text (" << ciphertext.size() << " bytes)" << endl;
    cout << ciphertext;
    cout << endl << endl;

    // Decrypt
    AES::Decryption aesDecryption(key, AES::DEFAULT_KEYLENGTH);
    CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

    StreamTransformationFilter stfDecryptor(cbcDecryption, new StringSink(decryptedtext));
    stfDecryptor.Put(reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size());
    stfDecryptor.MessageEnd();

    // Dump Decrypted Text
    cout << "Decrypted Text (" << decryptedtext.size() << " bytes)" << endl;
    cout << decryptedtext;
    cout << endl << endl;

    // Remove empty bits of string
    const char* str = decryptedtext.c_str();
    decrFile = writeFile(inFile, str);

    return decrFile;
}


EncrypterWindow::EncrypterWindow()
        : m_Button_Toggle("Decrypt"),
          m_Button_File("Choose File"),
          m_Button_Folder("Choose Folder"){

    set_border_width(10);
    set_default_size(250,50);
    set_title("File encrypter");

    add(m_gridBox);

    m_gridBox.add(m_Button_File);
    m_Button_File.signal_clicked().connect(sigc::mem_fun(*this,
                                                         &EncrypterWindow::on_button_file_clicked) );

    m_gridBox.add(m_Button_Folder);
    m_Button_Folder.signal_clicked().connect(sigc::mem_fun(*this,
                                                           &EncrypterWindow::on_button_folder_clicked) );

    m_gridBox.attach_next_to(m_Button_Toggle,POS_BOTTOM, 2, 1);
    m_Button_Toggle.signal_toggled().connect(sigc::mem_fun(*this,
                                                           &EncrypterWindow::on_button_encryption_toggle) );
    show_all_children();
}

//destroy window
EncrypterWindow::~EncrypterWindow()
{
}


void EncrypterWindow::on_button_encryption_toggle() {
    string text;

    button_state = m_Button_Toggle.get_active();

    // boolean is zero from start
    if(button_state == 0){
        cout << "Decrypt state - " << button_state << endl;
        text = "Decrypt";
        m_Button_Toggle.set_label(text);

    } else if(button_state == 1){
        cout << "Encrypt state - " << button_state << endl;
        text = "Encrypt";
        m_Button_Toggle.set_label(text);
    }

}

void EncrypterWindow::on_button_folder_clicked(){

    Gtk::FileChooserDialog dialog("Please choose a folder",
                                  Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
    dialog.set_transient_for(*this);

    //Add response buttons the the dialog:
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("Select", Gtk::RESPONSE_OK);

    int result = dialog.run();

    //Handle the response:
    switch(result)
    {
        case(Gtk::RESPONSE_OK): {
            cout << "Select clicked." << endl;

            string inFile, encrFile, decrFile;
            string folder = dialog.get_filename();

            cout << "Folder selected: " << dialog.get_filename() << endl;

            //loop through directory
            for (auto &p: experimental::filesystem::recursive_directory_iterator(folder)) {

                inFile = (basic_string<char> &&) p;

                if (button_state == 1) {
                    //check if file is not encrypted
                    if (!(inFile.find(".enc") != string::npos)) {
                        cout << "Encrypt file: " << inFile << endl;
                        // Encrypt data
                        auto enc = Encrypter(inFile);
                        encrFile = enc.encrypt();
                    } else {
                        cout << inFile << " can not be encrypted" << endl;
                    }

                } else if (button_state == 0) {
                    //check if file is encrypted
                    if (inFile.find(".enc") != string::npos) {
                        cout << "Decrypt file: " << inFile << endl;
                        // Decrypt data
                        auto dec = Decrypter(inFile);
                        decrFile = dec.decrypt();
                    } else {
                        cout << inFile << " can not be decrypted" << endl;
                    }
                }
            }
            break;
        }
        case(Gtk::RESPONSE_CANCEL):
        {
            cout << "Cancel clicked." << endl;
            break;
        }
        default:
        {
            cout << "Unexpected button clicked." << endl;
            break;
        }
    }
}

void EncrypterWindow::on_button_file_clicked(){

    Gtk::FileChooserDialog dialog("Please choose a file",
                                  Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);

    //Add response buttons the the dialog:
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);

    //Show the dialog and wait for a user response:
    int result = dialog.run();

    //Handle the response:
    switch(result)
    {
        case(Gtk::RESPONSE_OK):
        {
            cout << "Open clicked." << endl;

            string inFile, encrFile, decrFile;
            inFile  = dialog.get_filename();    //Notice that this is a std::string, not a Glib::ustring.

            //encrypt state
            if(button_state == 1){
                //check if file is not encrypted
                if (!(inFile.find(".enc") != string::npos)) {
                    cout << "Encrypt file: " << inFile << endl;
                    // Encrypt data
                    auto enc = Encrypter(inFile);
                    encrFile = enc.encrypt();
                }else {
                    cout << inFile << " can not be encrypted" << endl;
                }
            //Decrypt state
            } else if(button_state == 0) {
                //check if file is encrypted
                if (inFile.find(".enc") != string::npos) {
                    cout << "Decrypt file: " << inFile << endl;
                    // Decrypt data
                    auto dec = Decrypter(inFile);
                    decrFile = dec.decrypt();
                } else {
                    cout << inFile << " can not be decrypted" << endl;
                }
            }
            break;
        }
        case(Gtk::RESPONSE_CANCEL):
        {
            cout << "Cancel clicked." << endl;
            break;
        }
        default:
        {
            cout << "Unexpected button clicked." << endl;
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    //Set memory for AES key
    memset(key, 0x00, AES::DEFAULT_KEYLENGTH);
    memset(iv, 0x00, AES::BLOCKSIZE);

    //Create Gtkmm application
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.encrypter");

    //create app window
    EncrypterWindow window;

    return app->run(window);
}