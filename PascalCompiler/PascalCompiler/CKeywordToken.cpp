#include "CKeywordToken.h"

CKeywordToken::CKeywordToken(EKeyWords keyword) : CToken(ETokenType::KEYWORD)
{
	this->keyword = keyword;
}

EKeyWords CKeywordToken::GetKeyword()
{
	return this->keyword;
}
