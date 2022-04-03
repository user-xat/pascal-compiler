#pragma once
#ifndef CREAL_VARIANT_H
#define CREAL_VARIANT_H

#include "CVariant.h"

class CRealVariant : public CVariant
{
private:
	double m_value;
public:
	CRealVariant(double value);
	double GetValue();
	std::string ToString() override;
};

#endif // !_CREAL_VARIANT_H_
