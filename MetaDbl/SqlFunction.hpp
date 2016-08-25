#ifndef _METADBL_SQLFUNCTION_HPP_
#define _METADBL_SQLFUNCTION_HPP_

#include "MetaDbl/CoreInclude.hpp"

#include "MetaDbl/SqlDefine.hpp"

DECL_NAMESPACE_METADBL_BEGIN

SqlType SqlFieldToLocalType(enum_field_types fieldType, uint32_t fieldFlags);

enum_field_types SqlLocalToFieldType(SqlType sqlType);

bool SqlIsLocalTypeUnsigned(SqlType sqlType);

int SqlLocalGetDefaultLength(SqlType sqlType);

DECL_NAMESPACE_METADBL_END

#endif // _METADBL_SQLFUNCTION_HPP_
