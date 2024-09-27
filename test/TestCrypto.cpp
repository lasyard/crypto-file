#include "doctest/doctest.h"

#include <sstream>

#include "Crypto.h"

TEST_CASE("encrypt_decrypt")
{
    const byte key[]{"12345678abcdefgh"};
    const byte iv[]{"abcdefgh12345678ABCDEFGH87654321"};
    std::string str = "aBCDefghI";
    std::string output;
    encrypt(str, output, key, iv);
    std::string origin;
    decrypt((const byte *)output.c_str(), output.length(), origin, key, iv);
    CHECK(str == origin);
}

TEST_CASE("compress_decompress")
{
    const char *str = "123abcABCD";
    std::stringstream in;
    in << str;
    std::string output;
    compress(in, output);
    std::stringstream out;
    decompress(output, out);
    std::string origin;
    out >> origin;
    CHECK(str == origin);
}

TEST_CASE("compress_decompress_string")
{
    std::string in("123abcABCD");
    std::string output;
    compress(in, output);
    std::string out;
    decompress(output, out);
    CHECK(in == out);
}
