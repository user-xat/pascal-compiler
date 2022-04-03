#include "CToken.h"

CToken::CToken(ETokenType type)
{
	this->m_type = type;
}

ETokenType CToken::GetType()
{
	return this->m_type;
}
