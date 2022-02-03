#pragma once
#ifndef _CTOKEN_H_
#define _CTOKEN_H_

#include "ETokenType.h"
#include <string>
#include <memory>

class CToken
{
private:
	ETokenType type;
public:
	CToken(ETokenType type);
	ETokenType GetType();
	virtual std::string ToString() = 0;
};

typedef std::unique_ptr<CToken> CTokenPtr;

#endif // !_CTOKEN_H_
