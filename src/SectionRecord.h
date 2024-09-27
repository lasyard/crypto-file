#ifndef _SECTION_RECORD_H_
#define _SECTION_RECORD_H_

#include <cstring>
#include <ostream>

#include "Crypto.h"

class SectionRecord
{
public:
    static const int OFFSET_LEN = 8;
    static const int SIZE_LEN = 8;
    static const int NAME_LEN_LEN = 2;

    std::string name;
    size_t offset;
    size_t size;
    unsigned char key[CRYPTO_KEY_LEN];

    SectionRecord() : name(), offset(0), size(0)
    {
        memset(key, 0, CRYPTO_KEY_LEN);
    }

    SectionRecord(const SectionRecord &obj)
    {
        copy(obj);
    }

    const SectionRecord &operator=(const SectionRecord &obj)
    {
        copy(obj);
        return *this;
    }

    void write(std::ostream &os) const;
    bool read(std::istream &is);

    size_t len() const
    {
        return OFFSET_LEN + SIZE_LEN + CRYPTO_KEY_LEN + NAME_LEN_LEN + name.length();
    }

private:
    void copy(const SectionRecord &obj)
    {
        name = obj.name;
        offset = obj.offset;
        size = obj.size;
        memcpy(key, obj.key, CRYPTO_KEY_LEN);
    }

    friend std::ostream &operator<<(std::ostream &os, const SectionRecord &obj);
};

#endif /* _SECTION_RECORD_H_ */
