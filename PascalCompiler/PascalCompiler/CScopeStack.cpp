#include "CScopeStack.h"

CScopeStack::CScopeStack()
{
	m_current = std::make_shared<CScope>(nullptr);
}

void CScopeStack::Push()
{
	CScopePtr scope = std::make_shared<CScope>(m_current);
	m_current = scope;
}

bool CScopeStack::Pop()
{
	if (m_current->m_parent != nullptr) {
		m_current = m_current->m_parent;
		return true;
	}
	return false;
}

bool CScopeStack::AddIdent(const std::string ident, const IDataTypePtr type) const
{
	return m_current->AddIdent(ident, type);
}

bool CScopeStack::Contain(const std::string& ident) const
{
	CScopePtr ref = m_current;
	while (ref != nullptr && !ref->Contain(ident)) {
		ref = ref->m_parent;
	}
	return ref != nullptr;
}

IDataTypePtr CScopeStack::GetIdentType(const std::string& ident) const
{
	IDataTypePtr type;
	CScopePtr ref = m_current;
	do
	{
		type = ref->GetIdentType(ident);
		ref = ref->m_parent;
	} while (type == nullptr && ref != nullptr);
	return type;
}
