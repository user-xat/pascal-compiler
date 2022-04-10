#pragma once
#ifndef CSET_H
#define CSET_H

#include <set>
#include "ESymbol.h"

class CSet
{
private:
	std::set<ESymbol> m_set;

public:
	CSet(const std::set<ESymbol> set) : m_set{ set } {}
	bool contains(const ESymbol symbol) const;
	CSet operator+(const CSet& other) const;
	/*friend CSet& operator+(CSet& set, const EKeyWords keyword);
	friend CSet& operator+(const EKeyWords keyword, CSet& set);*/
};

#endif // !CSET_H
