#pragma once
#ifndef CSET_H
#define CSET_H

#include <set>
#include "EKeyWords.h"

class CSet
{
private:
	std::set<EKeyWords> m_set;

public:
	CSet(const std::set<EKeyWords> set) : m_set{ set } {}
	bool contains(const EKeyWords keyword) const;
	CSet operator+(const CSet& other) const;
	/*friend CSet& operator+(CSet& set, const EKeyWords keyword);
	friend CSet& operator+(const EKeyWords keyword, CSet& set);*/
};

#endif // !ISET_H
