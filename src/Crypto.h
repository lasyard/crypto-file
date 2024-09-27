#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#include <iostream>
#include <string>

#include <cryptopp/config.h>
#include <cryptopp/filters.h>

// Since crypto++6.0, `byte` is in `CryptoPP` namespace.
#if CRYPTOPP_MAJOR >= 6
typedef CryptoPP::byte byte;
#endif

constexpr auto CRYPTO_KEY_LEN = 16;
constexpr auto CRYPTO_IV_LEN = 32;

void digest(const std::string &input, std::string &output);

void encrypt(
    const std::string &input,
    std::string &output,
    const byte key[CRYPTO_KEY_LEN],
    const byte iv[CRYPTO_IV_LEN]
);
void encrypt(
    const byte *input,
    size_t size,
    std::string &output,
    const byte key[CRYPTO_KEY_LEN],
    const byte iv[CRYPTO_IV_LEN]
);
void decrypt(
    const byte *buf,
    size_t length,
    std::string &output,
    const byte key[CRYPTO_KEY_LEN],
    const byte iv[CRYPTO_IV_LEN]
);
void decrypt(
    const byte *buf,
    size_t length,
    byte *output,
    size_t outputLength,
    const byte key[CRYPTO_KEY_LEN],
    const byte iv[CRYPTO_IV_LEN]
);

void compress(std::istream &input, std::string &output);
void compress(const std::string &input, std::string &output);
void decompress(const std::string &input, std::ostream &output);
void decompress(const std::string &input, std::string &output);

void getKey(byte key[CRYPTO_KEY_LEN], const std::string &pass);
void newKey(byte key[CRYPTO_KEY_LEN]);

void setIV(byte iv[], const std::string &str);

#endif /* _CRYPTO_H_ */
