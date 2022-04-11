#pragma once
#ifndef CERRORTYPE_H
#define CERRORTYPE_H

#include "IDataType.h"

class CErrorType : public IDataType
{
public:
	CErrorType() {};
	EDataType GetType() const override;
	std::string ToString() const override;
};

#endif // !CERRORTYPE_H
