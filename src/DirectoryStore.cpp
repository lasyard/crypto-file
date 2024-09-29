#include <fstream>
#include <sstream>

#include "DirectoryStore.h"
#include "Exeptions.h"

constexpr size_t BUF_LEN = 1024;

namespace fs = std::filesystem;

DirectoryStore::DirectoryStore(const std::string &dirName) : Store(), m_dirName(dirName)
{
}

DirectoryStore::~DirectoryStore()
{
}

void DirectoryStore::create()
{
    if (!fs::is_directory(m_dirName)) {
        fs::create_directories(m_dirName);
    } else {
        throw DirCreate(m_dirName.string());
    }
}

void DirectoryStore::open()
{
}

void DirectoryStore::clear()
{
    for (auto const &entry : fs::directory_iterator(m_dirName)) {
        fs::remove_all(entry);
    }
}

void DirectoryStore::readSection(const std::string &name, std::ostream &content)
{
    std::ifstream file(m_dirName / name, std::ios::binary | std::ios::in);
    if (!file.is_open()) {
        throw SectionNotFound(name);
    }
    streamCopy(file, content);
    file.close();
}

void DirectoryStore::readSection(const std::string &name, std::string &content)
{
    std::stringstream str;
    readSection(name, str);
    content = str.str();
}

void DirectoryStore::writeSection(const std::string &name, std::istream &content)
{
    auto path = m_dirName / name;
    if (!fs::is_directory(path.parent_path())) {
        fs::create_directories(path.parent_path());
    }
    std::ofstream file(m_dirName / name, std::ios::binary | std::ios::out | std::ios::trunc);
    streamCopy(content, file);
    file.close();
}

void DirectoryStore::writeSection(const std::string &name, const std::string &content)
{
    std::stringstream str;
    str << content;
    writeSection(name, str);
}

void DirectoryStore::deleteSection(const std::string &name)
{
    auto path = m_dirName / name;
    do {
        fs::remove(path);
        path = path.parent_path();
    } while (path != m_dirName && fs::is_empty(path));
}

void DirectoryStore::getSectionNames(std::vector<std::string> &names) const
{
    for (auto const &entry : fs::recursive_directory_iterator(m_dirName)) {
        names.push_back(entry.path().string().substr(m_dirName.string().length()));
    }
}

bool DirectoryStore::contains(const std::string &name) const
{
    return is_regular_file(m_dirName / name);
}

bool DirectoryStore::operator==(const Store &obj) const
{
    try {
        const DirectoryStore &dir = dynamic_cast<const DirectoryStore &>(obj);
        return dir.m_dirName == m_dirName;
    } catch ([[maybe_unused]] std::bad_cast &e) {
        return false;
    }
}

std::streamsize DirectoryStore::streamCopy(std::istream &in, std::ostream &out)
{
    char *buf = new char[BUF_LEN];
    std::streamsize count = 0;
    while (!in.eof()) {
        in.read(buf, BUF_LEN);
        out.write(buf, in.gcount());
        count += in.gcount();
    }
    delete[] buf;
    return count;
}
