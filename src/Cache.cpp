#include "Cache.h"
#include "Exeptions.h"
#include "Store.h"

Cache::~Cache()
{
    if (m_store != nullptr) {
        delete m_store;
    }
}

const std::string &Cache::get(const std::string &name)
{
    try {
        auto &section = m_cache.at(name);
        if (section.readDirty && m_store != nullptr) {
            m_store->readSection(name, section.content);
        }
        section.readDirty = false;
        return section.content;
    } catch ([[maybe_unused]] std::out_of_range &e) {
        throw SectionNotFound(name);
    }
}

void Cache::put(const std::string &name, const std::string &content)
{
    auto &section = m_cache[name];
    section.content = content;
    section.writeDirty = true;
    section.readDirty = false;
}

void Cache::remove(const std::string &name)
{
    if (m_store != nullptr) {
        m_store->deleteSection(name);
    }
    m_cache.erase(name);
}

void Cache::attach(Store *store)
{
    if (m_store != nullptr) {
        delete m_store;
    }
    store->open();
    m_store = store;
    std::vector<std::string> names;
    m_store->getSectionNames(names);
    for (auto &name : names) {
        m_cache[name] = Section();
    }
}

void Cache::save()
{
    if (m_store == nullptr) {
        throw NoFileSpecified();
    }
    for (auto &[name, section] : m_cache) {
        if (section.writeDirty) {
            m_store->writeSection(name, section.content);
            section.writeDirty = false;
        }
    }
    m_store->flush();
}

void Cache::saveAs(Store *store)
{
    if (m_store != nullptr) {
        if (*m_store != *store) {
            for (auto &[name, section] : m_cache) {
                if (section.readDirty) {
                    m_store->readSection(name, section.content);
                }
            }
            delete m_store;
            store->create();
            m_store = store;
        }
    } else {
        store->create();
        m_store = store;
    }
    save();
}

void Cache::forEach(std::function<bool(const std::string &)> callback) const
{
    for (auto const &[name, section] : m_cache) {
        if (!callback(name)) {
            break;
        }
    }
}
