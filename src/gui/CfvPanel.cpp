#include <algorithm>

#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/textctrl.h>
#include <wx/textdlg.h>
#include <wx/tokenzr.h>
#include <wx/treebook.h>
#include <wx/treectrl.h>

#include "CfvPanel.h"

#include "CfvDocument.h"
#include "Defs.h"

IMPLEMENT_DYNAMIC_CLASS(CfvPanel, wxPanel)
IMPLEMENT_TM(CfvPanel)

BEGIN_EVENT_TABLE(CfvPanel, wxPanel)
EVT_UPDATE_UI(ID_INSERT, CfvPanel::OnUpdateInsert)
EVT_MENU(ID_INSERT, CfvPanel::OnInsert)
EVT_UPDATE_UI(wxID_DELETE, CfvPanel::OnUpdateDelete)
EVT_MENU(wxID_DELETE, CfvPanel::OnDelete)
END_EVENT_TABLE()

CfvPanel::CfvPanel(wxWindow *parent) : wxPanel(parent), m_doc(nullptr)
{
    wxLog::AddTraceMask(TM);
    auto *sizer = new wxBoxSizer(wxVERTICAL);
    m_book = new wxTreebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_LEFT);
    sizer->Add(m_book, wxSizerFlags().Expand().Border(wxALL, 0).Proportion(1));
    SetSizer(sizer);
    m_book->GetTreeCtrl()->SetMinSize(wxSize(160, -1));
}

CfvPanel::~CfvPanel()
{
    DeleteAllPages();
}

void CfvPanel::OnUpdate()
{
    std::vector<std::string> names;
    m_doc->ForEachSection([&names](const std::string &name) -> bool {
        names.push_back(name);
        return true;
    });
    if (names.empty()) {
        wxLogStatus(_("File is empty."));
        return;
    }
    std::sort(names.begin(), names.end());
    for (auto const &name : names) {
        wxString content = c(m_doc->GetSection(name));
        AddPage(name, content);
    }
    for (size_t i = 0; i < m_book->GetPageCount(); ++i) {
        m_book->ExpandNode(i);
    }
}

void CfvPanel::SaveContents()
{
    for (size_t i = 0; i < m_book->GetPageCount(); ++i) {
        auto text = wxDynamicCast(m_book->GetPage(i), wxTextCtrl);
        if (text != nullptr && text->IsModified()) {
            m_doc->SaveSection(GetSectionName(i), s(text->GetValue()));
        }
    }
}

void CfvPanel::OnUpdateInsert(wxUpdateUIEvent &event)
{
    event.Enable(m_book->GetTreeCtrl()->HasFocus());
}

void CfvPanel::OnInsert([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    wxTextEntryDialog dlgName(nullptr, _("Input the name of new section:"));
    auto sel = m_book->GetSelection();
    if (sel != wxNOT_FOUND) {
        dlgName.SetValue(GetSectionName(sel));
    }
    if (dlgName.ShowModal() == wxID_OK) {
        wxString name = dlgName.GetValue();
        if (!name.IsEmpty()) {
            AddPage(name, wxEmptyString, true);
            m_doc->Modify(true);
        }
    }
}

void CfvPanel::OnUpdateDelete(wxUpdateUIEvent &event)
{
    event.Enable(m_book->GetTreeCtrl()->HasFocus() && m_book->GetSelection() != wxNOT_FOUND);
}

void CfvPanel::OnDelete([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    int sel = m_book->GetSelection();
    if (sel != wxNOT_FOUND) {
        auto text = wxString::Format(_("Are you sure to delete section \"%s\"?"), m_book->GetPageText(sel));
        int answer = wxMessageBox(text, _("Confirm deleting"), wxYES_NO | wxCENTER);
        if (answer == wxYES) {
            Unbind(sel);
            // Do real deletion.
            m_doc->DeleteSection(GetSectionName(sel));
            m_book->DeletePage(sel);
        }
    }
}

void CfvPanel::AddPage(const wxString &name, const wxString &content, bool dirty)
{
    wxStringTokenizer tks(name, "/");
    int parent = wxNOT_FOUND;
    int current = 0;
    auto tk = tks.GetNextToken();
    for (; current < (int)m_book->GetPageCount(); ++current) {
        int currentParent = m_book->GetPageParent(current);
        if (currentParent == parent) {
            if (tk == m_book->GetPageText(current)) {
                if (!tks.HasMoreTokens()) {
                    wxLogError("Section \"%s\" already exists.", tk);
                    return;
                }
                parent = current;
                tk = tks.GetNextToken();
            }
        } else if (parent != wxNOT_FOUND && currentParent == m_book->GetPageParent(parent)) {
            break;
        }
    }
    do {
        auto text = new wxTextCtrl(m_book, wxID_ANY, content, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
        wxFont monoFont(16, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
        text->SetFont(monoFont);
        text->Bind(wxEVT_TEXT, &CfvDocument::OnChange, m_doc);
        // Important to call this
        if (dirty) {
            // To save the content even not modified.
            text->MarkDirty();
        }
        InsertPage(parent, text, tk, true);
        parent = current;
        ++current;
        tk = tks.GetNextToken();
    } while (!tk.IsEmpty());
}

void CfvPanel::InsertPage(int parent, wxWindow *page, const wxString &text, bool bSelect, int imageId)
{
    if (parent == wxNOT_FOUND) {
        m_book->AddPage(page, text, bSelect, imageId);
    } else {
        m_book->InsertSubPage(parent, page, text, bSelect, imageId);
    }
}

void CfvPanel::DeleteAllPages()
{
    for (size_t i = 0; i < m_book->GetPageCount(); ++i) {
        Unbind(i);
    }
    m_book->DeleteAllPages();
}

void CfvPanel::Unbind(int sel)
{
    auto *text = wxDynamicCast(m_book->GetPage(sel), wxTextCtrl);
    if (text != nullptr) {
        text->Unbind(wxEVT_TEXT, &CfvDocument::OnChange, m_doc);
    }
}

bool CfvPanel::IsLeaf(int sel) const
{
    return sel != wxNOT_FOUND && m_book->GetPage(sel)->IsKindOf(CLASSINFO(wxTextCtrl));
}

std::string CfvPanel::GetSectionName(int sel) const
{
    wxString name = m_book->GetPageText(sel);
    while ((sel = m_book->GetPageParent(sel)) != wxNOT_FOUND) {
        name = m_book->GetPageText(sel) + "/" + name;
    }
    return s(name);
}
