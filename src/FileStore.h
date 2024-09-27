#ifndef _FILE_STORE_H_
#define _FILE_STORE_H_

#include <fstream>

#include "CryptoStore.h"
#include "SectionRecord.h"

class FileStore : public CryptoStore
{
public:
    static const char *const CATALOG_NAME;

    FileStore(const std::string &fileName, const std::string &pass, const std::string &iv);
    virtual ~FileStore();

    void create() override;
    void open() override;
    void clear() override;
    void flush() override;

    void deleteSection(const std::string &name) override;

    void getSectionNames(std::vector<std::string> &names) const override;

    bool contains(const std::string &name) const override;
    bool operator==(const Store &obj) const override;

    void changePass(const std::string &pass) override;

private:
    byte m_iv[CRYPTO_IV_LEN];
    byte m_key[CRYPTO_KEY_LEN];
    std::string m_fileName;
    std::fstream m_file;
    std::map<std::string, SectionRecord> m_catalog;

    void decryptSection(const std::string &name, std::string &content) override;
    void encryptSection(const std::string &name, const std::string &content) override;

    void loadCatalog();
    void saveCatalog();

    size_t findSlot(size_t length) const;

    friend std::ostream &operator<<(std::ostream &os, const FileStore &obj);
};

#endif /* _FILE_STORE_H_ */
