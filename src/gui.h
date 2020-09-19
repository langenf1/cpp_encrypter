#ifndef CPP_ENCRYPTER_GUI_H
#define CPP_ENCRYPTER_GUI_H

#include <gtkmm.h>

using namespace Gtk;

class EncrypterWindow : public Gtk::Window
{
public:
    EncrypterWindow();
    virtual ~EncrypterWindow();
    gboolean button_state;

protected:
    //Signal handlers:
    void on_button_file_clicked();
    void on_button_folder_clicked();
    void on_button_encryption_toggle();

    //Child widgets:
    Grid m_gridBox;
    Button m_Button_File, m_Button_Folder;
    ToggleButton m_Button_Toggle;

};

#endif //CPP_ENCRYPTER_GUI_H
