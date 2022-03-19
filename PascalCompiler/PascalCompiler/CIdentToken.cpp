#include "CIdentToken.h"

CIdentToken::CIdentToken(std::string identifier) : CToken(ETokenType::IDENT)
{
	this->identifier = identifier;
}

std::string CIdentToken::GetIdentifier()
{
	return identifier;
}

std::string CIdentToken::ToString()
{
	return "IDENT:\t" + identifier;
}
