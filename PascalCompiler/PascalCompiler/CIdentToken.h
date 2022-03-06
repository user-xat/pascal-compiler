#pragma once
#ifndef CIDENT_TOKEN_H
#define CIDENT_TOKEN_H

#include "CToken.h"

class CIdentToken : public CToken
{
private:
	std::string name;
public:
	CIdentToken(std::string name);
	std::string ToString() override;
};

#endif // !_CIDENT_TOKEN_H_
