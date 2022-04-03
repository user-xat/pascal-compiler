#pragma once
#ifndef CKEYWORD_TOKEN_H
#define CKEYWORD_TOKEN_H

#include "CToken.h"
#include "EKeyWords.h"

class CKeywordToken : public CToken
{
private:
	EKeyWords m_keyword;
public:
	CKeywordToken(EKeyWords keyword);
	EKeyWords GetKeyword();
	std::string ToString() override;
};

#endif // !_CKEYWORD_TOKEN_H_
