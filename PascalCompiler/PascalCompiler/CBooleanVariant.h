#pragma once
#ifndef CBOOLEAN_VARIANT_H
#define CBOOLEAN_VARIANT_H

#include "CVariant.h"

class CBooleanVariant : public CVariant
{
private:
	bool value;
public:
	CBooleanVariant(bool value);
	bool GetValue();
	std::string ToString() override;
};

typedef std::unique_ptr<CBooleanVariant> CBooleanVariantPtr;

#endif // !_CBOOLEAN_VARIANT_H_
