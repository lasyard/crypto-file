#include "doctest/doctest.h"

#include <filesystem>
#include <sstream>

#include "FileStore.h"

static const char *fileName = "crypto-file-test.dat";

TEST_CASE("write_read_delete")
{
    const char *sectionName = "a1b2c3";
    auto file = new FileStore(fileName, "123", "ABC");
    file->create();
    std::string str("This is a test.");
    file->writeSection(sectionName, str);
    delete file;
    file = new FileStore(fileName, "123", "ABC");
    file->open();
    CHECK(file->contains(sectionName));
    std::string str1;
    file->readSection(sectionName, str1);
    CHECK(str1 == str);
    file->deleteSection(sectionName);
    CHECK(!file->contains(sectionName));
    delete file;
    std::filesystem::remove(fileName);
}

TEST_CASE("smoke")
{
    auto file = new FileStore(fileName, "123", "ABC");
    file->create();
    std::stringstream str;
    for (int i = 0; i < 100; i++) {
        str << "This is test string " << i;
        std::string name = "item" + std::to_string(i);
        file->writeSection(name, str);
        std::stringstream().swap(str);
    }
    delete file;
    file = new FileStore(fileName, "123", "ABC");
    file->open();
    std::string s;
    for (int i = 0; i < 100; i++) {
        std::string name = "item" + std::to_string(i);
        file->readSection(name, s);
        CHECK(s == "This is test string " + std::to_string(i));
        s.clear();
    }
    delete file;
    std::filesystem::remove(fileName);
}

TEST_CASE("change_pass")
{
    auto file = new FileStore(fileName, "123", "ABC");
    file->create();
    std::string str("This is a test.");
    file->writeSection("a1b2c3", str);
    delete file;
    file = new FileStore(fileName, "123", "ABC");
    file->open();
    file->changePass("DEF");
    delete file;
    file = new FileStore(fileName, "DEF", "ABC");
    file->open();
    CHECK(file->contains("a1b2c3"));
    std::string str1;
    file->readSection("a1b2c3", str1);
    delete file;
    std::filesystem::remove(fileName);
    CHECK(str1 == str);
}

TEST_CASE("operator==")
{
    auto file = new FileStore("abc", "123", "ABC");
    auto file1 = new FileStore("abc", "456", "DEF");
    CHECK(*file == *file1);
}
