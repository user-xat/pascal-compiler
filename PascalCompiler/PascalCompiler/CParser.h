#pragma once
#ifndef CPARSER_H
#define CPARSER_H

#include "CLexer.h"

class CParser {
private:
	CLexerPtr lexer_;
	CTokenPtr curToken_;
public:
	CParser(const std::string &filepath);
	void Parse();
private:
	void GetNextToken();
	void Accept(EKeyWords keyword);
	bool CheckTokenType(ETokenType type);
	bool CheckKeyword(EKeyWords keyword);
	bool CheckConstVariant(EVariantType variant);
	bool IsType();
	void Program();
	void Block();
	void TypeSection();
	void TypeDefinition();
	void VariableSection();
	void DescriptionOfVariables();
	void FunctionSection();
	void FunctionHeader();
	void FormalParametersSection();
	void ParametersGroup();
	void CompoundOperator();
	void Operator();
	void ConditionOperator();
	void AssignmentOperator();
	void LoopOperator();
	void LoopWithPrecondition();
	void LoopWithPostcondition();
	void LoopWithParameter();
	void Expression();
	bool RelationOperation();
	void SimpleExpression();
	void Term();
	bool MultiplicativeOperation();
	void Factor();
	void FunctionNotation();
	void ActualParameter();
};

#endif // !CPARSER_H

