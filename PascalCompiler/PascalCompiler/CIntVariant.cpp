#include "CIntVariant.h"

CIntVariant::CIntVariant(int value) : CVariant(EVariantType::INTEGER)
{
	this->value = value;
}

int CIntVariant::GetValue()
{
	return this->value;
}
