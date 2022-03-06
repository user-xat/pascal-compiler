#pragma once
#ifndef CSTRING_VARIANT_H
#define CSTRING_VARIANT_H

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

typedef std::unique_ptr<CStringVariant> CStringVariantPtr;

#endif // !_CSTRING_VARIANT_H_
