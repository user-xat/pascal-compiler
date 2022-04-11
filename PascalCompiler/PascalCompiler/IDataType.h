#pragma once
#ifndef IDATATYPE_H
#define IDATATYPE_H

#include "EDataType.h"
#include <string>
#include <memory>

class IDataType
{
public:
	virtual EDataType GetType() const = 0;
	virtual std::string ToString() const = 0;
	virtual ~IDataType() {};
};

using IDataTypePtr = std::shared_ptr<IDataType>;

#endif // !IDATATYPE_H

