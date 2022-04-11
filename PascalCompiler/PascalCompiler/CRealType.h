#pragma once
#ifndef CREALTYPE_H
#define CREALTYPE_H

#include "IDataType.h"

class CRealType : public IDataType
{
public:
	CRealType() {};
	EDataType GetType() const override;
	std::string ToString() const override;
};

#endif // !CREALTYPE_H
