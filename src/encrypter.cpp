/**
@file main.cpp
*/
#include <iostream>
#include <string>
#include <fstream>
#include <experimental/filesystem>
#include "crypto++/modes.h"
#include "crypto++/aes.h"
#include "crypto++/filters.h"
#include "crypto++/hkdf.h"
#include "crypto++/sha.h"
#include "encrypter.h"
#include "gui.h"

using namespace std;
using namespace CryptoPP;

byte key[AES::DEFAULT_KEYLENGTH], iv[AES::BLOCKSIZE];
string filesToProcess, cred;

/*!
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

/*!
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

/*!
 * Function used to encrypt data of a file.
 *
 * @param
 * string inFile: Filename of original file which needs to be encrypted
 * string outFile: Filename of output file chosen by user
 */
string Encrypter::encrypt(){
    string plaintext, ciphertext, encrFile, outFile;

    plaintext = readFile(filesToProcess);

    // Dump Plain Text
    cout << "Plain Text (" << plaintext.size() << " bytes)" << endl;
    cout << plaintext;
    cout << endl << endl;

    // Encrypt Text
    AES::Encryption aesEncryption(key, AES::DEFAULT_KEYLENGTH);
    CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv);

    StreamTransformationFilter stfEncryptor(cbcEncryption, new StringSink( ciphertext));
    stfEncryptor.Put(reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.length() + 1);
    stfEncryptor.MessageEnd();

    // Write to new file
    outFile = inFile + string(".enc");
    encrFile = writeFile(outFile, ciphertext);

    if(remove(filesToProcess.c_str()) != 0 ){
        perror( "Error deleting file" );
    }

    return encrFile;
}

/*!
 * Function used to decrypt encrypted data of a file.
 *
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

    // Remove empty bits of string
    const char* str = decryptedtext.c_str();
    decrFile = writeFile(inFile, str);

   if(remove(filesToProcess.c_str()) != 0 ){
        perror( "Error deleting file" );
    }

    return decrFile;
}

/*!
 *
 * Contains the configurations and function matching of the UI.
 * 
 */
EncrypterWindow::EncrypterWindow()
        : m_Button_Submit("Encrypt"),
          m_Button_File("Choose File"),
          m_Button_Folder("Choose Folder"),
          m_Mode_Toggle("Encrypt"),
          m_Entry()
          {

    set_border_width(10);
    set_default_size(220,50);
    set_title("File encrypter");

    add(m_gridBox);

    m_gridBox.add(m_Mode_Toggle);
    m_Mode_Toggle.signal_clicked().connect(sigc::mem_fun(*this,
                                                            &EncrypterWindow::on_function_toggle) );
    m_gridBox.add(m_Button_File);
    m_Button_File.signal_clicked().connect(sigc::mem_fun(*this,
                                                         &EncrypterWindow::on_button_file_clicked) );
    
    m_gridBox.attach_next_to(m_Entry, POS_BOTTOM, 2, 1);
    m_Entry.set_visibility(false);

    m_gridBox.attach_next_to(m_Button_Submit,POS_BOTTOM, 2, 1);
    m_Button_Submit.signal_clicked().connect(sigc::mem_fun(*this,
                                                           &EncrypterWindow::on_button_submit) );

    show_all_children();
}

/*!
 *
 * Responsible for destroying the UI Window when 
    the application is done.
 * 
 */
EncrypterWindow::~EncrypterWindow()
{
}

/*!
 *
 * Toggles the text in the GUI to the current active function.
 * 
 */
void EncrypterWindow::on_function_toggle(){
    string text;
    (m_Mode_Toggle.get_active() == 1) ? (text = "Decrypt") : (text = "Encrypt");
    m_Button_Submit.set_label(text);
    m_Mode_Toggle.set_label(text);
}

/*!
 *
 * Checks if the supplied files are encrypted using the extension.
 * 
 */
bool CheckEncryptionStatus(string inFile){
    return (inFile.find(".enc") != string::npos) ? (false) : (true); 
}

/*!
 *
 * Button function which either encrypts or decrypts if the conditions are met.
 * 
 */
void EncrypterWindow::on_button_submit(){
    string text = m_Entry.get_text();
    string output;

    // TODO: Check for invalid characters.
    if(text == ""){
        cout << "no password has been entered" << endl;
    }
    else{
        cout <<  text << endl;
        if(filesToProcess != ""){
            cout << filesToProcess << endl; 
            
            // A valid password has been entered and file name has been entered

            int button_state = m_Mode_Toggle.get_active();
            cout << button_state << endl;
            // Encrypt state
            if(button_state == 0){
                //check if file is not encrypted
                if (CheckEncryptionStatus(filesToProcess) == true){
                    auto enc = Encrypter(filesToProcess);
                    output = enc.encrypt();
                }
                else{
                    cout << filesToProcess << endl;
                    cout << "Cannot encrypt" << endl;
                }

            //Decrypt state
            } else if(button_state == 1) {
                //check if file is encrypted
                if (CheckEncryptionStatus(filesToProcess) == false){
                    auto dec = Decrypter(filesToProcess);
                    output = dec.decrypt();
                }
                else{
                    cout << filesToProcess << endl;
                    cout << "Cannot decrypt" << endl;
                }
            }
        }
    }
}

/*!
 *
 * Button function that allows the selection of a file to be encrypted.
 * 
 */
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
            string encrFile, decrFile;
            filesToProcess  = dialog.get_filename();
            cout << filesToProcess << endl; 
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

/*!
 *
 * The main function of the application.
 * 
 */
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

