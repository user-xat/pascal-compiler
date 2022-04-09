#include "CVariant.h"

CVariant::CVariant(ESymbol type)
{
	this->m_type = type;
}

ESymbol CVariant::GetType()
{
	return this->m_type;
}
