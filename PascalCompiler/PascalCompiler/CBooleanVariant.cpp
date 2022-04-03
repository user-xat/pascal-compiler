#include "CBooleanVariant.h"

CBooleanVariant::CBooleanVariant(bool value) : CVariant(EVariantType::BOOLEAN)
{
	this->m_value = value;
}

bool CBooleanVariant::GetValue()
{
	return this->m_value;
}

std::string CBooleanVariant::ToString()
{
	std::string result("CONST:\tboolean: ");
	if (this->m_value)
		result += "True";
	else
		result += "False";
	return result;
}
