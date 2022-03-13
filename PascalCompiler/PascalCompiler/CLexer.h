#pragma once
#ifndef CLEXER_H
#define CLEXER_H

#include "CIOFile.h"
#include "CToken.h"
#include "CConstToken.h"
#include "CIdentToken.h"
#include "CKeywordToken.h"
#include <sstream>
#include <map>

class CLexer
{
private:
	CIOFilePtr file;
	std::string line;
	int ch_num;
public:
	CLexer(const std::string &path_to_file);
	CTokenPtr GetNextToken();
private:
	std::string GetWord();
	std::string GetNumber();
	bool GetNewLine();
	bool SkipSpaces();
	bool SkipComments();
	CTokenPtr ProcessingKeyWord(const std::string& word);
	bool IsLetter(const char ch);
	bool IsDigit(const char ch);
	bool IsWhiteSpace(const char ch);
	template<typename T>
	bool IsNumber(std::string num, T& value);
	const std::map<std::string, EKeyWords> key_words = {
		{"integer", EKeyWords::INTEGER},
		{"real", EKeyWords::REAL},
		{"string", EKeyWords::STRING},
		{"boolean", EKeyWords::BOOLEAN},
		{":=", EKeyWords::AOP_ASSIGN},
		{"+", EKeyWords::AOP_SUM},
		{"-", EKeyWords::AOP_SUB},
		{"*", EKeyWords::AOP_MULT},
		{"/", EKeyWords::AOP_DIV},
		{"and", EKeyWords::LOP_AND},
		{"or", EKeyWords::LOP_OR},
		{"not", EKeyWords::LOP_NOT},
		{"xor", EKeyWords::LOP_XOR},
		{"<", EKeyWords::COP_LT},
		{"<=", EKeyWords::COP_LE},
		{">", EKeyWords::COP_GT},
		{">=", EKeyWords::COP_GE},
		{"=", EKeyWords::COP_EQ},
		{"<>", EKeyWords::COP_NE},
		{";", EKeyWords::END_OF_STATEMENT},
		{"begin", EKeyWords::BEGIN},
		{"end", EKeyWords::END},
		{"var", EKeyWords::VAR},
		{"type", EKeyWords::TYPE},
		{"if", EKeyWords::IF},
		{"then", EKeyWords::THEN},
		{"else", EKeyWords::ELSE},
		{"while", EKeyWords::WHILE},
		{"do", EKeyWords::DO},
		{":", EKeyWords::COLON},
		{"(", EKeyWords::OPENING_BRACKET},
		{")", EKeyWords::CLOSING_BRACKET},
		{"'", EKeyWords::SINGLE_QUOTE},
		{",", EKeyWords::COMMA},
		{"function", EKeyWords::FUNCTION},
		{"for", EKeyWords::FOR},
		{"to", EKeyWords::TO},
		{"downto", EKeyWords::DOWNTO},
		{"repeat", EKeyWords::REPEAT},
		{"until", EKeyWords::UNTIL},
	};
};

template<typename T>
bool CLexer::IsNumber(std::string num, T& value) {
	std::stringstream ss;
	ss << num;
	ss >> value;
	return !ss.fail();
}

#endif // !_CLEXER_H_
