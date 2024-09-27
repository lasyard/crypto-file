#include <cryptopp/aes.h>
#include <cryptopp/files.h>
#include <cryptopp/gcm.h>
#include <cryptopp/sha.h>
#include <cryptopp/zlib.h>

#include "Crypto.h"

using namespace CryptoPP;

void digest(const std::string &input, std::string &output)
{
    SHA256 encoder;
    StringSource(input, true, new HashFilter(encoder, new StringSink(output)));
}

void encrypt(
    const std::string &input,
    std::string &output,
    const byte key[CRYPTO_KEY_LEN],
    const byte iv[CRYPTO_IV_LEN]
)
{
    GCM_Final<AES, GCM_2K_Tables, true> gcm;
    gcm.SetKeyWithIV(key, CRYPTO_KEY_LEN, iv);
    StringSource(input, true, new AuthenticatedEncryptionFilter(gcm, new StringSink(output)));
}

void encrypt(
    const byte *input,
    size_t size,
    std::string &output,
    const byte key[CRYPTO_KEY_LEN],
    const byte iv[CRYPTO_IV_LEN]
)
{
    GCM_Final<AES, GCM_2K_Tables, true> gcm;
    gcm.SetKeyWithIV(key, CRYPTO_KEY_LEN, iv);
    ArraySource(input, size, true, new AuthenticatedEncryptionFilter(gcm, new StringSink(output)));
}

void decrypt(
    const byte *buf,
    size_t length,
    std::string &output,
    const byte key[CRYPTO_KEY_LEN],
    const byte iv[CRYPTO_IV_LEN]
)
{
    GCM_Final<AES, GCM_2K_Tables, false> gcm;
    gcm.SetKeyWithIV(key, CRYPTO_KEY_LEN, iv);
    ArraySource(buf, length, true, new AuthenticatedDecryptionFilter(gcm, new StringSink(output)));
}

void decrypt(
    const byte *buf,
    size_t length,
    byte *output,
    size_t outputLength,
    const byte key[CRYPTO_KEY_LEN],
    const byte iv[CRYPTO_IV_LEN]
)
{
    GCM_Final<AES, GCM_2K_Tables, false> gcm;
    gcm.SetKeyWithIV(key, CRYPTO_KEY_LEN, iv);
    ArraySource(buf, length, true, new AuthenticatedDecryptionFilter(gcm, new ArraySink(output, outputLength)));
}

void compress(std::istream &input, std::string &output)
{
    FileSource(input, true, new ZlibCompressor(new StringSink(output)));
}

void decompress(const std::string &input, std::ostream &output)
{
    StringSource(input, true, new ZlibDecompressor(new FileSink(output)));
}

void compress(const std::string &input, std::string &output)
{
    StringSource(input, true, new ZlibCompressor(new StringSink(output)));
}

void decompress(const std::string &input, std::string &output)
{
    StringSource(input, true, new ZlibDecompressor(new StringSink(output)));
}

void getKey(byte key[CRYPTO_KEY_LEN], const std::string &pass)
{
    std::string result;
    digest(pass, result);
    result.copy((char *)key, CRYPTO_KEY_LEN);
}

void newKey(byte key[CRYPTO_KEY_LEN])
{
    std::string pass("0123456789ABCDEF");
    std::generate(pass.begin(), pass.end(), rand);
    getKey(key, pass);
}

void setIV(byte iv[], const std::string &str)
{
    size_t len = CRYPTO_IV_LEN;
    if (str.length() < len) {
        memset(iv, 0, len);
        len = str.length();
    }
    memcpy(iv, str.c_str(), len);
}
