#include "CIntType.h"

EDataType CIntType::GetType() const
{
	return EDataType::INTEGER;
}

std::string CIntType::ToString() const
{
	return "integer";
}
