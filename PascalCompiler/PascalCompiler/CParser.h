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
	void Type();
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
};

#endif // !CPARSER_H

