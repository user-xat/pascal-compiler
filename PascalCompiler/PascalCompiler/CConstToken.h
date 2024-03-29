#pragma once
#ifndef CCONST_TOKEN_H
#define CCONST_TOKEN_H

#include "CToken.h"
#include "CIntVariant.h"
#include "CRealVariant.h"
#include "CBooleanVariant.h"
#include "CStringVariant.h"

class CConstToken : public CToken
{
private:
	CVariantPtr m_variant;
public:
	CConstToken(int value);
	CConstToken(double value);
	CConstToken(std::string value);
	CConstToken(bool value);
	CVariant* GetVariant() const;
	std::string ToString() const override;
};

#endif // !_CCONST_TOKEN_H_
