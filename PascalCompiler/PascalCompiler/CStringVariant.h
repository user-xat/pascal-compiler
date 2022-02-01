#pragma once
#ifndef _CSTRING_VARIANT_H_
#define _CSTRING_VARIANT_H_

#include "CVariant.h"

class CStringVariant : public CVariant
{
private:
	string value;
public:
	CStringVariant(string value);
	string GetValue();
};

#endif // !_CSTRING_VARIANT_H_
