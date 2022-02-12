#pragma once
#ifndef _CREAL_VARIANT_H_
#define _CREAL_VARIANT_H_

#include "CVariant.h"

class CRealVariant : public CVariant
{
private:
	double value;
public:
	CRealVariant(double value);
	double GetValue();
	std::string ToString() override;
};

typedef std::unique_ptr<CRealVariant> CRealVariantPtr;

#endif // !_CREAL_VARIANT_H_
