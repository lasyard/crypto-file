#include "doctest/doctest.h"

#include <filesystem>
#include <vector>

#include "Cache.h"
#include "FileStore.h"

static const char *fileName = "section-file-test.dat";

TEST_CASE("simple_save_load")
{
    auto file = new Cache();
    std::string str("This is a test.");
    file->put("a1", str);
    file->put("b2", "second");
    file->put("c3", "third");
    file->saveAs(new FileStore(fileName, "123", "ABC"));
    delete file;
    SUBCASE("load")
    {
        file = new Cache();
        file->attach(new FileStore(fileName, "123", "ABC"));
        std::string str1 = file->get("a1");
        CHECK(str1 == str);
        delete file;
    }
    SUBCASE("iterator")
    {
        file = new Cache();
        file->attach(new FileStore(fileName, "123", "ABC"));
        std::vector<std::string> names;
        file->forEach([&names](const std::string &name) -> bool {
            names.push_back(name);
            return true;
        });
        CHECK(names.size() == 3);
        CHECK(std::find(names.begin(), names.end(), "a1") != names.end());
        CHECK(std::find(names.begin(), names.end(), "b2") != names.end());
        CHECK(std::find(names.begin(), names.end(), "c3") != names.end());
        delete file;
    }
    std::filesystem::remove(fileName);
}
