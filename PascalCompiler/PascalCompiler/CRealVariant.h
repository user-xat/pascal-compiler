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
};


#endif // !_CREAL_VARIANT_H_
