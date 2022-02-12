#include "CBooleanVariant.h"

CBooleanVariant::CBooleanVariant(bool value) : CVariant(EVariantType::BOOLEAN)
{
	this->value = value;
}

bool CBooleanVariant::GetValue()
{
	return this->value;
}

std::string CBooleanVariant::ToString()
{
	std::string result("BOOLEAN: ");
	if (this->value)
		result += "True";
	else
		result += "False";
	return result;
}
