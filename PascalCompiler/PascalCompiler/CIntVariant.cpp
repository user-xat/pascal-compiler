#include "CIntVariant.h"

CIntVariant::CIntVariant(int value) : CVariant(ESymbol::INTEGER_VARIANT)
{
	this->m_value = value;
}

int CIntVariant::GetValue()
{
	return this->m_value;
}

std::string CIntVariant::ToString()
{
	//return std::string(std::to_string(this->m_value));
	return std::string("CONST:\tinteger: " + std::to_string(this->m_value));
}
