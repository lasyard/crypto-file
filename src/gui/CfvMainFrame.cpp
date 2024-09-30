#include <wx/aboutdlg.h>
#include <wx/config.h>
#include <wx/notebook.h>
#include <wx/stdpaths.h>
#include <wx/utils.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "CfvMainFrame.h"

#include "CfvApp.h"
#include "CfvPanel.h"
#include "Defs.h"
#include "Utils.h"

IMPLEMENT_DYNAMIC_CLASS(CfvMainFrame, wxDocParentFrame)
IMPLEMENT_TM(CfvMainFrame)

// Event propagating order: doc -> view -> frame.
BEGIN_EVENT_TABLE(CfvMainFrame, wxDocParentFrame)
EVT_CLOSE(CfvMainFrame::OnClose)
EVT_UPDATE_UI(ID_CHANGE_PASS, CfvMainFrame::OnUpdateMenu)
EVT_UPDATE_UI(ID_INSERT, CfvMainFrame::OnUpdateMenu)
EVT_UPDATE_UI(wxID_DELETE, CfvMainFrame::OnUpdateMenu)
// Don't know why this ID cannot be wxID_ABOUT.
EVT_MENU(ID_ABOUT, CfvMainFrame::OnAbout)
EVT_MENU(ID_LICENSE, CfvMainFrame::OnLicense)
EVT_MENU(ID_WX_INFO, CfvMainFrame::OnWxInfo)
END_EVENT_TABLE()

const wxString CfvMainFrame::CFG_FILE_HISTORY = "FileHistory";
const wxString CfvMainFrame::LICENSE_FILE_NAME = "LICENSE";

CfvMainFrame::CfvMainFrame() : wxDocParentFrame()
{
    wxLog::AddTraceMask(TM);
}

CfvMainFrame::CfvMainFrame(
    wxDocManager *manager,
    wxFrame *parent,
    wxWindowID id,
    const wxString &title,
    const wxPoint &pos,
    const wxSize &size,
    long type,
    const wxString &name
)
    : wxDocParentFrame(manager, parent, id, title, pos, size, type, name)
{
    wxLog::AddTraceMask(TM);
    auto *res = wxXmlResource::Get();
    auto *menuBar = res->LoadMenuBar("menuBar");
    SetMenuBar(menuBar);
    auto *statusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP);
    statusBar->SetFieldsCount();
    SetStatusBar(statusBar);
    auto *sizer = new wxBoxSizer(wxVERTICAL);
    m_panel = new CfvPanel(this);
    sizer->Add(m_panel, wxSizerFlags().Expand().Border(wxALL, 0).Proportion(1));
    SetSizer(sizer);
    m_panel->Show(false);
    SetSize(1080, 720);
    // The config file is in `~/Library/Preferences` on macOS.
    wxConfig config(_(APP_NAME));
    config.SetPath(CFG_FILE_HISTORY);
    m_docManager->FileHistoryLoad(config);
    auto *fileMenu = GetMenuBar()->GetMenu(GetMenuBar()->FindMenu(_("File")));
    m_docManager->FileHistoryUseMenu(fileMenu);
    m_docManager->FileHistoryAddFilesToMenu();
}

CfvMainFrame::~CfvMainFrame()
{
}

void CfvMainFrame::OnClose([[maybe_unused]] wxCloseEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    wxConfig config(_(APP_NAME));
    config.SetPath(CFG_FILE_HISTORY);
    m_docManager->FileHistorySave(config);
    Destroy();
    wxExit();
}

void CfvMainFrame::OnAbout([[maybe_unused]] wxCommandEvent &event)
{
    const wxString &appName = wxGetApp().GetAppDisplayName();
    wxAboutDialogInfo info;
    info.SetName(appName);
    info.SetVersion(APP_VERSION);
    wxString date = __DATE__;
    info.SetCopyright("(C) " + date.Right(4) + " " AUTHOR " " EMAIL);
    info.AddDeveloper(AUTHOR " " EMAIL);
    wxAboutBox(info);
}

void CfvMainFrame::OnLicense([[maybe_unused]] wxCommandEvent &event)
{
    wxString text;
    wxString resDir = wxStandardPaths::Get().GetResourcesDir();
    Utils::ReadAllText(text, wxFileName(resDir, LICENSE_FILE_NAME).GetFullPath());
#if wxUSE_UNICODE
    const wxString copyrightSign = c("\xc2\xa9");
    text.Replace("(c)", copyrightSign);
    text.Replace("(C)", copyrightSign);
#endif
    Utils::ShowTextBox(_("License"), text);
}

void CfvMainFrame::OnWxInfo([[maybe_unused]] wxCommandEvent &event)
{
    wxInfoMessageBox(this);
}

void CfvMainFrame::OnUpdateMenu(wxUpdateUIEvent &event)
{
    // There is no doc/view if the event propagated here.
    event.Enable(false);
}

void CfvMainFrame::OnUpdateCheckMenu(wxUpdateUIEvent &event)
{
    event.Enable(false);
    event.Check(false);
}
