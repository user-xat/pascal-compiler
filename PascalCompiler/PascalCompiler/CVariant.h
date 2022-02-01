#pragma once
#ifndef _CVARIANT_H_
#define _CVARIANT_H_

#include <string>
#include "EVariantType.h"
using namespace std;

class CVariant
{
private:
	EVariantType type;
public:
	CVariant(EVariantType type);
	EVariantType GetType();
	virtual string ToString() = 0;
};

#endif // !_CVARIANT_H_
