#include "CRealVariant.h"

CRealVariant::CRealVariant(double value) : CVariant(EVariantType::REAL)
{
	this->value = value;
}

double CRealVariant::GetValue()
{
	return this->value;
}
