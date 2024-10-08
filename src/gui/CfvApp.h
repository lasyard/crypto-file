#ifndef _CRYPTO_FILE_GUI_CFV_APP_H_
#define _CRYPTO_FILE_GUI_CFV_APP_H_

#include <wx/app.h>
#include <wx/docview.h>

#include "Common.h"

class CfvApp : public wxApp
{
public:
    DECLARE_TM()

    bool OnInit() override;
    int OnExit() override;

private:
    wxDocManager *m_docManager;
    // This must be long lived.
    wxLocale m_locale;
};

DECLARE_APP(CfvApp)

#endif /* _CRYPTO_FILE_GUI_CFV_APP_H_ */
