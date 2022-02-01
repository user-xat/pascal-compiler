#pragma once
#ifndef _CTOKEN_H_
#define _CTOKEN_H_

#include "ETokenType.h"
#include <string>
using namespace std;

class CToken
{
private:
	ETokenType type;
public:
	CToken(ETokenType type);
	ETokenType GetType();
	virtual string ToString() = 0;
};

#endif // !_CTOKEN_H_
