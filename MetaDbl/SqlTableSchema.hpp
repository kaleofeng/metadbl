#ifndef _METADBL_SQLTABLESCHEMA_HPP_
#define _METADBL_SQLTABLESCHEMA_HPP_

#include "MetaDbl/CoreInclude.hpp"

#include <vector>

#include "MetaDbl/SqlTableField.hpp"

DECL_NAMESPACE_METADBL_BEGIN

class SqlTableSchema {

private:
    std::string m_dbName;
    std::string m_tableName;
    std::vector<SqlTableField> m_fields;

public:
    SqlTableSchema();

    SqlTableSchema(const SqlTableSchema& other);

    SqlTableSchema& operator =(const SqlTableSchema& other);

    SqlTableSchema(SqlTableSchema&& other);

    SqlTableSchema& operator =(SqlTableSchema&& other);

    const std::string& GetDbName() const;

    void SetDbName(const std::string& dbName);

    const std::string& GetTableName() const;

    void SetTableName(const std::string& tableName);

    int GetFieldSize() const;

    SqlTableField GetField(int index) const;

    void AppendField(const SqlTableField& tableField);

    void RemoveAllField();
};

inline const std::string& SqlTableSchema::GetDbName() const {
    return m_dbName;
}

inline void SqlTableSchema::SetDbName(const std::string& value) {
    m_dbName = value;
}

inline const std::string& SqlTableSchema::GetTableName() const {
    return m_tableName;
}

inline void SqlTableSchema::SetTableName(const std::string& value) {
    m_tableName = value;
}

inline int SqlTableSchema::GetFieldSize() const {
    return static_cast<int>(m_fields.size());
}

inline SqlTableField SqlTableSchema::GetField(int index) const {
    if (index < 0 || index > m_fields.size()) {
        return {};
    }

    return m_fields[index];
}

inline void SqlTableSchema::AppendField(const SqlTableField& value) {
    m_fields.push_back(value);
}

inline void SqlTableSchema::RemoveAllField() {
    m_fields.clear();
}

DECL_NAMESPACE_METADBL_END

#endif // _METADBL_SQLTABLESCHEMA_HPP_
