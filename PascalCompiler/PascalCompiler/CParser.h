#pragma once
#ifndef CPARSER_H
#define CPARSER_H

#include "CLexer.h"
#include "CSet.h"
#include <set>

class CParser {
private:
	CLexerPtr m_lexer;
	CTokenPtr m_cur_token;

public:
	CParser(const std::string &filepath);
	void Parse();
private:
	bool GetNextToken();
	void Accept(ESymbol symbol);
	bool CheckTokenType(ESymbol type) const;
	bool CheckKeyword(ESymbol keyword) const;
	bool CheckConstVariant(ESymbol variant) const;
	void Type(const CSet& followers);
	bool Contains(const CSet& set) const;
	void BeginSkipErr(const CSet& starts, const CSet& followers);
	void EndSkipErr(const CSet& followers);
	void Program();
	void Block(const CSet& followers);
	void TypeSection(const CSet& followers);
	void TypeDefinition(const CSet& followers);
	void VariableSection(const CSet& followers);
	void DescriptionOfVariables(const CSet& followers);
	void FunctionSection(const CSet& followers);
	void FunctionHeader(const CSet& followers);
	void FormalParametersSection(const CSet& followers);
	void ParametersGroup(const CSet& followers);
	void CompoundStatement(const CSet& followers);
	void Statement(const CSet& followers);
	void ConditionStatement(const CSet& followers);
	void AssignmentStatement(const CSet& followers);
	void Variable(const CSet& followers);
	void LoopStatement(const CSet& followers);
	void LoopWithPrecondition(const CSet& followers);
	void LoopWithPostcondition(const CSet& followers);
	void LoopWithParameter(const CSet& followers);
	void Expression(const CSet& followers);
	bool RelationOperation(const CSet& followers);
	void SimpleExpression(const CSet& followers);
	void Term(const CSet& followers);
	bool MultiplicativeOperation(const CSet& followers);
	void Factor(const CSet& followers);
	void FunctionNotation(const CSet& followers);
	void ActualParameter();
	const std::map<ESymbol, std::string> m_key_words = {
		{ESymbol::INTEGER_KEYWORD, "integer"},
		{ESymbol::REAL_KEYWORD, "real"},
		{ESymbol::STRING_KEYWORD, "string"},
		{ESymbol::BOOLEAN_KEYWORD, "boolean"},
		{ESymbol::AOP_ASSIGN_KEYWORD, ":="},
		{ESymbol::AOP_SUM_KEYWORD, "+"},
		{ESymbol::AOP_SUB_KEYWORD, "-"},
		{ESymbol::AOP_MULT_KEYWORD, "*"},
		{ESymbol::AOP_DIV_KEYWORD, "div"},
		{ESymbol::AOP_DIV_REAL_KEYWORD, "/"},
		{ESymbol::AOP_MOD_KEYWORD, "mod"},
		{ESymbol::LOP_AND_KEYWORD, "and"},
		{ESymbol::LOP_OR_KEYWORD, "or"},
		{ESymbol::LOP_NOT_KEYWORD, "not"},
		{ESymbol::LOP_XOR_KEYWORD, "xor"},
		{ESymbol::COP_LT_KEYWORD, "<"},
		{ESymbol::COP_LE_KEYWORD, "<="},
		{ESymbol::COP_GT_KEYWORD, ">"},
		{ESymbol::COP_GE_KEYWORD, ">="},
		{ESymbol::COP_EQ_KEYWORD, "="},
		{ESymbol::COP_NE_KEYWORD, "<>"},
		{ESymbol::PROGRAM_KEYWORD, "program"},
		{ESymbol::SEMICOLON_KEYWORD, ";"},
		{ESymbol::BEGIN_KEYWORD, "begin"},
		{ESymbol::END_KEYWORD, "end"},
		{ESymbol::VAR_KEYWORD, "var"},
		{ESymbol::TYPE_KEYWORD, "type"},
		{ESymbol::IF_KEYWORD, "if"},
		{ESymbol::THEN_KEYWORD, "then"},
		{ESymbol::ELSE_KEYWORD, "else"},
		{ESymbol::WHILE_KEYWORD, "while"},
		{ESymbol::DO_KEYWORD, "do"},
		{ESymbol::COLON_KEYWORD, ":"},
		{ESymbol::OPENING_BRACKET_KEYWORD, "("},
		{ESymbol::CLOSING_BRACKET_KEYWORD, ")"},
		{ESymbol::SINGLE_QUOTE_KEYWORD, "'"},
		{ESymbol::COMMA_KEYWORD, ","},
		{ESymbol::DOT_KEYWORD, "."},
		{ESymbol::FUNCTION_KEYWORD, "function"},
		{ESymbol::FOR_KEYWORD, "for"},
		{ESymbol::TO_KEYWORD, "to"},
		{ESymbol::DOWNTO_KEYWORD, "downto"},
		{ESymbol::REPEAT_KEYWORD, "repeat"},
		{ESymbol::UNTIL_KEYWORD, "until"},
	};
};

#endif // !CPARSER_H

