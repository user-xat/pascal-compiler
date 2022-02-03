#include "CBooleanVariant.h"

CBooleanVariant::CBooleanVariant(bool value) : CVariant(EVariantType::BOOLEAN)
{
	this->value = value;
}

bool CBooleanVariant::GetValue()
{
	return this->value;
}
