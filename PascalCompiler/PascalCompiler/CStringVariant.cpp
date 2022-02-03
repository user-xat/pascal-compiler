#include "CStringVariant.h"

CStringVariant::CStringVariant(std::string value) : CVariant(EVariantType::STRING)
{
	this->value = value;
}

std::string CStringVariant::GetValue()
{
	return this->value;
}
