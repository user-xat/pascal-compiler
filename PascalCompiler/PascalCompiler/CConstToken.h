#pragma once
#ifndef _CCONST_TOKEN_H_
#define _CCONST_TOKEN_H_

#include "CToken.h"
#include "CIntVariant.h"
#include "CRealVariant.h"
#include "CBooleanVariant.h"
#include "CStringVariant.h"

class CConstToken : public CToken
{
private:
	CVariantPtr variant;
public:
	CConstToken(EVariantType variant_type);
	std::string ToString() override;
};

#endif // !_CCONST_TOKEN_H_
