#include "CIntVariant.h"

CIntVariant::CIntVariant(int value) : CVariant(EVariantType::INTEGER)
{
	this->value = value;
}

int CIntVariant::GetValue()
{
	return this->value;
}

std::string CIntVariant::ToString()
{
	return std::string("CONST:\tinteger: " + std::to_string(this->value));
}
