#pragma once
#ifndef CSCOPE_H
#define CSCOPE_H

#include <map>
#include <memory>
#include "IDataType.h"

class CScope;
using CScopePtr = std::shared_ptr<CScope>;

class CScope
{
	friend class CScopeStack;
private:
	CScopePtr m_parent;
	std::map<std::string, IDataTypePtr> m_idents;

public:
	CScope(CScopePtr parent);
	bool AddIdent(const std::string ident, const IDataTypePtr type);
	bool Contain(const std::string& ident) const;
	IDataTypePtr GetIdentType(const std::string& ident) const;
};

#endif // !CSCOPE_H

