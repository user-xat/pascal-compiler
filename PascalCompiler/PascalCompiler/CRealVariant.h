#pragma once
#ifndef CREAL_VARIANT_H
#define CREAL_VARIANT_H

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
