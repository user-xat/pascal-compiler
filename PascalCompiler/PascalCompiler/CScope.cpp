#include "CScope.h"

CScope::CScope(CScopePtr parent) : m_parent{ parent } {}

bool CScope::AddIdent(const std::string ident, const IDataTypePtr type)
{
	if (!Contain(ident)) {
		m_idents.emplace(ident, type);
		return true;
	}
	return false;
}

bool CScope::Contain(const std::string& ident) const
{
	return m_idents.find(ident) != m_idents.cend();
}

IDataTypePtr CScope::GetIdentType(const std::string& ident) const
{
	auto type = m_idents.find(ident);
	if (type == m_idents.cend()) {
		return nullptr;
	}
	return type->second;
}
