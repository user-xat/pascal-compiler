#pragma once
#ifndef CINT_VARIANT_H
#define CINT_VARIANT_H

#include "CVariant.h"

class CIntVariant : public CVariant
{
private:
	int value;
public:
	CIntVariant(int value);
	int GetValue();
	std::string ToString() override;
};

typedef std::unique_ptr<CIntVariant> CIntVariantPtr;

#endif // !_CINT_VARIANT_H_
