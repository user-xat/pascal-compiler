#include "CConstToken.h"

CConstToken::CConstToken(int value) : CToken(ETokenType::CONST)
{
	this->variant = std::make_unique<CIntVariant>(value);
}

CConstToken::CConstToken(double value) : CToken(ETokenType::CONST)
{
	this->variant = std::make_unique<CRealVariant>(value);
}

CConstToken::CConstToken(std::string value) : CToken(ETokenType::CONST)
{
	this->variant = std::make_unique<CStringVariant>(value);
}

CConstToken::CConstToken(bool value) : CToken(ETokenType::CONST)
{
	this->variant = std::make_unique<CBooleanVariant>(value);
}

CVariant* CConstToken::GetVariant()
{
	return variant.get();
}

std::string CConstToken::ToString()
{
	return variant->ToString();
}
