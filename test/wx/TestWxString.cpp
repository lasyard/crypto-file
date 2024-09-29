#include "doctest/doctest.h"

#include <wx/string.h>

TEST_CASE("utf8")
{
    SUBCASE("唵嘛呢叭咪吽")
    {
        std::string s("唵嘛呢叭咪吽");
        auto wxs = wxString::FromUTF8(s);
        CHECK(!wxs.IsEmpty());
        CHECK(wxs.utf8_string() == s);
    }
    SUBCASE("幸福")
    {
        std::string s("幸福");
        auto wxs = wxString::FromUTF8(s);
        CHECK(!wxs.IsEmpty());
        CHECK(wxs.utf8_string() == s);
    }
}
