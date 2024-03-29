#pragma once
#ifndef CBOOLEAN_VARIANT_H
#define CBOOLEAN_VARIANT_H

#include "CVariant.h"

class CBooleanVariant : public CVariant
{
private:
	bool m_value;
public:
	CBooleanVariant(bool value);
	bool GetValue();
	std::string ToString() override;
};

#endif // !_CBOOLEAN_VARIANT_H_
