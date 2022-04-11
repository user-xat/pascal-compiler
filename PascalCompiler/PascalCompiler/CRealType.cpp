#include "CRealType.h"

EDataType CRealType::GetType() const 
{
	return EDataType::REAL;
}

std::string CRealType::ToString() const 
{
	return "real";
}
