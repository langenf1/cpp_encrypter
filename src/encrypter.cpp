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


ExampleWindow::ExampleWindow()
        : m_ButtonBox(Gtk::ORIENTATION_VERTICAL),
          m_Button_File("Choose File"),
          m_Button_Folder("Choose Folder")
{
    set_border_width(10);
    set_title("Gtk::FileSelection example");

    add(m_ButtonBox);

    m_ButtonBox.pack_start(m_Button_File);
    m_Button_File.signal_clicked().connect(sigc::mem_fun(*this,
                                                         &ExampleWindow::on_button_file_clicked) );

    m_ButtonBox.pack_start(m_Button_Folder);
    m_Button_Folder.signal_clicked().connect(sigc::mem_fun(*this,
                                                           &ExampleWindow::on_button_folder_clicked) );

    show_all_children();
}
//destroy window
ExampleWindow::~ExampleWindow()
{
}

void ExampleWindow::on_button_folder_clicked(){

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
        case(Gtk::RESPONSE_OK):
        {
            cout << "Select clicked." << endl;

            string folder = dialog.get_filename();
            string inFile, encrFile, decrFile;

            //loop through directory
            for(auto& p: experimental::filesystem::recursive_directory_iterator(folder)){
                inFile = (basic_string<char> &&) p;
                auto enc = Encrypter(inFile);
                // Encrypt data
                encrFile = enc.encrypt();

                auto dec = Decrypter(encrFile);
                // Decrypt data
                decrFile = dec.decrypt();

                cout << p << endl;
            }

            cout << "Folder selected: " << dialog.get_filename() << endl;
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

void ExampleWindow::on_button_file_clicked(){

    Gtk::FileChooserDialog dialog("Please choose a file",
                                  Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);

    //Add response buttons the the dialog:
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);

    //Add filters, so that only certain file types can be selected:

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Text files");
    filter_text->add_mime_type("text/plain");
    dialog.add_filter(filter_text);

    auto filter_cpp = Gtk::FileFilter::create();
    filter_cpp->set_name("C/C++ files");
    filter_cpp->add_mime_type("text/x-c");
    filter_cpp->add_mime_type("text/x-c++");
    filter_cpp->add_mime_type("text/x-c-header");
    dialog.add_filter(filter_cpp);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog.add_filter(filter_any);

    //Show the dialog and wait for a user response:
    int result = dialog.run();

    //Handle the response:
    switch(result)
    {
        case(Gtk::RESPONSE_OK):
        {
            cout << "Open clicked." << endl;

            //Notice that this is a std::string, not a Glib::ustring.
            string filename = dialog.get_filename();

            string inFile, encrFile, decrFile;
            inFile = filename;

            auto enc = Encrypter(inFile);
            // Encrypt data
            encrFile = enc.encrypt();

            auto dec = Decrypter(encrFile);
            // Decrypt data
            decrFile = dec.decrypt();

            cout << "File selected: " <<  filename << endl;
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
    // Set memory for AES key
    memset(key, 0x00, AES::DEFAULT_KEYLENGTH);
    memset(iv, 0x00, AES::BLOCKSIZE);

    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.encrypter");

    ExampleWindow window;

    return app->run(window);
}