#ifndef CPP_ENCRYPTER_GUI_H
#define CPP_ENCRYPTER_GUI_H
#include <gtkmm.h>

using namespace Gtk;

/*!
 *
 * Class EncrypterWindow
 * 
 * Inheriting from GtK::Window
 * Lists all the child widgets and their respective signal handlers
 * 
 */
class EncrypterWindow : public Gtk::Window
{
public:
    EncrypterWindow();                 
    virtual ~EncrypterWindow();        
    gboolean button_state;             /*!< Keeps track of the toggle button state */ 

protected:
    //Signal handlers:
    void on_button_file_clicked();      /*!< Function called by m_Button_File */
    void on_button_folder_clicked();    /*!< Function called by m_Button_Folder */
    void on_function_toggle();          /*!< Function called by m_Mode_toggle */
    void on_button_submit();            /*!< Function called by m_Button_Submit */

    //Child widgets:
    Grid m_gridBox;                     /*!< Gridbox that contains the main application */
    Button m_Button_File, m_Button_Folder, m_Button_Submit; /*!< Main application buttons */
    ToggleButton m_Mode_Toggle;         /*!< Toggle button for switching between encryption and decryption */
    Entry m_Entry;                      /*!< WIP Entry for providing password for encryption  */
};

#endif //CPP_ENCRYPTER_GUI_H

