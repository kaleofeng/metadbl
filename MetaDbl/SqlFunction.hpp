#ifndef _METADB_SQLFUNCTION_HPP_
#define _METADB_SQLFUNCTION_HPP_

#include "MetaDbl/CoreInclude.hpp"

#include "MetaDbl/SqlDefine.hpp"

DECL_NAMESPACE_METADB_BEGIN

SqlType SqlFieldToLocalType(enum_field_types fieldType, uint32_t fieldFlags);

enum_field_types SqlLocalToFieldType(SqlType sqlType);

bool SqlIsLocalTypeUnsigned(SqlType sqlType);

int SqlLocalGetDefaultLength(SqlType sqlType);

DECL_NAMESPACE_METADB_END

#endif // _METADB_SQLFUNCTION_HPP_
