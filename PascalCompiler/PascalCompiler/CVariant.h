#pragma once
#ifndef _CVARIANT_H_
#define _CVARIANT_H_

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

typedef std::unique_ptr<CVariant> CVariantPtr;

#endif // !_CVARIANT_H_