#pragma once
#ifndef CLEXER_H
#define CLEXER_H

#include "CIOFile.h"
#include "CToken.h"
#include "CConstToken.h"
#include "CIdentToken.h"
#include "CKeywordToken.h"
#include "ESymbol.h"
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
	const std::map<std::string, ESymbol> key_words = {
		{"integer", ESymbol::INTEGER_KEYWORD},
		{"real", ESymbol::REAL_KEYWORD},
		{"string", ESymbol::STRING_KEYWORD},
		{"boolean", ESymbol::BOOLEAN_KEYWORD},
		{":=", ESymbol::AOP_ASSIGN_KEYWORD},
		{"+", ESymbol::AOP_SUM_KEYWORD},
		{"-", ESymbol::AOP_SUB_KEYWORD},
		{"*", ESymbol::AOP_MULT_KEYWORD},
		{"div", ESymbol::AOP_DIV_KEYWORD},
		{"/", ESymbol::AOP_DIV_REAL_KEYWORD},
		{"mod", ESymbol::AOP_MOD_KEYWORD},
		{"and", ESymbol::LOP_AND_KEYWORD},
		{"or", ESymbol::LOP_OR_KEYWORD},
		{"not", ESymbol::LOP_NOT_KEYWORD},
		{"xor", ESymbol::LOP_XOR_KEYWORD},
		{"<", ESymbol::COP_LT_KEYWORD},
		{"<=", ESymbol::COP_LE_KEYWORD},
		{">", ESymbol::COP_GT_KEYWORD},
		{">=", ESymbol::COP_GE_KEYWORD},
		{"=", ESymbol::COP_EQ_KEYWORD},
		{"<>", ESymbol::COP_NE_KEYWORD},
		{"program", ESymbol::PROGRAM_KEYWORD},
		{";", ESymbol::SEMICOLON_KEYWORD},
		{"begin", ESymbol::BEGIN_KEYWORD},
		{"end", ESymbol::END_KEYWORD},
		{"var", ESymbol::VAR_KEYWORD},
		{"type", ESymbol::TYPE_KEYWORD},
		{"if", ESymbol::IF_KEYWORD},
		{"then", ESymbol::THEN_KEYWORD},
		{"else", ESymbol::ELSE_KEYWORD},
		{"while", ESymbol::WHILE_KEYWORD},
		{"do", ESymbol::DO_KEYWORD},
		{":", ESymbol::COLON_KEYWORD},
		{"(", ESymbol::OPENING_BRACKET_KEYWORD},
		{")", ESymbol::CLOSING_BRACKET_KEYWORD},
		{"'", ESymbol::SINGLE_QUOTE_KEYWORD},
		{",", ESymbol::COMMA_KEYWORD},
		{".", ESymbol::DOT_KEYWORD},
		{"function", ESymbol::FUNCTION_KEYWORD},
		{"for", ESymbol::FOR_KEYWORD},
		{"to", ESymbol::TO_KEYWORD},
		{"downto", ESymbol::DOWNTO_KEYWORD},
		{"repeat", ESymbol::REPEAT_KEYWORD},
		{"until", ESymbol::UNTIL_KEYWORD},
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
