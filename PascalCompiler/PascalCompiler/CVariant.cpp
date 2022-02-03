#include "CVariant.h"

CVariant::CVariant(EVariantType type)
{
	this->type = type;
}

EVariantType CVariant::GetType()
{
	return this->type;
}
