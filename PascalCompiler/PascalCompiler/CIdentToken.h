#pragma once
#ifndef CIDENT_TOKEN_H
#define CIDENT_TOKEN_H

#include "CToken.h"

class CIdentToken : public CToken
{
private:
	std::string m_identifier;
public:
	CIdentToken(std::string identifier);
	std::string GetIdentifier() const;
	std::string ToString() const override;
};

#endif // !_CIDENT_TOKEN_H_
