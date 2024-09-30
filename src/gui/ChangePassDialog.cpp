#include <wx/msgdlg.h>
#include <wx/textctrl.h>
#include <wx/xrc/xmlres.h>

#include "ChangePassDialog.h"

ChangePassDialog::ChangePassDialog(wxWindow *parent, const wxString &currentPass)
    : wxDialog()
    , m_currentPass(currentPass)
    , m_newPass()
{
    wxXmlResource::Get()->LoadDialog(this, parent, "dlgChangePass");
}

bool ChangePassDialog::TransferDataFromWindow()
{
    auto *passCurrent = XRCCTRL(*this, "passCurrent", wxTextCtrl);
    if (passCurrent->GetValue() != m_currentPass) {
        wxMessageBox(_("Wrong password!"));
        return false;
    }
    auto *passNew = XRCCTRL(*this, "passNew", wxTextCtrl);
    auto *passNew2 = XRCCTRL(*this, "passNew2", wxTextCtrl);
    if (passNew2->GetValue() != passNew->GetValue()) {
        wxMessageBox(_("Confirm new password failed!"));
        return false;
    }
    m_newPass = passNew->GetValue();
    return true;
}
