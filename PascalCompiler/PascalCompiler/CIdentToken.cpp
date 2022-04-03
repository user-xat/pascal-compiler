#include "CIdentToken.h"

CIdentToken::CIdentToken(std::string identifier) : CToken(ETokenType::IDENT)
{
	this->m_identifier = identifier;
}

std::string CIdentToken::GetIdentifier()
{
	return m_identifier;
}

std::string CIdentToken::ToString()
{
	return "IDENT:\t" + m_identifier;
}
