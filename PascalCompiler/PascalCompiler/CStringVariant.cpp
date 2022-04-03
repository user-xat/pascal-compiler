#include "CStringVariant.h"

CStringVariant::CStringVariant(std::string value) : CVariant(EVariantType::STRING)
{
	this->m_value = value;
}

std::string CStringVariant::GetValue()
{
	return this->m_value;
}

std::string CStringVariant::ToString()
{
	return std::string("CONST:\tstring: " + this->m_value);
}
