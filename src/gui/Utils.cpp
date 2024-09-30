#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/textfile.h>

#include "Utils.h"

bool Utils::DelegateEvent(wxWindow *win, wxEvent &event)
{
    // Do not call `ProcessWindowEvent` here, for this function is often called in a window's event handling and the
    // window is often parent of `win`, thus the event will bubble up resulting in dead loop.
    if (win != nullptr && win->ProcessWindowEventLocally(event)) {
        return true;
    }
    event.Skip();
    return false;
}

void Utils::ReadAllText(wxString &text, const wxString &fileName)
{
    wxTextFile file(fileName);
    file.Open();
    for (auto str = file.GetFirstLine(); !file.Eof(); str = file.GetNextLine()) {
        text.Append(str).Append("\n");
    }
    file.Close();
}

void Utils::ShowTextBox(const wxString &title, const wxString &text)
{
    wxDialog dlg((wxWindow *)nullptr, wxID_ANY, title);
    auto *sizer = new wxBoxSizer(wxVERTICAL);
    dlg.SetSizer(sizer);
    auto *ctlText =
        new wxTextCtrl(&dlg, wxID_ANY, text, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    sizer->Add(ctlText, wxSizerFlags(1).Expand().Border());
    dlg.CenterOnParent();
    dlg.ShowModal();
}
