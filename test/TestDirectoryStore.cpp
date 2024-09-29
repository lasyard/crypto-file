#include "doctest/doctest.h"

#include "DirectoryStore.h"

static const char *dirName = "directory-test";

TEST_CASE("contains")
{
    auto file = new DirectoryStore(".");
    file->open();
    CHECK(file->contains("TestDirectoryStore"));
    delete file;
}

TEST_CASE("write_read_delete")
{
    const char *sectionName = "a1b2c3";
    auto file = new DirectoryStore(dirName);
    file->create();
    std::string str("This is a test.");
    file->writeSection(sectionName, str);
    delete file;
    file = new DirectoryStore(dirName);
    file->open();
    CHECK(file->contains(sectionName));
    std::string str1;
    file->readSection(sectionName, str1);
    CHECK(str1 == str);
    file->deleteSection(sectionName);
    CHECK(!file->contains(sectionName));
    delete file;
    std::filesystem::remove_all(dirName);
}

TEST_CASE("write_with_subdir")
{
    const char *sectionName = "a1/b2/c3";
    auto file = new DirectoryStore(".");
    file->open();
    std::string str("This is a test.");
    file->writeSection(sectionName, str);
    delete file;
    file = new DirectoryStore(".");
    file->open();
    CHECK(file->contains(sectionName));
    std::string str1;
    file->readSection(sectionName, str1);
    CHECK(str1 == str);
    file->deleteSection(sectionName);
    CHECK(!file->contains(sectionName));
    delete file;
}

TEST_CASE("operator==")
{
    auto file = new DirectoryStore("abc");
    auto file1 = new DirectoryStore("abc");
    CHECK(*file == *file1);
}
