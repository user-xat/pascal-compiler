#include "CStringVariant.h"

CStringVariant::CStringVariant(std::string value) : CVariant(ESymbol::STRING_VARIANT)
{
	this->m_value = value;
}

std::string CStringVariant::GetValue()
{
	return this->m_value;
}

std::string CStringVariant::ToString()
{
	//return std::string(this->m_value);
	return std::string("CONST:\tstring: " + this->m_value);
}
