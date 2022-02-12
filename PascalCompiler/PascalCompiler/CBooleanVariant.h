#pragma once
#ifndef _CBOOLEAN_VARIANT_H_
#define _CBOOLEAN_VARIANT_H_

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
