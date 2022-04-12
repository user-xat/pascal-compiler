#include "CScopeStack.h"

CScopeStack::CScopeStack() : m_current{ nullptr } {}

void CScopeStack::Push()
{
	CScopePtr scope = std::make_shared<CScope>(m_current);
	m_current = scope;
}

bool CScopeStack::Pop()
{
	if (m_current != nullptr) {
		m_current = m_current->m_parent;
		return true;
	}
	return false;
}

bool CScopeStack::AddIdent(const std::string &ident, const IDataTypePtr &type) const
{
	if (m_current == nullptr)
		return false;
	return m_current->AddIdent(ident, type);
}

void CScopeStack::AccumulateIdent(const std::string& ident)
{
	m_accumulator.push_back(ident);
}

void CScopeStack::ReleaseIdent(const IDataTypePtr& type)
{
	for (std::string& ident : m_accumulator) {
		AddIdent(ident, type);
	}
	m_accumulator.clear();
}

void CScopeStack::ClearAccum()
{
	m_accumulator.clear();
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
	while (type == nullptr && ref != nullptr)
	{
		type = ref->GetIdentType(ident);
		ref = ref->m_parent;
	}
	return type;
}
