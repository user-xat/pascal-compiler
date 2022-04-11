#pragma once
#ifndef CSCOPESTACK_H
#define CSCOPESTACK_H

#include "CScope.h"

class CScopeStack
{
private:
	CScopePtr m_current;

public:
	CScopeStack();
	void Push();
	bool Pop();
	bool AddIdent(const std::string ident, const IDataTypePtr type) const;
	bool Contain(const std::string& ident) const;
	IDataTypePtr GetIdentType(const std::string& ident) const;
};

#endif // !CSCOPESTACK_H
