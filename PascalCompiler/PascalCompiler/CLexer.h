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
	CIOFilePtr m_file;
	std::string m_line;
	int m_cur_sym;
	int m_start_token;
public:
	CLexer(const std::string &path_to_file);
	CTokenPtr GetNextToken();
	void ThrowError(const std::string& error);

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
	std::string GenerateErrorMessage(int line_num);
	const std::map<std::string, EKeyWords> key_words = {
		{"integer", EKeyWords::INTEGER},
		{"real", EKeyWords::REAL},
		{"string", EKeyWords::STRING},
		{"boolean", EKeyWords::BOOLEAN},
		{":=", EKeyWords::AOP_ASSIGN},
		{"+", EKeyWords::AOP_SUM},
		{"-", EKeyWords::AOP_SUB},
		{"*", EKeyWords::AOP_MULT},
		{"div", EKeyWords::AOP_DIV},
		{"/", EKeyWords::AOP_DIV_REAL},
		{"mod", EKeyWords::AOP_MOD},
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
		{"program", EKeyWords::PROGRAM},
		{";", EKeyWords::SEMICOLON},
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
		{".", EKeyWords::DOT},
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

using CLexerPtr = std::unique_ptr<CLexer>;

#endif // !_CLEXER_H_
