#pragma once
#ifndef CSTRINGTYPE_H
#define CSTRINGTYPE_H

#include "IDataType.h"

class CStringType : public IDataType
{
public:
	CStringType() {};
	EDataType GetType() const override;
	std::string ToString() const override;
};

#endif // !CSTRINGTYPE_H
