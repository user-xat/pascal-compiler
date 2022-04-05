#include "CRealVariant.h"

CRealVariant::CRealVariant(double value) : CVariant(EVariantType::REAL)
{
	this->m_value = value;
}

double CRealVariant::GetValue()
{
	return this->m_value;
}

std::string CRealVariant::ToString()
{
	return std::string(std::to_string(this->m_value));
	//return std::string("CONST:\treal: " + std::to_string(this->m_value));
}
