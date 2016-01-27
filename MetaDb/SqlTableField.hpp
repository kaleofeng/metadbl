#ifndef _METADB_SQLTABLEFIELD_HPP_
#define _METADB_SQLTABLEFIELD_HPP_

#include "MetaDb/CoreInclude.hpp"

#include "MetaDb/SqlDefine.hpp"

DECL_NAMESPACE_METADB_BEGIN

struct SqlTableField {

    int  m_length = 0;
    bool m_nullable = false;
    bool m_autoIncrement = false;
    bool m_primaryKey = false;
    SqlType m_sqlType = SQLTYPE_NONE;
    std::string m_name;
    std::string m_defaultValue;

    SqlTableField() {}

    SqlTableField(const SqlTableField& other) {
        *this = other;
    }

    SqlTableField& operator =(const SqlTableField& other) {
        if (&other != this) {
            m_length = other.m_length;
            m_nullable = other.m_nullable;
            m_autoIncrement = other.m_autoIncrement;
            m_primaryKey = other.m_primaryKey;
            m_sqlType = other.m_sqlType;
            m_name = other.m_name;
            m_defaultValue = other.m_defaultValue;
        }

        return *this;
    }

    SqlTableField(SqlTableField&& other) {
        *this = std::move(other);
    }

    SqlTableField& operator =(SqlTableField&& other) {
        if (&other != this) {
            m_length = other.m_length;
            m_nullable = other.m_nullable;
            m_autoIncrement = other.m_autoIncrement;
            m_primaryKey = other.m_primaryKey;
            m_sqlType = other.m_sqlType;
            m_name = std::move(other.m_name);
            m_defaultValue = std::move(other.m_defaultValue);
        }

        return *this;
    }
};

DECL_NAMESPACE_METADB_END

#endif // _METADB_SQLTABLEFIELD_HPP_
