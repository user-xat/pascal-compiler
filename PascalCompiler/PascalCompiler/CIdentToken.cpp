#include "CIdentToken.h"

CIdentToken::CIdentToken(std::string identifier) : CToken(ESymbol::IDENT_TOKEN)
{
	m_identifier = identifier;
}

std::string CIdentToken::GetIdentifier() const {
	return m_identifier;
}

std::string CIdentToken::ToString() const
{
	//return m_identifier;
	return "IDENT:\t" + m_identifier;
}
