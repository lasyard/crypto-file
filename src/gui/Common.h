#ifndef _CRYPTO_FILE_GUI_COMMON_H_
#define _CRYPTO_FILE_GUI_COMMON_H_

#include <wx/log.h>
#include <wx/string.h>

#define DECLARE_TM()      static const wxChar *const TM;
#define IMPLEMENT_TM(cls) const wxChar *const cls::TM = wxS(#cls);

#define t(x) x // helper to translate static text

/**
 * @brief  helper to convert std::string to wxString.
 *
 * @param s the std::string
 * @return wxString the converted
 */
inline wxString c(const std::string &s)
{
    auto str = wxString::FromUTF8(s);
    if (s.length() > 0 && str.IsEmpty()) {
        wxLogError("Illegal UTF-8 string encoutered.");
    }
    return str;
}

/**
 * @brief helper to convert wxString to std::string.
 *
 * @param str the wxString
 * @return const char* the converted
 */
inline std::string s(const wxString &str)
{
    return str.utf8_string();
}

#endif /* _CRYPTO_FILE_GUI_COMMON_H_ */
