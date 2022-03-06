#pragma once
#ifndef CTOKEN_H
#define CTOKEN_H

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
