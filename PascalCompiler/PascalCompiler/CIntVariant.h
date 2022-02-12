#pragma once
#ifndef _CINT_VARIANT_H_
#define _CINT_VARIANT_H_

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
