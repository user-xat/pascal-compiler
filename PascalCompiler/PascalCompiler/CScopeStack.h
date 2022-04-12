#pragma once
#ifndef CSCOPESTACK_H
#define CSCOPESTACK_H

#include "CScope.h"
#include <vector>

class CScopeStack
{
private:
	CScopePtr m_current;
	std::vector<std::string> m_accumulator;

public:
	CScopeStack();
	void Push();
	bool Pop();
	bool AddIdent(const std::string &ident, const IDataTypePtr &type) const;
	void AccumulateIdent(const std::string &ident);
	void ReleaseIdent(const IDataTypePtr& type);
	void ClearAccum();
	bool Contain(const std::string& ident) const;
	IDataTypePtr GetIdentType(const std::string& ident) const;
};

using CScopeStackPtr = std::unique_ptr<CScopeStack>;

#endif // !CSCOPESTACK_H
