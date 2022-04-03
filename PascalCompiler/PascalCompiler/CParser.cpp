#include "CParser.h"

CParser::CParser(const std::string &filepath) {
	m_lexer = std::make_unique<CLexer>(filepath);
}

void CParser::Parse()
{
	GetNextToken();
	Program();
}

void CParser::GetNextToken() {
	m_curToken = m_lexer->GetNextToken();
}

void CParser::Accept(EKeyWords keyword)
{
	if (m_curToken == nullptr || m_curToken->GetType() != ETokenType::KEYWORD
		|| dynamic_cast<CKeywordToken*>(m_curToken.get())->GetKeyword() != keyword) {
			// TODO:
			// THROW EXCEPTION
	}
	GetNextToken();
}

bool CParser::CheckTokenType(ETokenType type)
{
	if (m_curToken != nullptr) {
		return m_curToken->GetType() == type;
	}
	return false;
}

bool CParser::CheckKeyword(EKeyWords keyword)
{
	return CheckTokenType(ETokenType::KEYWORD) && dynamic_cast<CKeywordToken*>(m_curToken.get())->GetKeyword() == keyword;
}

bool CParser::CheckConstVariant(EVariantType variant)
{
	return CheckTokenType(ETokenType::CONST) && dynamic_cast<CConstToken*>(m_curToken.get())->GetVariant()->GetType() == variant;
}

bool CParser::IsType() {
	if (m_curToken != nullptr && m_curToken->GetType() == ETokenType::KEYWORD) {
		CKeywordToken* token = dynamic_cast<CKeywordToken*>(m_curToken.get());
		return token->GetKeyword() == EKeyWords::INTEGER || token->GetKeyword() == EKeyWords::REAL
			|| token->GetKeyword() == EKeyWords::STRING || token->GetKeyword() == EKeyWords::BOOLEAN;
	}
	return false;
}

// <program>:: = program <name>;<block>.
void CParser::Program()
{
	if (CheckKeyword(EKeyWords::PROGRAM)) {
		GetNextToken();		// get program's name
		//GetNextToken();		// get end of statement
		if (!CheckTokenType(ETokenType::IDENT)) {
			// TODO:
			// THROW EXCEPTION
		}
		Accept(EKeyWords::END_OF_STATEMENT);
		Block();
		Accept(EKeyWords::DOT);
	}
}

// <block>::=<type section><variable section><function section><operator section>
void CParser::Block() {
	TypeSection();
	VariableSection();
	FunctionSection();
	CompoundOperator();
}

// <type section>::=<empty>|type<type definition>;{<type definition>;}
void CParser::TypeSection() {
	if (CheckKeyword(EKeyWords::TYPE)) {
		GetNextToken();
		do
		{
			TypeDefinition();
			Accept(EKeyWords::END_OF_STATEMENT);
		} while (CheckTokenType(ETokenType::IDENT));
	}
}

// <type definition>::=<name>=<type>
void CParser::TypeDefinition() {
	if (CheckTokenType(ETokenType::IDENT)) {
		std::string typesName = dynamic_cast<CIdentToken*>(m_curToken.get())->GetIdentifier();
		GetNextToken();
		Accept(EKeyWords::COP_EQ);
		IsType();
		GetNextToken();
	}
}

// <variable section>::= var <description of variables>;{<description of variables>;}|<empty>
void CParser::VariableSection() {
	if (CheckKeyword(EKeyWords::VAR)) {
		GetNextToken();
		do
		{
			DescriptionOfVariables();
			Accept(EKeyWords::END_OF_STATEMENT);
		} while (CheckTokenType(ETokenType::IDENT));
	}
}

// <description of variables>::=<name>{, <name>}:<type>
void CParser::DescriptionOfVariables() {
	std::string name;
	do
	{
		if (CheckTokenType(ETokenType::IDENT)) {
			name = dynamic_cast<CIdentToken*>(m_curToken.get())->GetIdentifier();
		}
		GetNextToken();
		if (CheckKeyword(EKeyWords::COMMA)) {
			GetNextToken();
		}
		else {
			break;
		}
	} while (true);
	Accept(EKeyWords::COLON);
	if (!IsType()) {
		// TODO:
		// THROW EXCEPTION
	}
}

// <function section>::={function <function header><block>;}
void CParser::FunctionSection() {
	while (CheckKeyword(EKeyWords::FUNCTION)) {
		GetNextToken();
		FunctionHeader();
		Block();
		Accept(EKeyWords::END_OF_STATEMENT);
	}
}

// <function header>::= <name>:<result type>; |
// <name>(<formal parameters section>{; <formal parameters section>}) :<result type>;
void CParser::FunctionHeader() {
	std::string name;
	if (!CheckTokenType(ETokenType::IDENT)) {
		// TODO:
		// THROW EXCEPTION
	}
	GetNextToken();
	if (CheckKeyword(EKeyWords::OPENING_BRACKET)) {
		GetNextToken();
		do
		{
			FormalParametersSection();
			if (CheckKeyword(EKeyWords::END_OF_STATEMENT))
				GetNextToken();
			else
				break;
		} while (true);
		Accept(EKeyWords::CLOSING_BRACKET);
	}
	Accept(EKeyWords::COLON);
	if (!IsType()) {
		// TODO:
		// THROW EXCEPTION
	}
	Accept(EKeyWords::END_OF_STATEMENT);
}

// <formal parameters section>::= <parameters group> |
// var <parameters group> | function <parameters group>
void CParser::FormalParametersSection() {
	if (CheckKeyword(EKeyWords::VAR)) {
		GetNextToken();
	}
	else if (CheckKeyword(EKeyWords::FUNCTION)) {
		GetNextToken();
	}	
	ParametersGroup();
}

// <parameters group>::=<name>{,<name>}:<type>
void CParser::ParametersGroup() {
	std::string name;
	do
	{
		if (!CheckTokenType(ETokenType::IDENT)) {
			// TODO:
			// THROW EXCEPTION
		}
		name = dynamic_cast<CIdentToken*>(m_curToken.get())->GetIdentifier();
		GetNextToken();
		if (CheckKeyword(EKeyWords::COMMA))
			GetNextToken();
		else
			break;
	} while (true);
	Accept(EKeyWords::COLON);
	if (!IsType()) {
		// TODO:
		// THROW EXCEPTION
	}
}

// <compound section>::= begin <operator>{;<operator>} end
void CParser::CompoundOperator() {
	Accept(EKeyWords::BEGIN);
	Operator();
	while (CheckKeyword(EKeyWords::END_OF_STATEMENT)) {
		Operator();
	}
	Accept(EKeyWords::END);
}


// <operator>::=<assignment operator> | <compound operator> | 
// <condition operator> | <loop operator> | <empty operator>
void CParser::Operator() {
	if (CheckTokenType(ETokenType::IDENT)) {
		AssignmentOperator();
	}
	else if (CheckKeyword(EKeyWords::BEGIN)) {
		CompoundOperator();
	}
	else if (CheckKeyword(EKeyWords::IF)) {
		ConditionOperator();
	}
	else if (CheckKeyword(EKeyWords::FOR) 
		|| CheckKeyword(EKeyWords::WHILE) 
		|| CheckKeyword(EKeyWords::REPEAT)) {
		LoopOperator();
	}
}

// <condition operator>::= if <exression> then <operator>|
//	if <exression> then <operator> else <operator>
void CParser::ConditionOperator() {
	Accept(EKeyWords::IF);
	Expression();
	Accept(EKeyWords::THEN);
	Operator();
	if (CheckKeyword(EKeyWords::ELSE)) {
		Operator();
	}
}

// <assignment operator>::=<переменная>:=<выражение> | <имя функции>:=<выражение>
void CParser::AssignmentOperator() {
	std::string variable = dynamic_cast<CIdentToken*>(m_curToken.get())->GetIdentifier();
	GetNextToken();
	Accept(EKeyWords::AOP_ASSIGN);
	Expression();
}

// <loop operator>::=<loop with precondition>|
//	<loop with postcontition> | <loop with parameter>
void CParser::LoopOperator() {
	if (CheckKeyword(EKeyWords::FOR)) {
		LoopWithParameter();
	}
	else if (CheckKeyword(EKeyWords::WHILE)) {
		LoopWithPrecondition();
	}
	else if (CheckKeyword(EKeyWords::REPEAT)) {
		LoopWithPostcondition();
	}
}

// <loop with precondition>::= while <expression> do <operator>
void CParser::LoopWithPrecondition() {
	Accept(EKeyWords::WHILE);
	Expression();
	Accept(EKeyWords::DO);
	Operator();
}

// <loop with postcontition>::= repeat <operator>{;<operator>} until <expression>
void CParser::LoopWithPostcondition() {
	Accept(EKeyWords::REPEAT);
	Operator();
	while (CheckKeyword(EKeyWords::END_OF_STATEMENT)) {
		GetNextToken();
		Operator();
	}
	Accept(EKeyWords::UNTIL);
	Expression();
}

// <loop with parameter>::= for <parameter name>:=<expression>
// <to|downto><expression> do <operator>
void CParser::LoopWithParameter() {
	Accept(EKeyWords::FOR);
	if (!CheckTokenType(ETokenType::IDENT)) {
		// TODO:
		// THROW EXCEPTION
	}
	Accept(EKeyWords::AOP_ASSIGN);
	Expression();
	if (!CheckKeyword(EKeyWords::TO) || !CheckKeyword(EKeyWords::DOWNTO)) {
		// TODO:
		// THROW EXCEPTION
	}
	GetNextToken();
	Accept(EKeyWords::DO);
	Operator();
}

// <expression>::=<simple expression>{<relation operation><simple expression>}
void CParser::Expression() {
	SimpleExpression();
	while (RelationOperation()) {
		GetNextToken();
		SimpleExpression();
	}
}

// <relation operation>::= = | <> | < | <= | >= | >
bool CParser::RelationOperation() {
	bool result = false;
	if (CheckKeyword(EKeyWords::COP_EQ)) {
		result = true;
	}
	else if (CheckKeyword(EKeyWords::COP_NE)) {
		result = true;
	}
	else if (CheckKeyword(EKeyWords::COP_LT)) {
		result = true;
	}
	else if (CheckKeyword(EKeyWords::COP_LE)) {
		result = true;
	}
	else if (CheckKeyword(EKeyWords::COP_GE)) {
		result = true;
	}
	else if (CheckKeyword(EKeyWords::COP_GT)) {
		result = true;
	}
	return result;
}


// <simple expression>::=<+|-><term> {<+|-|or><term>}
void CParser::SimpleExpression() {
	Term();
	while (CheckKeyword(EKeyWords::AOP_SUM) || CheckKeyword(EKeyWords::AOP_SUB)
		|| CheckKeyword(EKeyWords::LOP_OR)) {
		GetNextToken();
		Term();
	}
}

// <term>::=<factor>{<multiplicative operation><factor>}
void CParser::Term() {
	Factor();
	while (MultiplicativeOperation()) {
		GetNextToken();
		Factor();
	}
}

// <multiplicative operation>::= *|/|div|mod|and
bool CParser::MultiplicativeOperation() {
	bool result = false;
	if (CheckKeyword(EKeyWords::AOP_MULT)) {
		result = true;
	}
	else if (CheckKeyword(EKeyWords::AOP_DIV)) {
		result = true;
	}
	else if (CheckKeyword(EKeyWords::LOP_AND)) { // div
		result = true;
	}
	else if (CheckKeyword(EKeyWords::LOP_AND)) { // mod
		result = true;
	}
	else if (CheckKeyword(EKeyWords::LOP_AND)) {
		result = true;
	}
	return result;
}

// <factor>::=<переменная>|<константа без знака>|
//	(<expression>) | <обозначение функции> | not <factor>
void CParser::Factor() {
	if (CheckTokenType(ETokenType::CONST)) {
		if (dynamic_cast<CConstToken*>(m_curToken.get())->GetVariant()->GetType() == EVariantType::BOOLEAN) {
			// TODO:
			// THROW EXCEPTION
		}
	}
	else if (CheckKeyword(EKeyWords::OPENING_BRACKET)) {
		GetNextToken();
		Expression();
		Accept(EKeyWords::CLOSING_BRACKET);
	}
	else if (CheckKeyword(EKeyWords::LOP_NOT)) {
		GetNextToken();
		Factor();
	}
	else {
		// <переменная> | <обозначение функции>
	}
}
// <переменная>:: = <имя>
// <константа без знака>::=<число без знака>|<строка>


// <число без знака>::=<целое без знака>|
// <вещественное без знака>
// <целое без знака>:: = <цифра>{ <цифра> }
// <вещественное без знака>:: = <целое без знака>.<цифра>{<цифра>} |
// <целое без знака>.<цифра>{<цифра>}E<порядок> |
// <целое без знака>E<порядок>

// <строка>::='<символ>{<символ>}'
 

// <function notation>::=<function name>|<function name>(<actual parameter>{, <actual parameter>})
void CParser::FunctionNotation() {
	std::string function_name = dynamic_cast<CIdentToken*>(m_curToken.get())->GetIdentifier();
	GetNextToken();
	if (CheckKeyword(EKeyWords::OPENING_BRACKET)) {
		GetNextToken();
		Expression();
		while (CheckKeyword(EKeyWords::COMMA)) {
			GetNextToken();
			Expression();
		}
		Accept(EKeyWords::CLOSING_BRACKET);
	}
}

