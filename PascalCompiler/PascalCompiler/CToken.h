#pragma once
#ifndef CTOKEN_H
#define CTOKEN_H

#include "ESymbol.h"
#include <string>
#include <memory>

class CToken
{
private:
	ESymbol m_type;
public:
	CToken(ESymbol type);
	ESymbol GetType() const;
	virtual std::string ToString() const = 0;
};

using CTokenPtr = std::unique_ptr<CToken>;

#endif // !_CTOKEN_H_
