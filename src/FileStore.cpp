#include <sstream>

#include "Exeptions.h"
#include "FileStore.h"

const char *const FileStore::CATALOG_NAME = "__catalog__";

FileStore::FileStore(const std::string &fileName, const std::string &pass, const std::string &iv)
    : CryptoStore()
    , m_fileName(fileName)
    , m_file()
    , m_catalog()
{
    srand(static_cast<unsigned int>(time(NULL)));
    setIV(m_iv, iv);
    getKey(m_key, pass);
}

FileStore::~FileStore()
{
    if (m_file.is_open()) {
        flush();
        m_file.close();
    }
}

void FileStore::create()
{
    m_file.open(m_fileName, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
    if (!m_file.is_open()) {
        throw FileOpen(m_fileName);
    }
    clear();
}

void FileStore::open()
{
    m_file.open(m_fileName, std::ios::binary | std::ios::in | std::ios::out);
    if (!m_file.is_open()) {
        throw FileOpen(m_fileName);
    }
    loadCatalog();
}

void FileStore::clear()
{
    // Clear all sections.
    m_catalog.clear();
    // Construct new catalog.
    SectionRecord cr;
    // Must not be empty, or it is treated as a new section.
    cr.name = CATALOG_NAME;
    memcpy(cr.key, m_key, CRYPTO_KEY_LEN);
    m_catalog[CATALOG_NAME] = cr;
}

void FileStore::flush()
{
    saveCatalog();
    m_file.flush();
}

void FileStore::deleteSection(const std::string &name)
{
    m_catalog.erase(name);
}

void FileStore::getSectionNames(std::vector<std::string> &names) const
{
    for (auto const &[name, c] : m_catalog) {
        if (name != CATALOG_NAME) {
            names.push_back(name);
        }
    }
}

bool FileStore::contains(const std::string &name) const
{
    return m_catalog.find(name) != m_catalog.end();
}

bool FileStore::operator==(const Store &obj) const
{
    try {
        const FileStore &file = dynamic_cast<const FileStore &>(obj);
        return file.m_fileName == m_fileName;
    } catch ([[maybe_unused]] std::bad_cast &e) {
        return false;
    }
}

void FileStore::changePass(const std::string &pass)
{
    getKey(m_key, pass);
}

void FileStore::decryptSection(const std::string &name, std::string &content)
{
    auto &cr = m_catalog.at(name);
    size_t size = cr.size;
    byte *buf = new byte[size];
    m_file.seekg(cr.offset);
    m_file.read((char *)buf, size);
    if ((size_t)m_file.gcount() < size) {
        throw FileCorrupt("Section is too short.");
    }
    try {
        decrypt(buf, size, content, cr.key, m_iv);
    } catch (CryptoPP::HashVerificationFilter::HashVerificationFailed &) {
        delete[] buf;
        throw BadPassword();
    }
    delete[] buf;
}

void FileStore::encryptSection(const std::string &name, const std::string &content)
{
    auto &cr = m_catalog[name];
    if (cr.name.empty()) { // New created section.
        cr.name = name;
        newKey(cr.key);
    }
    std::string output;
    encrypt(content, output, cr.key, m_iv);
    auto size = output.length();
    auto offset = findSlot(size);
    cr.offset = offset;
    cr.size = size;
    m_file.seekp(offset);
    m_file.write(output.c_str(), size);
}

void FileStore::loadCatalog()
{
    m_catalog.clear();
    m_file.seekg(0);
    // Read catalog info.
    SectionRecord cr;
    if (!cr.read(m_file)) {
        throw FileCorrupt("CATALOG is too short.");
    }
    memcpy(cr.key, m_key, CRYPTO_KEY_LEN);
    m_catalog[CATALOG_NAME] = cr;
    // Read catalog.
    std::stringstream content;
    readSection(CATALOG_NAME, content);
    // Read info of sections.
    while (cr.read(content)) {
        m_catalog[cr.name] = cr;
    }
}

void FileStore::saveCatalog()
{
    std::stringstream content;
    for (auto &[name, cr] : m_catalog) {
        if (name == CATALOG_NAME) {
            memcpy(cr.key, m_key, CRYPTO_KEY_LEN);
            continue;
        }
        cr.write(content);
    }
    writeSection(CATALOG_NAME, content);
    SectionRecord cr = m_catalog[CATALOG_NAME];
    // Don't write pass into file.
    memcpy(cr.key, "NO PASSWORD HERE", CRYPTO_KEY_LEN);
    m_file.seekp(0);
    cr.write(m_file);
}

size_t FileStore::findSlot(size_t size) const
{
    std::vector<const SectionRecord *> vec;
    // Must reference to get the real address in `m_catalog`.
    for (auto const &[name, cr] : m_catalog) {
        if (name != CATALOG_NAME) {
            vec.push_back(&cr);
        }
    }
    std::sort(vec.begin(), vec.end(), [](const SectionRecord *a, const SectionRecord *b) {
        return a->offset < b->offset;
    });
    // Here must be `at` for this method is `const`.
    auto offset = m_catalog.at(CATALOG_NAME).len();
    for (auto i : vec) {
        if (i->size == 0) { // Ignore temprary records.
            continue;
        }
        if (i->offset >= offset + size) {
            break;
        }
        offset = i->offset + i->size;
    }
    return offset;
}

std::ostream &operator<<(std::ostream &os, const FileStore &obj)
{
    os << "Catalog of file \"" << obj.m_fileName << "\":" << std::endl;
    for (auto const &[name, cr] : obj.m_catalog) {
        os << cr << std::endl;
    }
    return os;
}
