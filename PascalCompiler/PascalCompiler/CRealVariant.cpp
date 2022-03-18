#include "CRealVariant.h"

CRealVariant::CRealVariant(double value) : CVariant(EVariantType::REAL)
{
	this->value = value;
}

double CRealVariant::GetValue()
{
	return this->value;
}

std::string CRealVariant::ToString()
{
	return std::string("CONST:\treal: " + std::to_string(this->value));
}
