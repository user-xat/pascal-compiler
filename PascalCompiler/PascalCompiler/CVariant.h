#pragma once
#ifndef CVARIANT_H
#define CVARIANT_H

#include <string>
#include <memory>
#include "ESymbol.h"

class CVariant
{
private:
	ESymbol m_type;
public:
	CVariant(ESymbol type);
	ESymbol GetType();
	virtual std::string ToString() = 0;
};

using CVariantPtr = std::unique_ptr<CVariant>;

#endif // !_CVARIANT_H_
