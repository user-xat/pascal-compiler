#include "CToken.h"

CToken::CToken(ETokenType type)
{
	this->type = type;
}

ETokenType CToken::GetType()
{
	return this->type;
}
