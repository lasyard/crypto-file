#ifndef _CRYPTO_FILE_GUI_CFV_VIEW_H_
#define _CRYPTO_FILE_GUI_CFV_VIEW_H_

#include <wx/docview.h>

#include "Common.h"

class wxBookCtrlEvent;

class CfvDocument;
class CfvPanel;

class CfvView : public wxView
{
    DECLARE_DYNAMIC_CLASS(CfvView)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    CfvView();
    virtual ~CfvView();

    bool OnCreate(wxDocument *doc, long flags) override;
    bool OnClose(bool deleteWindow) override;

    void OnUpdate(wxView *sender, wxObject *hint) override;
    void OnDraw(wxDC *dc) override;
    void OnClosingDocument() override;

    void OnUpdateMenu(wxUpdateUIEvent &event);
    void OnMenu(wxCommandEvent &event);
    void OnUpdateRawMode(wxUpdateUIEvent &event);
    void OnRawMode(wxCommandEvent &event);

    void SaveContents();

private:
    CfvPanel *m_panel;
};

#endif /* _CRYPTO_FILE_GUI_CFV_VIEW_H_ */
