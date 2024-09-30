#include <wx/fs_arc.h>
#include <wx/stdpaths.h>
#include <wx/xrc/xmlres.h>

#include "CfvApp.h"

#include "CfvDocument.h"
#include "CfvMainFrame.h"
#include "CfvView.h"

IMPLEMENT_APP(CfvApp)
IMPLEMENT_TM(CfvApp)

bool CfvApp::OnInit()
{
    if (!wxApp::OnInit()) {
        return false;
    }
    wxLog::AddTraceMask(TM);
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    m_locale.Init();
    if (!m_locale.AddCatalog(APP_NAME) || !m_locale.AddCatalog("xrc")) {
        wxLogError("Could not load translation files for \"" + m_locale.GetCanonicalName() + "\" locale.");
    }
    m_locale.AddCatalog("wxstd");
    wxFileSystem::AddHandler(new wxArchiveFSHandler());
    wxXmlResource::Get()->InitAllHandlers();
    wxString resDir = wxStandardPaths::Get().GetResourcesDir();
    wxXmlResource::Get()->LoadFile(wxFileName(resDir, XRS_FILE));
    wxImage::AddHandler(new wxPNGHandler());
    SetVendorName("Lasy");
    SetAppName(_(APP_NAME));
    SetAppDisplayName(_(APP_NAME));
    m_docManager = new wxDocManager();
    (void)new wxDocTemplate(
        m_docManager,
        GetAppDisplayName(),
        "*.cfv",
        "",
        "cfv",
        GetAppDisplayName() + _(" Doc"),
        GetAppDisplayName() + _(" View"),
        CLASSINFO(CfvDocument),
        CLASSINFO(CfvView)
    );
    m_docManager->SetMaxDocsOpen(1);
    wxFrame *frame = new CfvMainFrame(m_docManager, nullptr, wxID_ANY, GetAppDisplayName());
    frame->Center(wxBOTH);
    frame->Show();
    SetTopWindow(frame);
    return true;
}

int CfvApp::OnExit()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    delete m_docManager;
    return wxApp::OnExit();
}
