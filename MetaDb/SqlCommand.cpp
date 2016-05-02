#include "MetaDb/SqlCommand.hpp"

#include <Metazion/Define.hpp>

#include "MetaDb/SqlConnection.hpp"
#include "MetaDb/SqlFunction.hpp"
#include "MetaDb/SqlResult.hpp"

DECL_NAMESPACE_METADB_BEGIN

SqlCommand::SqlCommand(SqlConnection& sqlConnection)
    : m_sqlConnection(sqlConnection) {}

SqlCommand::~SqlCommand() {}

bool SqlCommand::Prepare(const char* sql) {
    m_sql = sql;
    return PrepareStatement();
}

void SqlCommand::Close() {
    if (m_stmt != nullptr) {
        mysql_stmt_close(m_stmt);
        m_stmt = nullptr;
    }
}

bool SqlCommand::Bind(SqlFieldParam* fieldParams, int number) {
    int paramCount = mysql_stmt_param_count(m_stmt);
    if (paramCount != number) {
        m_sqlConnection.SetLastError("SqlCommand Bind Error << param number should be[%d]"
            ", but in[%d]", paramCount, number);
        return false;
    }

    m_fieldParams = fieldParams;
    m_fieldNumber = number;
    return BindParams();
}

bool SqlCommand::SetNull(int index) {
    return PushField(index, nullptr, 0);
}

bool SqlCommand::SetInt8(int index, int8_t value) {
    return PushField(index, &value, sizeof(value));
}

bool SqlCommand::SetUint8(int index, uint8_t value) {
    return PushField(index, &value, sizeof(value));
}

bool SqlCommand::SetInt16(int index, int16_t value) {
    return PushField(index, &value, sizeof(value));
}

bool SqlCommand::SetUint16(int index, uint16_t value) {
    return PushField(index, &value, sizeof(value));
}

bool SqlCommand::SetInt32(int index, int32_t value) {
    return PushField(index, &value, sizeof(value));
}

bool SqlCommand::SetUint32(int index, uint32_t value) {
    return PushField(index, &value, sizeof(value));
}

bool SqlCommand::SetInt64(int index, int64_t value) {
    return PushField(index, &value, sizeof(value));
}

bool SqlCommand::SetUint64(int index, uint64_t value) {
    return PushField(index, &value, sizeof(value));
}

bool SqlCommand::SetFloat(int index, float value) {
    return PushField(index, &value, sizeof(value));
}

bool SqlCommand::SetDouble(int index, double value) {
    return PushField(index, &value, sizeof(value));
}

bool SqlCommand::SetString(int index, const char* value, int length) {
    return PushField(index, value, length);
}

bool SqlCommand::SetBinary(int index, const void* value, int length) {
    return PushField(index, value, length);
}

bool SqlCommand::Execute() {
    if (m_stmt == nullptr) {
        m_sqlConnection.SetLastError("SqlCommand Execute Error << stmt has not been parepared");
        return false;
    }

    const auto ret = mysql_stmt_execute(m_stmt);
    if (ret != 0) {
        AcquireErrorInfo();
        return false;
    }

    return true;
}

bool SqlCommand::Execute(SqlResult& sqlResult) {
    if (m_stmt == nullptr) {
        m_sqlConnection.SetLastError("SqlResult Execute Error << stmt has not been parepared");
        return false;
    }

    const auto ret = mysql_stmt_execute(m_stmt);
    if (ret != 0) {
        AcquireErrorInfo();
        return false;
    }

    sqlResult.Reset(&m_sqlConnection, m_stmt);
    return true;
}

bool SqlCommand::PrepareStatement() {
    const auto sql = m_sql.c_str();
    const auto length = m_sql.size();

    const auto mysql = m_sqlConnection.GetMysql();
    MZ_ASSERT_TRUE(mysql != nullptr);

    m_stmt = mysql_stmt_init(mysql);
    if (m_stmt == nullptr) {
        m_sqlConnection.AcquireErrorInfo();
        return false;
    }

    const auto ret = mysql_stmt_prepare(m_stmt, sql, (unsigned long)length);
    if (ret != 0) {
        AcquireErrorInfo();
        return false;
    }

    return true;
}

bool SqlCommand::BindParams() {
    int totalLength = 0;
    for (auto index = 0; index < m_fieldNumber; ++index) {
        auto& fieldParam = m_fieldParams[index];
        fieldParam.m_buffer = nullptr;
        fieldParam.m_acturalLength = 0;
        fieldParam.m_isNull = 0;
        fieldParam.m_error = 0;

        totalLength += fieldParam.m_length;
    }

    m_bindBuffer.Reset(m_fieldNumber);
    m_dataBuffer.Reset(totalLength);

    int offset = 0;
    for (auto index = 0; index < m_fieldNumber; ++index) {
        auto& fieldParam = m_fieldParams[index];
        fieldParam.m_buffer = m_dataBuffer.GetValue(offset);
        offset += fieldParam.m_length;

        auto fieldBind = m_bindBuffer.GetValue(index);
        MZ_ASSERT_TRUE(fieldBind != nullptr);
        fieldBind->buffer_type = SqlLocalToFieldType(fieldParam.m_sqlType);
        fieldBind->buffer = fieldParam.m_buffer;
        fieldBind->buffer_length = fieldParam.m_length;
        fieldBind->length = &fieldParam.m_acturalLength;
        fieldBind->is_null = &fieldParam.m_isNull;
        fieldBind->is_unsigned = SqlIsLocalTypeUnsigned(fieldParam.m_sqlType);
        fieldBind->error = &fieldParam.m_error;
    }

    auto binds = m_bindBuffer.GetValue(0);
    const auto ret = mysql_stmt_bind_param(m_stmt, binds);
    if (ret != 0) {
        AcquireErrorInfo();
        return false;
    }

    return true;
}

void SqlCommand::AcquireErrorInfo() {
    if (m_stmt != nullptr) {
        const auto errorCode = mysql_stmt_errno(m_stmt);
        const auto errorMsg = mysql_stmt_error(m_stmt);
        m_sqlConnection.SetErrorInfo(errorCode, errorMsg);
    }
}

bool SqlCommand::PushField(int index, const void* value, int length) {
    MZ_ASSERT_TRUE(length >= 0);

    if (!CheckIndex(index)) {
        m_sqlConnection.SetLastError("SqlCommand PushField Error << field index[%d]"
            "out of range[%d", index, m_fieldNumber);
        return false;
    }

    auto& fieldParam = m_fieldParams[index];
    if (length > fieldParam.m_length) {
        m_sqlConnection.SetLastError("SqlCommand PushField Error << data length[%d]"
            "is greater than buffer length[%d]", length, fieldParam.m_length);
        return false;
    }

    if (value != nullptr) {
        memcpy(fieldParam.m_buffer, value, length);
        fieldParam.m_acturalLength = length;
        fieldParam.m_isNull = 0;
    }
    else {
        fieldParam.m_acturalLength = 0;
        fieldParam.m_isNull = 1;
    }

    return true;
}

bool SqlCommand::CheckIndex(int index) const {
    return index >= 0 && index < m_fieldNumber;
}

DECL_NAMESPACE_METADB_END