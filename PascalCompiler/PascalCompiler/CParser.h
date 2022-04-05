#pragma once
#ifndef CPARSER_H
#define CPARSER_H

#include "CLexer.h"

class CParser {
private:
	CLexerPtr m_lexer;
	CTokenPtr m_cur_token;

public:
	CParser(const std::string &filepath);
	void Parse();
private:
	bool GetNextToken();
	void Accept(EKeyWords keyword);
	void Accept(ETokenType type);
	bool CheckTokenType(ETokenType type);
	bool CheckKeyword(EKeyWords keyword);
	bool CheckConstVariant(EVariantType variant);
	void Type();
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
	void CompoundStatement();
	void Statement();
	void ConditionStatement();
	void AssignmentStatement();
	void Variable();
	void LoopStatement();
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

