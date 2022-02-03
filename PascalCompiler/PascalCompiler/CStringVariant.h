#pragma once
#ifndef _CSTRING_VARIANT_H_
#define _CSTRING_VARIANT_H_

#include "CVariant.h"

class CStringVariant : public CVariant
{
private:
	std::string value;
public:
	CStringVariant(std::string value);
	std::string GetValue();
	std::string ToString() override;
};

#endif // !_CSTRING_VARIANT_H_
