#include "CToken.h"

CToken::CToken(ESymbol type)
{
	this->m_type = type;
}

ESymbol CToken::GetType()
{
	return this->m_type;
}
