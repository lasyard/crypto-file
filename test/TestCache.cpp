#include "doctest/doctest.h"

#include <filesystem>
#include <vector>

#include "Cache.h"
#include "FileStore.h"
#include "Sqlite3Store.h"

static const char *fileName = "cache-test.dat";

static void doCreate(const std::string &str, Store *store)
{
    auto *file = new Cache();
    file->put("a1", str);
    file->put("b2", "second");
    file->put("c3", "third");
    file->saveAs(store);
    delete file;
}

static void doLoadTest(const std::string &str, Store *store)
{
    auto *file = new Cache();
    file->attach(store);
    std::string str1 = file->get("a1");
    CHECK(str1 == str);
    delete file;
}

static void doIteratorTest(Store *store)
{
    auto *file = new Cache();
    file->attach(store);
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

TEST_CASE("file_store_save_load")
{
    std::string str("This is a test.");
    doCreate(str, new FileStore(fileName, "123", "ABC"));
    SUBCASE("load")
    {
        doLoadTest(str, new FileStore(fileName, "123", "ABC"));
    }
    SUBCASE("iterator")
    {
        doIteratorTest(new FileStore(fileName, "123", "ABC"));
    }
    std::filesystem::remove(fileName);
}

TEST_CASE("sqlite3_store_save_load")
{
    std::string str("This is a test.");
    doCreate(str, new Sqlite3Store(fileName, "123", "ABC"));
    SUBCASE("load")
    {
        doLoadTest(str, new Sqlite3Store(fileName, "123", "ABC"));
    }
    SUBCASE("iterator")
    {
        doIteratorTest(new Sqlite3Store(fileName, "123", "ABC"));
    }
    std::filesystem::remove(fileName);
}
