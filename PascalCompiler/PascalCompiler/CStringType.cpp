#include "CStringType.h"

EDataType CStringType::GetType() const
{
	return EDataType::STRING;
}

std::string CStringType::ToString() const
{
	return "string";
}
