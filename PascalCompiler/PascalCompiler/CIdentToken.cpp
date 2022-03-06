#include "CIdentToken.h"

CIdentToken::CIdentToken(std::string name) : CToken(ETokenType::IDENT)
{
	this->name = name;
}

std::string CIdentToken::ToString()
{
	return name;
}
