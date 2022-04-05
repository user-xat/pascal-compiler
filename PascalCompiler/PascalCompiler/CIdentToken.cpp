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
	return m_identifier;
	//return "IDENT:\t" + m_identifier;
}
