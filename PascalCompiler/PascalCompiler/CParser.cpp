#include "CParser.h"

CParser::CParser(const std::string& filepath) {
	m_lexer = std::make_unique<CLexer>(filepath);
}

void CParser::Parse()
{
	GetNextToken();
	Program();
}

bool CParser::GetNextToken() {
	m_cur_token = m_lexer->GetNextToken();
	return true;
}

void CParser::Accept(EKeyWords keyword)
{
	if (m_cur_token == nullptr) {
		throw "Current token is nullptr";
	}
	else if (m_cur_token->GetType() != ETokenType::KEYWORD) {
		m_lexer->ThrowError("Expected token is not keyword");
	}
	else if (dynamic_cast<CKeywordToken*>(m_cur_token.get())->GetKeyword() != keyword) {
		m_lexer->ThrowError("Expected token is " + m_cur_token->ToString());
	}
	GetNextToken();
}

void CParser::Accept(ETokenType type)
{
	if (m_cur_token == nullptr) {
		throw "Current token is nullptr";
	}
	else if (m_cur_token->GetType() != type) {
		std::string type_str;
		switch (type)
		{
		case ETokenType::IDENT:
			type_str = "identificator";
			break;
		case ETokenType::KEYWORD:
			type_str = "keyword";
			break;
		case ETokenType::CONST:
			type_str = "constant";
			break;
		}
		m_lexer->ThrowError("Expected token is not type of " + type_str);
	}
	GetNextToken();
}

bool CParser::CheckTokenType(ETokenType type)
{
	if (m_cur_token != nullptr) {
		return m_cur_token->GetType() == type;
	}
	return false;
}

bool CParser::CheckKeyword(EKeyWords keyword)
{
	return CheckTokenType(ETokenType::KEYWORD) && dynamic_cast<CKeywordToken*>(m_cur_token.get())->GetKeyword() == keyword;
}

bool CParser::CheckConstVariant(EVariantType variant)
{
	return CheckTokenType(ETokenType::CONST) && dynamic_cast<CConstToken*>(m_cur_token.get())->GetVariant()->GetType() == variant;
}

void CParser::Type() {
	if (m_cur_token == nullptr) {
		throw "Current token is nullptr";
	}
	else if (m_cur_token->GetType() != ETokenType::KEYWORD) {
		m_lexer->ThrowError("Expected token is not keyword");
	}
	else {
		CKeywordToken* token = dynamic_cast<CKeywordToken*>(m_cur_token.get());
		if (token->GetKeyword() != EKeyWords::INTEGER && token->GetKeyword() != EKeyWords::REAL
			&& token->GetKeyword() != EKeyWords::STRING && token->GetKeyword() != EKeyWords::BOOLEAN)
		{
			m_lexer->ThrowError("Expected token is not a type");
		}
		GetNextToken();
	}
}

// <program>:: = [program <name>;]<block>.
void CParser::Program()
{
	if (CheckKeyword(EKeyWords::PROGRAM)) {
		Accept(EKeyWords::PROGRAM);		// get program's name
		Accept(ETokenType::IDENT);
		Accept(EKeyWords::SEMICOLON);
	}
	Block();
	Accept(EKeyWords::DOT);
}

// <block>::=<type section><variable section><function section><compound statement>
void CParser::Block() {
	TypeSection();
	VariableSection();
	FunctionSection();
	CompoundStatement();
}

// <type section>::=<empty>|type<type definition>;{<type definition>;}
void CParser::TypeSection() {
	if (CheckKeyword(EKeyWords::TYPE)) {
		Accept(EKeyWords::TYPE);
		do
		{
			TypeDefinition();
			Accept(EKeyWords::SEMICOLON);
		} while (CheckTokenType(ETokenType::IDENT));
	}
}

// <type definition>::=<name>=<type>
void CParser::TypeDefinition() {
	Accept(ETokenType::IDENT);
	Accept(EKeyWords::COP_EQ);
	Type();
}

// <variable section>::= var <description of variables>;{<description of variables>;}|<empty>
void CParser::VariableSection() {
	if (CheckKeyword(EKeyWords::VAR)) {
		Accept(EKeyWords::VAR);
		do
		{
			DescriptionOfVariables();
			Accept(EKeyWords::SEMICOLON);
		} while (CheckTokenType(ETokenType::IDENT));
	}
}

// <description of variables>::=<name>{, <name>}:<type>
void CParser::DescriptionOfVariables() {
	Accept(ETokenType::IDENT);
	while (CheckKeyword(EKeyWords::COMMA)) {
		GetNextToken();
		Accept(ETokenType::IDENT);
	}
	Accept(EKeyWords::COLON);
	Type();
}

// <function section>::={function <function header><block>;}
void CParser::FunctionSection() {
	while (CheckKeyword(EKeyWords::FUNCTION)) {
		Accept(EKeyWords::FUNCTION);
		FunctionHeader();
		Block();
		Accept(EKeyWords::SEMICOLON);
	}
}

// <function header>::= <name>[(<formal parameters section>{; <formal parameters section>})]:<result type>;
void CParser::FunctionHeader() {
	Accept(ETokenType::IDENT);
	if (CheckKeyword(EKeyWords::OPENING_BRACKET)) {
		GetNextToken();
		do
		{
			FormalParametersSection();
		} while (CheckKeyword(EKeyWords::SEMICOLON) && GetNextToken());
		Accept(EKeyWords::CLOSING_BRACKET);
	}
	Accept(EKeyWords::COLON);
	Type();
	Accept(EKeyWords::SEMICOLON);
}

// <formal parameters section>::= <parameters group> |
// var <parameters group> | function <parameters group>
void CParser::FormalParametersSection() {
	if (CheckKeyword(EKeyWords::VAR) || CheckKeyword(EKeyWords::FUNCTION)) {
		GetNextToken();
	}
	ParametersGroup();
}

// <parameters group>::=<name>{,<name>}:<type>
void CParser::ParametersGroup() {
	do
	{
		Accept(ETokenType::IDENT);
	} while (CheckKeyword(EKeyWords::COMMA) && GetNextToken());
	Accept(EKeyWords::COLON);
	Type();
}

// <compound section>::= begin <statement>{;<statement>} end
void CParser::CompoundStatement() {
	Accept(EKeyWords::BEGIN);
	Statement();
	while (CheckKeyword(EKeyWords::SEMICOLON)) {
		GetNextToken();
		Statement();
	}
	Accept(EKeyWords::END);
}


// <statement>::=<assignment statement> | <compound statement> | 
// <condition statement> | <loop statement> | <empty statement>
void CParser::Statement() {
	if (CheckTokenType(ETokenType::IDENT)) {
		AssignmentStatement();
	}
	else if (CheckKeyword(EKeyWords::BEGIN)) {
		CompoundStatement();
	}
	else if (CheckKeyword(EKeyWords::IF)) {
		ConditionStatement();
	}
	else if (CheckKeyword(EKeyWords::FOR)
		|| CheckKeyword(EKeyWords::WHILE)
		|| CheckKeyword(EKeyWords::REPEAT))
	{
		LoopStatement();
	}
}

// <condition statement>::= if <exression> then <statement> [else <statement>]
void CParser::ConditionStatement() {
	Accept(EKeyWords::IF);
	Expression();
	Accept(EKeyWords::THEN);
	Statement();
	if (CheckKeyword(EKeyWords::ELSE)) {
		GetNextToken();
		Statement();
	}
}

// <assignment statement>::=<variable>:=<выражение> | <имя функции>:=<выражение>
void CParser::AssignmentStatement() {
	Variable();
	Accept(EKeyWords::AOP_ASSIGN);
	Expression();
}

// <переменная>::=<полная переменная>|<компонента переменной> | <указанная переменная>
// <обозначение функции>::=<имя функции>|<имя функции> (<фактический параметр>{, <фактический параметр>})
void CParser::Variable()
{
	Accept(ETokenType::IDENT);
	while (CheckKeyword(EKeyWords::OPENING_BRACKET)) {

	}
}

// <loop statement>::=<loop with precondition>|
//	<loop with postcontition> | <loop with parameter>
void CParser::LoopStatement() {
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

// <loop with precondition>::= while <expression> do <statement>
void CParser::LoopWithPrecondition() {
	Accept(EKeyWords::WHILE);
	Expression();
	Accept(EKeyWords::DO);
	Statement();
}

// <loop with postcontition>::= repeat <statement>{;<statement>} until <expression>
void CParser::LoopWithPostcondition() {
	Accept(EKeyWords::REPEAT);
	Statement();
	while (CheckKeyword(EKeyWords::SEMICOLON)) {
		GetNextToken();
		Statement();
	}
	Accept(EKeyWords::UNTIL);
	Expression();
}

// <loop with parameter>::= for <parameter name>:=<expression>
// <to|downto><expression> do <statement>
void CParser::LoopWithParameter() {
	Accept(EKeyWords::FOR);
	Accept(ETokenType::IDENT);
	Accept(EKeyWords::AOP_ASSIGN);
	Expression();
	if (CheckKeyword(EKeyWords::TO)) {
		Accept(EKeyWords::TO);
	}
	else if (CheckKeyword(EKeyWords::DOWNTO)) {
		Accept(EKeyWords::DOWNTO);
	}
	Expression();
	Accept(EKeyWords::DO);
	Statement();
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
	bool result = true;
	/*if (!CheckKeyword(EKeyWords::COP_EQ) && !CheckKeyword(EKeyWords::COP_NE)
		&& !CheckKeyword(EKeyWords::COP_LT) && !CheckKeyword(EKeyWords::COP_LE)
		&& !CheckKeyword(EKeyWords::COP_GE) && !CheckKeyword(EKeyWords::COP_GT))
	{
		result = false;
		m_lexer->ThrowError("Exepted token should be on of following: =, <>, <, <=, >=, >");
	}*/
	
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
	if (CheckKeyword(EKeyWords::AOP_SUM)) {
		GetNextToken();
	}
	else if (CheckKeyword(EKeyWords::AOP_SUB)) {
		GetNextToken();
	}
	Term();
	while (CheckKeyword(EKeyWords::AOP_SUM) || CheckKeyword(EKeyWords::AOP_SUB)
		|| CheckKeyword(EKeyWords::LOP_OR)) 
	{
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
		//if (dynamic_cast<CConstToken*>(m_cur_token.get())->GetVariant()->GetType() == EVariantType::BOOLEAN) {
		//	// TODO:
		//	// THROW EXCEPTION
		//}
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
		// <переменная> | <function notation>
		Variable();
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
	//std::string function_name = dynamic_cast<CIdentToken*>(m_cur_token.get())->GetIdentifier();
	Accept(ETokenType::IDENT);
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


// <actual parameter>::=<expression>|<variable>|<имя функции>
void CParser::ActualParameter() {

}
