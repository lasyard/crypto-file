#include <wx/listbook.h>

#include "CfvView.h"

#include "CfvApp.h"
#include "CfvDocument.h"
#include "CfvMainFrame.h"
#include "CfvPanel.h"
#include "Defs.h"
#include "Utils.h"

IMPLEMENT_DYNAMIC_CLASS(CfvView, wxView)
IMPLEMENT_TM(CfvView)

BEGIN_EVENT_TABLE(CfvView, wxView)
EVT_UPDATE_UI(ID_INSERT, CfvView::OnUpdateMenu)
EVT_MENU(ID_INSERT, CfvView::OnMenu)
EVT_UPDATE_UI(wxID_DELETE, CfvView::OnUpdateMenu)
EVT_MENU(wxID_DELETE, CfvView::OnMenu)
END_EVENT_TABLE()

CfvView::CfvView() : wxView()
{
    wxLog::AddTraceMask(TM);
}

CfvView::~CfvView()
{
}

bool CfvView::OnCreate([[maybe_unused]] wxDocument *doc, [[maybe_unused]] long flags)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    auto *frame = static_cast<CfvMainFrame *>(wxGetApp().GetTopWindow());
    SetFrame(frame);
    m_panel = frame->GetPanel();
    m_panel->SetDocument(static_cast<CfvDocument *>(doc));
    m_panel->Show();
    frame->Layout();
    Activate(true);
    return true;
}

bool CfvView::OnClose(bool deleteWindow)
{
    Activate(false);
    auto *frame = static_cast<CfvMainFrame *>(GetFrame());
    frame->SetTitle("");
    m_panel->ClearContents();
    m_panel->Show(false);
    return wxView::OnClose(deleteWindow);
}

void CfvView::OnUpdate([[maybe_unused]] wxView *sender, [[maybe_unused]] wxObject *hint)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    m_panel->OnUpdate();
}

void CfvView::OnDraw([[maybe_unused]] wxDC *dc)
{
    // Do nothing.
}

void CfvView::OnClosingDocument()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
}

void CfvView::OnUpdateMenu(wxUpdateUIEvent &event)
{
    Utils::DelegateEvent(m_panel, event);
}

void CfvView::OnMenu(wxCommandEvent &event)
{
    Utils::DelegateEvent(m_panel, event);
}

void CfvView::SaveContents()
{
    m_panel->SaveContents();
}
