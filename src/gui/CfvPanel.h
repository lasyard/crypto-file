#ifndef _GUI_CFV_PANEL_H_
#define _GUI_CFV_PANEL_H_

#include <wx/bookctrl.h>
#include <wx/panel.h>
#include <wx/string.h>
#include <wx/xrc/xmlres.h>

#include "Common.h"

class wxTreebook;

class CfvDocument;

class CfvPanel : public wxPanel
{
    DECLARE_DYNAMIC_CLASS(CfvPanel)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    CfvPanel(wxWindow *parent = nullptr);
    virtual ~CfvPanel();

    void OnUpdate();
    void SaveContents();

    void OnUpdateInsert(wxUpdateUIEvent &event);
    void OnInsert(wxCommandEvent &event);
    void OnUpdateDelete(wxUpdateUIEvent &event);
    void OnDelete(wxCommandEvent &event);

    void SetDocument(CfvDocument *doc)
    {
        m_doc = doc;
    }

    void ClearContents()
    {
        DeleteAllPages();
    }

private:
    wxTreebook *m_book;

    CfvDocument *m_doc;

    void AddPage(const wxString &name, const wxString &content, bool dirty = false);
    void InsertPage(int parent, wxWindow *page, const wxString &text, bool bSelect = false, int imageId = wxNOT_FOUND);
    void DeleteAllPages();

    void Unbind(int sel);

    bool IsLeaf(int sel) const;

    std::string GetSectionName(int sel) const;
};

#endif /* _GUI_CFV_PANEL_H_ */
