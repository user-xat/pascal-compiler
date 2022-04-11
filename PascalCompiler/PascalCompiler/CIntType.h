#pragma once
#ifndef CINTTYPE_H
#define CINTTYPE_H

#include "IDataType.h"
class CIntType : public IDataType
{
public:
	CIntType() {};
	EDataType GetType() const override;
	std::string ToString() const override;
};


#endif // !CINTTYPE_H
