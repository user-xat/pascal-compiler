#pragma once
#ifndef _CKEYWORD_TOKEN_H_
#define _CKEYWORD_TOKEN_H_

#include "CToken.h"
#include "EKeyWords.h"

class CKeywordToken : public CToken
{
private:
	EKeyWords keyword;
public:
	CKeywordToken(EKeyWords keyword);
	EKeyWords GetKeyword();
	std::string ToString() override;
};

#endif // !_CKEYWORD_TOKEN_H_
