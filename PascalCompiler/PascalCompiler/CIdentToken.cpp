#include "CIdentToken.h"

CIdentToken::CIdentToken(std::string identifier) : CToken(ETokenType::IDENT)
{
	this->identifier = identifier;
}

std::string CIdentToken::ToString()
{
	return identifier;
}
