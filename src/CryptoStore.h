#ifndef _CRYPTO_FILE_CRYPTO_STORE_H_
#define _CRYPTO_FILE_CRYPTO_STORE_H_

#include "Crypto.h"
#include "Store.h"

class CryptoStore : public Store
{
public:
    CryptoStore() : Store()
    {
    }

    virtual ~CryptoStore()
    {
    }

    void readSection(const std::string &name, std::ostream &content) override
    {
        std::string tmp;
        decryptSection(name, tmp);
        decompress(tmp, content);
    }

    void readSection(const std::string &name, std::string &content) override
    {
        std::string tmp;
        decryptSection(name, tmp);
        decompress(tmp, content);
    }

    void writeSection(const std::string &name, std::istream &content) override
    {
        std::string tmp;
        compress(content, tmp);
        encryptSection(name, tmp);
    }

    void writeSection(const std::string &name, const std::string &content) override
    {
        std::string tmp;
        compress(content, tmp);
        encryptSection(name, tmp);
    }

    virtual void changePass(const std::string &pass) = 0;

protected:
    virtual void decryptSection(const std::string &name, std::string &content) = 0;
    virtual void encryptSection(const std::string &name, const std::string &content) = 0;
};

#endif /* _CRYPTO_FILE_CRYPTO_STORE_H_ */
