#ifndef _CRYPTO_FILE_EXEPTIONS_H_
#define _CRYPTO_FILE_EXEPTIONS_H_

#include <stdexcept>
#include <string>

class DirCreate : public std::runtime_error
{
public:
    explicit DirCreate(const std::string &dirName) : std::runtime_error("Cannot create dir \"" + dirName + "\".")
    {
    }
};

class FileOpen : public std::runtime_error
{
public:
    explicit FileOpen(const std::string &fileName) : std::runtime_error("Cannot open/create file \"" + fileName + "\".")
    {
    }
};

class FileCorrupt : public std::runtime_error
{
public:
    explicit FileCorrupt(const std::string &what) : std::runtime_error(what)
    {
    }
};

class BadPassword : public std::runtime_error
{
public:
    explicit BadPassword() : std::runtime_error("Bad password.")
    {
    }
};

class SectionNotFound : public std::runtime_error
{
public:
    explicit SectionNotFound(const std::string &name)
        : std::runtime_error("Section \"" + name + "\" is not found.")
        , m_name(name)
    {
    }

    const std::string &getName() const
    {
        return m_name;
    }

private:
    std::string m_name;
};

class NoFileSpecified : public std::runtime_error
{
public:
    explicit NoFileSpecified() : std::runtime_error("File to save is not specified.")
    {
    }
};

#endif /* _CRYPTO_FILE_EXEPTIONS_H_ */
