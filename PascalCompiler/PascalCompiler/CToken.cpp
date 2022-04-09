#include "CToken.h"

CToken::CToken(ESymbol type)
{
	m_type = type;
}

ESymbol CToken::GetType()
{
	return m_type;
}
