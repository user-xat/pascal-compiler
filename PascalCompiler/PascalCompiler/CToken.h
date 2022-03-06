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

using CTokenPtr = std::unique_ptr<CToken>;

#endif // !_CTOKEN_H_
