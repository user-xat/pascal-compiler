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
};

#endif // !_CBOOLEAN_VARIANT_H_
