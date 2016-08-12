#include "MetaDbl/SqlResult.hpp"

#include <Metazion/Define.hpp>

#include "MetaDbl/SqlConnection.hpp"
#include "MetaDbl/SqlFunction.hpp"

DECL_NAMESPACE_METADB_BEGIN

SqlResult::SqlResult() {}

SqlResult::~SqlResult() {}

bool SqlResult::Reset(SqlConnection* sqlConnection, MYSQL_STMT* stmt) {
    m_sqlConnection = sqlConnection;
    m_stmt = stmt;
    return true;
}

bool SqlResult::Bind(SqlFieldParam* fieldParams, int number) {
    int paramCount = mysql_stmt_field_count(m_stmt);
    if (paramCount != number) {
        m_sqlConnection->SetLastError("SqlResult Bind Error << param number should be[%d]"
            ", but in[%d]", paramCount, number);
        return false;
    }
    
    m_fieldParams = fieldParams;
    m_fieldNumber = number;
    return BindParams();
}

bool SqlResult::Next() {
    const auto ret = mysql_stmt_fetch(m_stmt);
    if (ret != 0) {
        AcquireErrorInfo();
        return false;
    }

    return true;
}

bool SqlResult::IsNull(int index, bool& value) {
    int length = 0;
    const auto ret = PullField(index, nullptr, length);
    value = length != 0;
    return ret;
}

bool SqlResult::GetInt8(int index, int8_t& value) {
    int length = sizeof(value);
    return PullField(index, &value, length);
}

bool SqlResult::GetUint8(int index, uint8_t& value) {
    int length = sizeof(value);
    return PullField(index, &value, length);
}

bool SqlResult::GetInt16(int index, int16_t& value) {
    int length = sizeof(value);
    return PullField(index, &value, length);
}

bool SqlResult::GetUint16(int index, uint16_t& value) {
    int length = sizeof(value);
    return PullField(index, &value, length);
}

bool SqlResult::GetInt32(int index, int32_t& value) {
    int length = sizeof(value);
    return PullField(index, &value, length);
}

bool SqlResult::GetUint32(int index, uint32_t& value) {
    int length = sizeof(value);
    return PullField(index, &value, length);
}

bool SqlResult::GetInt64(int index, int64_t& value) {
    int length = sizeof(value);
    return PullField(index, &value, length);
}

bool SqlResult::GetUint64(int index, uint64_t& value) {
    int length = sizeof(value);
    return PullField(index, &value, length);
}

bool SqlResult::GetFloat(int index, float& value) {
    int length = sizeof(value);
    return PullField(index, &value, length);
}

bool SqlResult::GetDouble(int index, double& value) {
    int length = sizeof(value);
    return PullField(index, &value, length);
}

bool SqlResult::GetString(int index, char* value, int& length) {
    return PullField(index, value, length);
}

bool SqlResult::GetBinary(int index, void* value, int& length) {
    return PullField(index, value, length);
}

bool SqlResult::BindParams() {
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
    const auto ret = mysql_stmt_bind_result(m_stmt, binds);
    if (ret != 0) {
        AcquireErrorInfo();
        return false;
    }

    return true;
}

void SqlResult::AcquireErrorInfo() {
    if (m_stmt != nullptr) {
        const auto errorCode = mysql_stmt_errno(m_stmt);
        const auto errorMsg = mysql_stmt_error(m_stmt);
        m_sqlConnection->SetErrorInfo(errorCode, errorMsg);
    }
}

bool SqlResult::PullField(int index, void* value, int& length) {
    MZ_ASSERT_TRUE(length >= 0);

    if (!CheckIndex(index)) {
        m_sqlConnection->SetLastError("SqlResult PullField Error << field index[%d]"
            "out of range[%d", index, m_fieldNumber);
        return false;
    }

    auto& fieldParam = m_fieldParams[index];
    if (length < fieldParam.m_length) {
        m_sqlConnection->SetLastError("SqlResult PullField Error << buffer length[%d]"
            "is less than data length[%d]", length, fieldParam.m_length);
        return false;
    }

    if (value == nullptr) {
        length = fieldParam.m_isNull;
        return true;
    }

    if (fieldParam.m_isNull == 0) {
        memcpy(value, fieldParam.m_buffer, fieldParam.m_acturalLength);
        length = fieldParam.m_acturalLength;
    }
    else {
        length = 0;
    }
    
    return true;
}

bool SqlResult::CheckIndex(int index) const {
    return index >= 0 && index < m_fieldNumber;
}

DECL_NAMESPACE_METADB_END