#pragma once
#ifndef CLEXER_H
#define CLEXER_H

#include "CIOFile.h"
#include "CToken.h"
#include "CConstToken.h"
#include "CIdentToken.h"
#include "CKeywordToken.h"
#include <sstream>

class CLexer
{
private:
	CIOFilePtr file;
	std::string line;
	int ch_num;
public:
	CLexer(const std::string& path_to_file);
	CTokenPtr GetNextToken();
private:
	std::string GetWord();
	std::string GetNumber();
	bool GetNewLine();
	bool SkipSpaces();
	bool SkipComments();
	CTokenPtr ProccessingWord();
	bool IsLetter(const char ch);
	bool IsDigit(const char ch);
	bool IsWhiteSpace(const char ch);
	template<typename T>
	bool IsNumber(std::string num, T& value);
};

template<typename T>
bool CLexer::IsNumber(std::string num, T& value) {
	std::stringstream ss;
	ss << num;
	ss >> value;
	return ss.good();
}

#endif // !_CLEXER_H_
