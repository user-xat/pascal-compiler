#pragma once
#ifndef _CLEXER_H_
#define _CLEXER_H_

#include "CIOFile.h"
#include "CToken.h"

class CLexer
{
private:
	CIOFilePtr file;
	std::string line;
	int ch_num;
public:
	CLexer(const std::string& path_to_file);
	CToken GetNextToken();
};

#endif // !_CLEXER_H_
