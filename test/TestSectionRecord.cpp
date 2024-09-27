#include "doctest/doctest.h"

#include <sstream>

#include "SectionRecord.h"

TEST_CASE("write_read")
{
    SectionRecord cr;
    cr.name = "test_file_name";
    cr.offset = 100;
    cr.size = 1234567;
    memcpy(cr.key, "1234ABCD5678EFGH", CRYPTO_KEY_LEN);
    std::stringstream str;
    cr.write(str);
    // The byte wroten in is not available immediately.
    // CHECK(str.rdbuf()->in_avail() == cr.len());
    SectionRecord cr1;
    CHECK(cr1.read(str));
    CHECK(cr1.name == cr.name);
    CHECK(cr1.offset == cr.offset);
    CHECK(cr1.size == cr.size);
    CHECK(memcmp(cr1.key, cr.key, CRYPTO_KEY_LEN) == 0);
}

TEST_CASE("read_fail")
{
    SectionRecord cr;
    std::stringstream str;
    str << "abcd";
    CHECK(!cr.read(str));
}
