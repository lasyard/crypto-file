#include <wx/log.h>
#include <wx/msgdlg.h>
#include <wx/textdlg.h>

#include "CfvDocument.h"

#include "CfvView.h"
#include "ChangePassDialog.h"
#include "Defs.h"

#include "../Cache.h"
#include "../Sqlite3Store.h"

IMPLEMENT_DYNAMIC_CLASS(CfvDocument, wxDocument)
IMPLEMENT_TM(CfvDocument)

BEGIN_EVENT_TABLE(CfvDocument, wxDocument)
EVT_UPDATE_UI(ID_CHANGE_PASS, CfvDocument::OnUpdateChangePass)
EVT_MENU(ID_CHANGE_PASS, CfvDocument::OnChangePass)
END_EVENT_TABLE()

const char *const CfvDocument::IV = APP_NAME;

CfvDocument::CfvDocument() : wxDocument(), m_doc(new Cache()), m_pass()
{
    wxLog::AddTraceMask(TM);
}

CfvDocument::~CfvDocument()
{
    delete m_doc;
}

bool CfvDocument::OnNewDocument()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    // Updating must be emitted explicitly here.
    UpdateAllViews();
    return wxDocument::OnNewDocument();
}

bool CfvDocument::OnCloseDocument()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    return wxDocument::OnCloseDocument();
}

// Called twice when closing, one in doc close, one in view close.
bool CfvDocument::DeleteContents()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    return wxDocument::DeleteContents();
}

bool CfvDocument::DoOpenDocument(const wxString &fileName)
{
    wxLogTrace(TM, "\"%s(%s)\" called.", __WXFUNCTION__, fileName);
    wxPasswordEntryDialog dlgPass(nullptr, _("Input the password for the file:"));
    if (dlgPass.ShowModal() == wxID_OK) {
        m_pass = dlgPass.GetValue();
        try {
            auto store = new Sqlite3Store(s(fileName), s(m_pass), IV);
            m_doc->attach(store);
            return true;
        } catch (std::runtime_error &e) {
            wxLogError("Failed to open \"%s\": %s", (const char *)fileName, e.what());
        }
    }
    return false;
}

bool CfvDocument::DoSaveDocument(const wxString &fileName)
{
    wxLogTrace(TM, "\"%s(%s)\" called.", __WXFUNCTION__, fileName);
    auto *view = GetView();
    if (view != nullptr) {
        view->SaveContents();
    }
    auto *store = new Sqlite3Store(s(fileName), s(m_pass), IV);
    m_doc->saveAs(store);
    return true;
}

void CfvDocument::Modify(bool modified)
{
    wxLogTrace(TM, "\"%s(%i)\" called.", __WXFUNCTION__, modified);
    wxDocument::Modify(modified);
}

const std::string &CfvDocument::GetSection(const std::string &name) const
{
    return m_doc->get(name);
}

void CfvDocument::SaveSection(const std::string &name, const std::string &content)
{
    m_doc->put(name, content);
    // Modify flag is set when doc is being edited in view, so need not set here.
}

void CfvDocument::DeleteSection(const std::string &name)
{
    m_doc->remove(name);
    Modify(true);
}

void CfvDocument::ForEachSection(std::function<bool(const std::string &)> callback) const
{
    m_doc->forEach(callback);
}

void CfvDocument::OnChange(wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    Modify(true);
    event.Skip();
}

void CfvDocument::OnUpdateChangePass(wxUpdateUIEvent &event)
{
    event.Enable(true);
}

void CfvDocument::OnChangePass([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    ChangePassDialog dlg(nullptr, m_pass);
    if (dlg.ShowModal() == wxID_OK) {
        auto pass = dlg.GetPass();
        CryptoStore *store = dynamic_cast<CryptoStore *>(m_doc->getStore());
        if (store != nullptr) {
            store->changePass(s(pass));
        }
        m_pass = pass;
    }
}

CfvView *CfvDocument::GetView() const
{
    auto *view = this->GetFirstView();
    return view != nullptr ? static_cast<CfvView *>(view) : nullptr;
}
