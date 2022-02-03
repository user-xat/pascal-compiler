#pragma once
#ifndef _CIDENT_TOKEN_H_
#define _CIDENT_TOKEN_H_

#include "CToken.h"

class CIdentToken : public CToken
{
private:

public:
	CIdentToken();
	std::string ToString() override;
};

#endif // !_CIDENT_TOKEN_H_
