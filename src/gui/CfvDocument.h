#ifndef _CRYPTO_FILE_GUI_CFV_DOCUMENT_H_
#define _CRYPTO_FILE_GUI_CFV_DOCUMENT_H_

#include <wx/docview.h>

#include "Common.h"

class CfvView;
class Cache;

class CfvDocument : public wxDocument
{
    DECLARE_DYNAMIC_CLASS(CfvDocument)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    CfvDocument();
    virtual ~CfvDocument();

    bool OnNewDocument() override;
    bool OnCloseDocument() override;
    bool DeleteContents() override;
    bool DoOpenDocument(const wxString &fileName) override;
    bool DoSaveDocument(const wxString &fileName) override;

    void Modify(bool modified) override;

    const std::string &GetSection(const std::string &name) const;
    void SaveSection(const std::string &name, const std::string &content);
    void DeleteSection(const std::string &name);

    void ForEachSection(std::function<bool(const std::string &)> callback) const;

    void OnChange(wxCommandEvent &event);

    void OnUpdateChangePass(wxUpdateUIEvent &event);
    void OnChangePass(wxCommandEvent &event);

private:
    static const char *const IV;

    Cache *m_doc;
    wxString m_pass;

    CfvView *GetView() const;
};

#endif /* _CRYPTO_FILE_GUI_CFV_DOCUMENT_H_ */
