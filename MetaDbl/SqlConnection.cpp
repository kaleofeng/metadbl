#include "MetaDbl/SqlConnection.hpp"

#include <Metazion/Share/Time/Time.hpp>

DECL_NAMESPACE_METADB_BEGIN

SqlConnection::SqlConnection() {}

SqlConnection::~SqlConnection() {}

bool SqlConnection::Connect(const Param& param) {
    m_param = param;

    m_mysql = mysql_init(nullptr);
    if (m_mysql == nullptr) {
        SetLastError("SqlConnection Connect Error << mysql init failed\n");
        return false;
    }

    const char* charset = m_defaultCharset.c_str();
    auto ret = mysql_options(m_mysql, MYSQL_SET_CHARSET_NAME, charset);
    if (ret != 0) {
        AcquireErrorInfo();
        return false;
    }

    unsigned int timeout = m_timeoutSeconds;
    ret = mysql_options(m_mysql, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);
    if (ret != 0) {
        AcquireErrorInfo();
        return false;
    }

    my_bool reconnect = m_autoReconnect ? 1 : 0;
    ret = mysql_options(m_mysql, MYSQL_OPT_RECONNECT, &reconnect);
    if (ret != 0) {
        AcquireErrorInfo();
        return false;
    }

    auto ptr = mysql_real_connect(m_mysql
        , m_param.m_host.c_str()
        , m_param.m_username.c_str()
        , m_param.m_password.c_str()
        , m_param.m_dbName.c_str()
        , m_param.m_port
        , nullptr
        , CLIENT_MULTI_STATEMENTS);
    if (ptr == nullptr) {
        AcquireErrorInfo();
        return false;
    }

    m_connectTime = NS_MZ_SHARE::GetNowMicrosecond();
    return true;
}

void SqlConnection::Close() {
    if (m_mysql != nullptr) {
        mysql_close(m_mysql);
        m_mysql = nullptr;
    }
}

bool SqlConnection::SelectDatabase(const char* dbName) {
    if (m_mysql == nullptr) {
        SetLastError("SqlConnection SelectDatabase Error << no mysql connection\n");
        return false;
    }

    const auto ret = mysql_select_db(m_mysql, dbName);
    if (ret != 0) {
        AcquireErrorInfo();
        return false;
    }

    return true;
}

bool SqlConnection::CheckAlive() const {
    if (m_mysql == nullptr) {
        return false;
    }

    const auto ret = mysql_ping(m_mysql);
    return ret == 0;
}

void SqlConnection::AcquireErrorInfo() {
    if (m_mysql != nullptr) {
        const auto errorCode = mysql_errno(m_mysql);
        const auto errorMsg = mysql_error(m_mysql);
        SetErrorInfo(errorCode, errorMsg);
    }
}

void SqlConnection::SetErrorInfo(int errorCode, const char* errorMsg) {
    m_errorCode = errorCode;
    m_errorMsg = errorMsg;
    SetLastError("Interal Error << errorCode[%d] errorMsg[%s]", errorCode, errorMsg);
}

DECL_NAMESPACE_METADB_END