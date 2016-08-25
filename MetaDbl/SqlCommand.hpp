#ifndef _METADBL_SQLCOMMAND_HPP_
#define _METADBL_SQLCOMMAND_HPP_

#include "MetaDbl/CoreInclude.hpp"

#include <string>

#include "MetaDbl/SqlBuffer.hpp"
#include "MetaDbl/SqlDefine.hpp"
#include "MetaDbl/SqlFieldParam.hpp"

DECL_NAMESPACE_METADBL_BEGIN

class SqlConnection;
class SqlResult;

class SqlCommand {

private:
    SqlConnection& m_sqlConnection;

    MYSQL_STMT* m_stmt = nullptr;
    std::string m_sql;
    SqlFieldParam* m_fieldParams = nullptr;
    int m_fieldNumber = 0;

    SqlBuffer<MYSQL_BIND, 16, 2> m_bindBuffer;
    SqlBuffer<char, 1024, 1024> m_dataBuffer;

public:
    SqlCommand(SqlConnection& sqlConnection);

    ~SqlCommand();

public:
    bool Prepare(const char* sql);

    void Close();

    bool Bind(SqlFieldParam* fieldParams, int number);

    bool SetNull(int index);

    bool SetInt8(int index, int8_t value);

    bool SetUint8(int index, uint8_t value);

    bool SetInt16(int index, int16_t value);

    bool SetUint16(int index, uint16_t value);

    bool SetInt32(int index, int32_t value);

    bool SetUint32(int index, uint32_t value);

    bool SetInt64(int index, int64_t value);

    bool SetUint64(int index, uint64_t value);

    bool SetFloat(int index, float value);

    bool SetDouble(int index, double value);

    bool SetString(int index, const char* value, int length);

    bool SetBinary(int index, const void* value, int length);

    bool Execute();

    bool Execute(SqlResult& sqlResult);

private:
    bool PrepareStatement();

    bool BindParams();

    bool PushField(int index, const void* value, int length);

    bool CheckIndex(int index) const;

    void AcquireErrorInfo();
};

DECL_NAMESPACE_METADBL_END

#endif // _METADBL_SQLCOMMAND_HPP_
