#include "CBooleanVariant.h"

CBooleanVariant::CBooleanVariant(bool value) : CVariant(ESymbol::BOOLEAN_VARIANT)
{
	m_value = value;
}

bool CBooleanVariant::GetValue()
{
	return m_value;
}

std::string CBooleanVariant::ToString()
{
	std::string result("CONST:\tboolean: ");
	if (m_value)
		result += "True";
	else
		result += "False";
	return result;
}
