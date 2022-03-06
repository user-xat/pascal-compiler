#pragma once
#ifndef CVARIANT_H
#define CVARIANT_H

#include <string>
#include <memory>
#include "EVariantType.h"

class CVariant
{
private:
	EVariantType type;
public:
	CVariant(EVariantType type);
	EVariantType GetType();
	virtual std::string ToString() = 0;
};

using CVariantPtr = std::unique_ptr<CVariant>;

#endif // !_CVARIANT_H_
