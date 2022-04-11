#include "CErrorType.h"

EDataType CErrorType::GetType() const
{
	return EDataType::ERROR;
}

std::string CErrorType::ToString() const
{
	return "type error";
}
