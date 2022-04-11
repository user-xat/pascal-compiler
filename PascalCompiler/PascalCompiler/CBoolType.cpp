#include "CBoolType.h"

EDataType CBoolType::GetType() const
{
	return EDataType::BOOLEAN;
}

std::string CBoolType::ToString() const
{
	return "boolean";
}
