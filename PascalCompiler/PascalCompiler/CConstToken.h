#pragma once
#ifndef _CCONST_TOKEN_H_
#define _CCONST_TOKEN_H_

#include "CToken.h"
#include "CIntVariant.h"
#include "CRealVariant.h"
#include "CBooleanVariant.h"
#include "CStringVariant.h"

template<typename T>
class CConstToken : public CToken
{
private:
	CVariantPtr variant;
public:
	CConstToken(EVariantType variant_type, T value);
	std::string ToString() override;
};

template<typename T>
CConstToken<T>::CConstToken(EVariantType variant_type, T value) : CToken(ETokenType::CONST)
{
	switch (variant_type)
	{
	case EVariantType::INTEGER:
		variant = std::make_unique(new CIntVariant(value));
		break;
	case EVariantType::REAL:
		variant = std::make_unique(new CRealVariant(value));
		break;
	case EVariantType::STRING:
		variant = std::make_unique(new CStringVariant(value));
		break;
	case EVariantType::BOOLEAN:
		variant = std::make_unique(new CBooleanVariant(value));
		break;
	default:
		break;
	}
}

template<typename T>
inline std::string CConstToken<T>::ToString()
{
	return variant->ToString();
}

#endif // !_CCONST_TOKEN_H_
