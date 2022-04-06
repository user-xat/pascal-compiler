#include "CSet.h"

bool CSet::contains(const EKeyWords keyword) const
{
	return m_set.find(keyword) != m_set.end();
}

CSet CSet::operator+(const CSet& other) const
{
	std::set<EKeyWords> result;
	result.insert(m_set.begin(), m_set.end());
	result.insert(other.m_set.begin(), other.m_set.end());
	return CSet(result);
}

//CSet& operator+(CSet& set, const EKeyWords keyword) {
//	set.m_set.insert(keyword);
//	return set;
//}
//
//CSet& operator+(const EKeyWords keyword, CSet& set) {
//	set.m_set.insert(keyword);
//	return set;
//}