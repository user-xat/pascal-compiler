#pragma once
#ifndef CBOOLTYPE_H
#define CBOOLTYPE_H

#include "IDataType.h"

class CBoolType : public IDataType
{
public:
	CBoolType() {};
	EDataType GetType() const override;
	std::string ToString() const override;
};

#endif // !CBOOLTYPE_H
