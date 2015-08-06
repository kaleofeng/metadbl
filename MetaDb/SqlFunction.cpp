#include "MetaDb/SqlFunction.hpp"

DECL_NAMESPACE_METADB_BEGIN

struct SqlTypeInfo {
    SqlTypeInfo(SqlType sqlType
        , enum_field_types fieldType
        , bool isUnsigned
        , int defaultLength)
        : m_sqlType(sqlType)
        , m_fieldType(fieldType)
        , m_isUnsigned(isUnsigned)
        , m_defaultLength(defaultLength) {}

    SqlType m_sqlType = SQLTYPE_NONE;
    enum_field_types m_fieldType = MYSQL_TYPE_NULL;
    bool m_isUnsigned = false;
    int m_defaultLength = 0;
};

static SqlTypeInfo gs_SqlTypeInfoMap[]  {
    { SQLTYPE_NONE, MYSQL_TYPE_NULL, false, 0 },
    { SQLTYPE_INT8, MYSQL_TYPE_TINY, false, sizeof(int8_t) },
    { SQLTYPE_UINT8, MYSQL_TYPE_TINY, true, sizeof(uint8_t) },
    { SQLTYPE_INT16, MYSQL_TYPE_SHORT, false, sizeof(int16_t) },
    { SQLTYPE_UINT16, MYSQL_TYPE_SHORT, true, sizeof(uint16_t) },
    { SQLTYPE_INT32, MYSQL_TYPE_LONG, false, sizeof(int32_t) },
    { SQLTYPE_UINT32, MYSQL_TYPE_LONG, true, sizeof(uint32_t) },
    { SQLTYPE_INT64, MYSQL_TYPE_LONGLONG, false, sizeof(int64_t) },
    { SQLTYPE_UINT64, MYSQL_TYPE_LONGLONG, true, sizeof(uint64_t) },
    { SQLTYPE_FLOAT, MYSQL_TYPE_FLOAT, false, sizeof(float) },
    { SQLTYPE_DOUBLE, MYSQL_TYPE_DOUBLE, false, sizeof(double) },
    { SQLTYPE_TIMESTAMP, MYSQL_TYPE_TIMESTAMP, false, sizeof(MYSQL_TIME) },
    { SQLTYPE_STRING, MYSQL_TYPE_STRING, false, -1 },
    { SQLTYPE_VARSTRING, MYSQL_TYPE_VAR_STRING, false, -1 },
    { SQLTYPE_BINARY, MYSQL_TYPE_BLOB, false, -1 },
};

SqlType SqlFieldToLocalType(enum_field_types fieldType, uint32_t fieldFlags) {

    const auto isUnsigned = (fieldFlags & UNSIGNED_FLAG) != 0;

    switch (fieldType) {
    case MYSQL_TYPE_TINY:
    case MYSQL_TYPE_BIT:
        return isUnsigned ? SQLTYPE_UINT8 : SQLTYPE_INT8;
    case MYSQL_TYPE_SHORT:
        return isUnsigned ? SQLTYPE_UINT16 : SQLTYPE_INT16;
    case MYSQL_TYPE_LONG:
    case MYSQL_TYPE_INT24:
        return isUnsigned ? SQLTYPE_UINT32 : SQLTYPE_INT32;
    case MYSQL_TYPE_LONGLONG:
        return isUnsigned ? SQLTYPE_UINT64 : SQLTYPE_INT64;
    case MYSQL_TYPE_FLOAT:
        return SQLTYPE_FLOAT;
    case MYSQL_TYPE_DOUBLE:
    case MYSQL_TYPE_DECIMAL:
    case MYSQL_TYPE_NEWDECIMAL:
        return SQLTYPE_DOUBLE;
    case MYSQL_TYPE_TIMESTAMP:
    case MYSQL_TYPE_DATE:
    case MYSQL_TYPE_TIME:
    case MYSQL_TYPE_DATETIME:
    case MYSQL_TYPE_NEWDATE:
        return SQLTYPE_TIMESTAMP;
    case MYSQL_TYPE_STRING:
    case MYSQL_TYPE_VARCHAR:
    case MYSQL_TYPE_VAR_STRING:
        return SQLTYPE_STRING;
    case MYSQL_TYPE_BLOB:
    case MYSQL_TYPE_TINY_BLOB:
    case MYSQL_TYPE_MEDIUM_BLOB:
    case MYSQL_TYPE_LONG_BLOB:
        return SQLTYPE_BINARY;
    default: return SQLTYPE_NONE;
    }
}

enum_field_types SqlLocalToFieldType(SqlType sqlType) {
    if (sqlType < SQLTYPE_NONE || sqlType >= SQLTYPE_COUNT) {
        sqlType = SQLTYPE_NONE;
    }

    auto& sqlTypeInfo = gs_SqlTypeInfoMap[sqlType];
    return sqlTypeInfo.m_fieldType;
}

bool SqlIsLocalTypeUnsigned(SqlType sqlType) {
    if (sqlType < SQLTYPE_NONE || sqlType >= SQLTYPE_COUNT) {
        sqlType = SQLTYPE_NONE;
    }

    auto& sqlTypeInfo = gs_SqlTypeInfoMap[sqlType];
    return sqlTypeInfo.m_isUnsigned;
}

int SqlLocalGetDefaultLength(SqlType sqlType) {
    if (sqlType < SQLTYPE_NONE || sqlType >= SQLTYPE_COUNT) {
        sqlType = SQLTYPE_NONE;
    }

    auto& sqlTypeInfo = gs_SqlTypeInfoMap[sqlType];
    return sqlTypeInfo.m_defaultLength;
}

DECL_NAMESPACE_METADB_END