#ifndef _SQLITE3STORE_H_
#define _SQLITE3STORE_H_

#include "CryptoStore.h"

struct sqlite3;
struct sqlite3_stmt;
struct sqlite3_context;
struct sqlite3_value;

class Sqlite3Store : public CryptoStore
{
public:
    static const char *const CHECK_STUB;

    Sqlite3Store(const std::string &fileName, const std::string &pass, const std::string &iv);
    virtual ~Sqlite3Store();

    void create() override;
    void open() override;
    void clear() override;

    void deleteSection(const std::string &name) override;

    void getSectionNames(std::vector<std::string> &names) const override;

    bool contains(const std::string &name) const override;
    bool operator==(const Store &obj) const override;

    void changePass(const std::string &pass) override;

private:
    static const char *const GET_SQL;

    byte m_iv[CRYPTO_IV_LEN];
    byte m_key[CRYPTO_KEY_LEN];
    byte m_oldKey[CRYPTO_KEY_LEN];
    std::string m_fileName;
    sqlite3 *m_db;

    mutable sqlite3_stmt *m_getStmt;
    mutable sqlite3_stmt *m_putStmt;
    mutable sqlite3_stmt *m_delStmt;
    mutable sqlite3_stmt *m_passStmt;
    mutable sqlite3_stmt *m_enumStmt;

    void decryptSection(const std::string &name, std::string &content) override;
    void encryptSection(const std::string &name, const std::string &content) override;

    void init();
    void prepareSql(sqlite3_stmt *&stmt, const std::string &sql) const;
    void closeStmt(sqlite3_stmt *&stmt);
    void execSql(const char *sql);
    void registerFun();

    friend void recryptFun(sqlite3_context *ctx, int n, sqlite3_value **value);
};

#endif /* _SQLITE3STORE_H_ */
