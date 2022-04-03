#include "CVariant.h"

CVariant::CVariant(EVariantType type)
{
	this->m_type = type;
}

EVariantType CVariant::GetType()
{
	return this->m_type;
}
