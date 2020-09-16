//
// Created by jonas on 15-09-20.
//

#ifndef CPP_ENCRYPTER_GUI_H
#define CPP_ENCRYPTER_GUI_H

#include <gtkmm.h>

using namespace Gtk;

class ExampleWindow : public Gtk::Window
{
public:
    ExampleWindow();
    virtual ~ExampleWindow();

protected:
    //Signal handlers:
    void on_button_file_clicked();
    void on_button_folder_clicked();

    //Child widgets:
    ButtonBox m_ButtonBox;
    Button m_Button_File, m_Button_Folder;
};

#endif //CPP_ENCRYPTER_GUI_H
