#ifndef _CRYPTO_FILE_GUI_CFV_MAIN_FRAME_H_
#define _CRYPTO_FILE_GUI_CFV_MAIN_FRAME_H_

#include <wx/docview.h>

#include "Common.h"

class CfvPanel;

class CfvMainFrame : public wxDocParentFrame
{
    DECLARE_DYNAMIC_CLASS(CfvMainFrame)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    CfvMainFrame();
    CfvMainFrame(
        wxDocManager *manager,
        wxFrame *frame,
        wxWindowID id,
        const wxString &title,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long type = wxDEFAULT_FRAME_STYLE,
        const wxString &name = wxFrameNameStr // `wxASCII_STR` is not defined on Linux.
    );
    virtual ~CfvMainFrame();

    CfvPanel *GetPanel() const
    {
        return m_panel;
    }

    void OnClose(wxCloseEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnLicense(wxCommandEvent &event);
    void OnWxInfo(wxCommandEvent &event);

    void OnUpdateMenu(wxUpdateUIEvent &event);
    void OnUpdateCheckMenu(wxUpdateUIEvent &event);

private:
    static const wxString CFG_FILE_HISTORY;
    static const wxString LICENSE_FILE_NAME;

    CfvPanel *m_panel;
};

#endif /* _CRYPTO_FILE_GUI_CFV_MAIN_FRAME_H_ */
