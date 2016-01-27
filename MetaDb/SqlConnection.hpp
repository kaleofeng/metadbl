#ifndef _METADB_SQLCONNECTION_HPP_
#define _METADB_SQLCONNECTION_HPP_

#include "MetaDb/CoreInclude.hpp"

#include <string>

#include "MetaDb/SqlDefine.hpp"

DECL_NAMESPACE_METADB_BEGIN

class SqlConnection {

public:
    struct Param {
        Param() = default;

        Param(const char* host
            , int port
            , const char* username
            , const char* password
            , const char* dbName)
            : m_host(host)
            , m_port(port)
            , m_username(username)
            , m_password(password)
            , m_dbName(dbName) {}

        std::string m_host;
        int m_port = 3306;
        std::string m_username;
        std::string m_password;
        std::string m_dbName;
    };


private:
    std::string m_defaultCharset = { "utf8" };
    int m_timeoutSeconds = 10;
    bool m_autoReconnect = true;

    int m_errorCode = 0;
    std::string m_errorMsg;
    char m_lastError[256] = { 0 };

    MYSQL* m_mysql = nullptr;
    Param m_param;

    int64_t m_connectTime = 0;

public:
    SqlConnection();

    ~SqlConnection();

    bool Connect(const Param& param);

    void Close();

    bool SelectDatabase(const char* dbName);

    bool CheckAlive() const;

    void AcquireErrorInfo();

    void SetErrorInfo(int errorCode, const char* errorMsg);

    const char* GetDefaultCharset() const;

    void SetDefaultCharset(const char* value);

    int GetTimeoutSeconds() const;

    void SetTimeoutSeconds(int value);

    bool IsAutoReconnect() const;

    void SetAutoReconnect(bool value);

    int GetErrorCode() const;

    void SetErrorCode(int value);

    const char* GetErrorMsg() const;

    void SetErrorMsg(const char* value);

    const char* GetLastError() const;

    template<typename... Args>
    void SetLastError(const char* format, Args... args);

    MYSQL* GetMysql();
};

inline const char* SqlConnection::GetDefaultCharset() const {
    return m_defaultCharset.c_str();
}

inline void SqlConnection::SetDefaultCharset(const char* value) {
    m_defaultCharset = value;
}

inline int SqlConnection::GetTimeoutSeconds() const {
    return m_timeoutSeconds;
}

inline void SqlConnection::SetTimeoutSeconds(int value) {
    m_timeoutSeconds = value;
}

inline bool SqlConnection::IsAutoReconnect() const {
    return m_autoReconnect;
}

inline void SqlConnection::SetAutoReconnect(bool value) {
    m_autoReconnect = value;
}

inline int SqlConnection::GetErrorCode() const {
    return m_errorCode;
}

inline void SqlConnection::SetErrorCode(int value) {
    m_errorCode = value;
}

inline const char* SqlConnection::GetErrorMsg() const {
    return m_errorMsg.c_str();
}

inline void SqlConnection::SetErrorMsg(const char* value) {
    m_errorMsg = value;
}

inline const char* SqlConnection::GetLastError() const {
    return m_lastError;
}

template<typename... Args>
inline void SqlConnection::SetLastError(const char* format, Args... args) {
    snprintf(m_lastError, sizeof(m_lastError), format, args...);
}

inline MYSQL* SqlConnection::GetMysql() {
    return m_mysql;
}

DECL_NAMESPACE_METADB_END

#endif // _METADB_SQLCONNECTION_HPP_
