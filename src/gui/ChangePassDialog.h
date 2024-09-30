#ifndef _GUI_CHANGE_PASS_DIALOG_H_
#define _GUI_CHANGE_PASS_DIALOG_H_

#include <wx/dialog.h>

class ChangePassDialog : public wxDialog
{
public:
    ChangePassDialog(wxWindow *parent, const wxString &currentPass);

    const wxString &GetPass() const
    {
        return m_newPass;
    }

protected:
    wxString m_currentPass;
    wxString m_newPass;

    bool TransferDataFromWindow() override;
};

#endif /* _GUI_CHANGE_PASS_DIALOG_H_ */
