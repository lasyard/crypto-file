#ifndef _CRYPTO_FILE_DIRECTORY_STORE_H_
#define _CRYPTO_FILE_DIRECTORY_STORE_H_

#include <filesystem>

#include "Store.h"

class DirectoryStore : public Store
{
public:
    DirectoryStore(const std::string &dirName);
    virtual ~DirectoryStore();

    void create() override;
    void open() override;
    void clear() override;

    void readSection(const std::string &name, std::ostream &content) override;
    void readSection(const std::string &name, std::string &content) override;
    void writeSection(const std::string &name, std::istream &content) override;
    void writeSection(const std::string &name, const std::string &content) override;
    void deleteSection(const std::string &name) override;

    void getSectionNames(std::vector<std::string> &names) const override;

    bool contains(const std::string &name) const override;
    bool operator==(const Store &obj) const override;

private:
    std::filesystem::path m_dirName;

    static std::streamsize streamCopy(std::istream &in, std::ostream &out);
};

#endif /* _CRYPTO_FILE_DIRECTORY_STORE_H_ */
