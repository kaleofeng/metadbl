#include "MetaDb/SqlTableSchema.hpp"

DECL_NAMESPACE_METADB_BEGIN

SqlTableSchema::SqlTableSchema() {}

SqlTableSchema::SqlTableSchema(const SqlTableSchema& other) {
    *this = other;
}

SqlTableSchema& SqlTableSchema::operator =(const SqlTableSchema& other) {
    if (&other != this) {
        m_dbName = other.m_dbName;
        m_tableName = other.m_tableName;
        m_fields = other.m_fields;
    }

    return *this;
}

SqlTableSchema::SqlTableSchema(SqlTableSchema&& other) {
    *this = std::move(other);
}

SqlTableSchema& SqlTableSchema::operator =(SqlTableSchema&& other) {
    if (&other != this) {
        m_dbName = std::move(other.m_dbName);
        m_tableName = std::move(other.m_tableName);
        m_fields = std::move(other.m_fields);
    }

    return *this;
}

DECL_NAMESPACE_METADB_END