#pragma once
#ifndef CKEYWORD_TOKEN_H
#define CKEYWORD_TOKEN_H

#include "CToken.h"
#include "ESymbol.h"

class CKeywordToken : public CToken
{
private:
	ESymbol m_keyword;
public:
	CKeywordToken(ESymbol keyword);
	ESymbol GetKeyword() const;
	std::string ToString() const override;
};

#endif // !_CKEYWORD_TOKEN_H_
