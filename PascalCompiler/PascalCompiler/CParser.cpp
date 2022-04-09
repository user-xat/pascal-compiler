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

void CParser::Accept(ESymbol symbol)
{
	if (m_cur_token == nullptr) {
		throw "Current token is nullptr";
	}
	else if ((m_cur_token->GetType() != ESymbol::IDENT_TOKEN
		|| m_cur_token->GetType() != ESymbol::CONST_TOKEN)
		&& m_cur_token->GetType() != ESymbol::KEYWORD_TOKEN) {
		if (m_cur_token->GetType() != symbol) {
			std::string type_str;
			switch (symbol)
			{
			case ESymbol::IDENT_TOKEN:
				type_str = "identificator";
				break;
			case ESymbol::KEYWORD_TOKEN:
				type_str = "keyword";
				break;
			case ESymbol::CONST_TOKEN:
				type_str = "constant";
				break;
			}
			m_lexer->ThrowError("Expected token is not type of " + type_str);
		}
	}
	else if (dynamic_cast<CKeywordToken*>(m_cur_token.get())->GetKeyword() != symbol) {
		m_lexer->ThrowError("Expected token is " + m_cur_token->ToString());
	}
	GetNextToken();
}

bool CParser::CheckTokenType(ESymbol type) const
{
	if (m_cur_token != nullptr) {
		return m_cur_token->GetType() == type;
	}
	return false;
}

bool CParser::CheckKeyword(ESymbol keyword) const
{
	return CheckTokenType(ESymbol::KEYWORD_TOKEN) && dynamic_cast<CKeywordToken*>(m_cur_token.get())->GetKeyword() == keyword;
}

bool CParser::CheckConstVariant(ESymbol variant) const
{
	return CheckTokenType(ESymbol::CONST_TOKEN) && dynamic_cast<CConstToken*>(m_cur_token.get())->GetVariant()->GetType() == variant;
}

void CParser::Type() {
	if (m_cur_token == nullptr) {
		throw "Current token is nullptr";
	}
	else if (m_cur_token->GetType() != ESymbol::KEYWORD_TOKEN) {
		m_lexer->ThrowError("Expected token is not keyword");
	}
	else {
		CKeywordToken* token = dynamic_cast<CKeywordToken*>(m_cur_token.get());
		if (token->GetKeyword() != ESymbol::INTEGER_KEYWORD && token->GetKeyword() != ESymbol::REAL_KEYWORD
			&& token->GetKeyword() != ESymbol::STRING_KEYWORD && token->GetKeyword() != ESymbol::BOOLEAN_KEYWORD)
		{
			m_lexer->ThrowError("Expected token is not a type");
		}
		GetNextToken();
	}
}

bool CParser::Contains(const CSet& set) const {
	if (CheckTokenType(ESymbol::KEYWORD_TOKEN)) {
		return set.contains(dynamic_cast<CKeywordToken*>(m_cur_token.get())->GetKeyword());
	}
	else {
		return m_cur_token != nullptr && set.contains(m_cur_token->GetType());
	}
}

void CParser::BeginSkipErr(const CSet& starts, const CSet& followers) {
	if (!Contains(starts) && !Contains(followers))
	{
		m_lexer->ThrowError("Unexpected token");
		while (!Contains(starts) && !Contains(followers)) {
			GetNextToken();
		}
	}
}

void CParser::EndSkipErr(const CSet& followers) {
	if (!Contains(followers)) {
		m_lexer->ThrowError("Unexpected token");
		while (!Contains(followers)) {
			GetNextToken();
		}
	}
}

// <program>:: = [program <name>;]<block>.
void CParser::Program()
{
	if (CheckKeyword(ESymbol::PROGRAM_KEYWORD)) {
		Accept(ESymbol::PROGRAM_KEYWORD);		// get program's name
		Accept(ESymbol::IDENT_TOKEN);
		Accept(ESymbol::SEMICOLON_KEYWORD);
	}
	Block(CSet(std::set<ESymbol> {ESymbol::DOT_KEYWORD}));
	Accept(ESymbol::DOT_KEYWORD);
}

// <block>::=<type section><variable section><function section><compound statement>
void CParser::Block(const CSet& followers) {
	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::TYPE_KEYWORD, ESymbol::VAR_KEYWORD, ESymbol::FUNCTION_KEYWORD, ESymbol::BEGIN_KEYWORD}), followers);
	TypeSection(followers + CSet(std::set<ESymbol> {ESymbol::VAR_KEYWORD, ESymbol::FUNCTION_KEYWORD, ESymbol::BEGIN_KEYWORD}));
	VariableSection(followers + CSet(std::set<ESymbol> {ESymbol::FUNCTION_KEYWORD, ESymbol::BEGIN_KEYWORD}));
	FunctionSection(followers + CSet(std::set<ESymbol> {ESymbol::BEGIN_KEYWORD}));
	CompoundStatement(followers);
	EndSkipErr(followers);
}

// <type section>::=<empty>|type<type definition>;{<type definition>;}
void CParser::TypeSection(const CSet& followers) {
	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::TYPE_KEYWORD}), followers);
	if (Contains(CSet(std::set<ESymbol> {ESymbol::TYPE_KEYWORD}))) {
		Accept(ESymbol::TYPE_KEYWORD);
		do
		{
			TypeDefinition(followers + CSet(std::set<ESymbol> {ESymbol::SEMICOLON_KEYWORD}));
			Accept(ESymbol::SEMICOLON_KEYWORD);
		} while (CheckTokenType(ESymbol::IDENT_TOKEN));
		EndSkipErr(followers);
	}
}

// <type definition>::=<name>=<type>
void CParser::TypeDefinition(const CSet& followers) {
	BeginSkipErr(std::set<ESymbol> {ESymbol::IDENT_TOKEN}, followers);
	if (CheckTokenType(ESymbol::IDENT_TOKEN)) {
		Accept(ESymbol::IDENT_TOKEN);
		Accept(ESymbol::COP_EQ_KEYWORD);
		Type();
		EndSkipErr(followers);
	}
}

// <variable section>::= var <description of variables>;{<description of variables>;}|<empty>
void CParser::VariableSection(const CSet& followers) {
	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::VAR_KEYWORD}), followers);
	if (Contains(CSet(std::set<ESymbol> {ESymbol::VAR_KEYWORD}))) {
		Accept(ESymbol::VAR_KEYWORD);
		do
		{
			DescriptionOfVariables(followers + CSet(std::set<ESymbol> {ESymbol::SEMICOLON_KEYWORD}));
			Accept(ESymbol::SEMICOLON_KEYWORD);
		} while (CheckTokenType(ESymbol::IDENT_TOKEN));
		EndSkipErr(followers);
	}
}

// <description of variables>::=<name>{, <name>}:<type>
void CParser::DescriptionOfVariables(const CSet& followers) {
	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::IDENT_TOKEN}), followers);
	if (CheckTokenType(ESymbol::IDENT_TOKEN)) {
		Accept(ESymbol::IDENT_TOKEN);
		while (CheckKeyword(ESymbol::COMMA_KEYWORD)) {
			GetNextToken();
			Accept(ESymbol::IDENT_TOKEN);
		}
		Accept(ESymbol::COLON_KEYWORD);
		Type();
		EndSkipErr(followers);
	}
}

// <function section>::={function <function header><block>;}
void CParser::FunctionSection(const CSet& followers) {
	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::FUNCTION_KEYWORD}), followers);
	while (Contains(CSet(std::set<ESymbol> {ESymbol::FUNCTION_KEYWORD}))) {
		Accept(ESymbol::FUNCTION_KEYWORD);
		FunctionHeader(followers + CSet(std::set<ESymbol> {ESymbol::TYPE_KEYWORD, ESymbol::VAR_KEYWORD, ESymbol::FUNCTION_KEYWORD, ESymbol::BEGIN_KEYWORD}));
		Block(std::set<ESymbol> {ESymbol::SEMICOLON_KEYWORD});
		Accept(ESymbol::SEMICOLON_KEYWORD);
		BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::FUNCTION_KEYWORD}), followers);
	}
}

// <function header>::= <name>[(<formal parameters section>{; <formal parameters section>})]:<result type>;
void CParser::FunctionHeader(const CSet& followers) {
	BeginSkipErr(std::set<ESymbol> {ESymbol::IDENT_TOKEN}, followers);
	if (CheckTokenType(ESymbol::IDENT_TOKEN)) {
		Accept(ESymbol::IDENT_TOKEN);
		if (CheckKeyword(ESymbol::OPENING_BRACKET_KEYWORD)) {
			GetNextToken();
			do
			{
				FormalParametersSection(followers + CSet(std::set<ESymbol> {ESymbol::SEMICOLON_KEYWORD, ESymbol::CLOSING_BRACKET_KEYWORD}));
			} while (CheckKeyword(ESymbol::SEMICOLON_KEYWORD) && GetNextToken());
			Accept(ESymbol::CLOSING_BRACKET_KEYWORD);
		}
		Accept(ESymbol::COLON_KEYWORD);
		Type();
		Accept(ESymbol::SEMICOLON_KEYWORD);
		EndSkipErr(followers);
	}
}

// НЕ РАБОТАЕТ НЕЙТРАЛИЗАЦИЯ СИНТАКСИЧЕСКИХ ОШИБОК
// <formal parameters section>::= <parameters group> |
// var <parameters group> | function <parameters group>
void CParser::FormalParametersSection(const CSet& followers) {
	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::VAR_KEYWORD, ESymbol::FUNCTION_KEYWORD}), followers);
	if (Contains(CSet(std::set<ESymbol> {ESymbol::VAR_KEYWORD, ESymbol::FUNCTION_KEYWORD}))) {
		GetNextToken();
	}
	BeginSkipErr(std::set<ESymbol> {ESymbol::IDENT_TOKEN}, followers);
	ParametersGroup(followers);
}

// <parameters group>::=<name>{,<name>}:<type>
void CParser::ParametersGroup(const CSet& followers) {
	BeginSkipErr(std::set<ESymbol> {ESymbol::IDENT_TOKEN}, followers);
	if (CheckTokenType(ESymbol::IDENT_TOKEN)) {
		do
		{
			Accept(ESymbol::IDENT_TOKEN);
		} while (CheckKeyword(ESymbol::COMMA_KEYWORD) && GetNextToken());
		Accept(ESymbol::COLON_KEYWORD);
		Type();
		EndSkipErr(followers);
	}
}

// <compound section>::= begin <statement>{;<statement>} end
void CParser::CompoundStatement(const CSet& followers) {
	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::BEGIN_KEYWORD}), followers);
	if (Contains(CSet(std::set<ESymbol> {ESymbol::BEGIN_KEYWORD}))) {
		Accept(ESymbol::BEGIN_KEYWORD);
		Statement(followers + CSet(std::set<ESymbol> {ESymbol::SEMICOLON_KEYWORD, ESymbol::END_KEYWORD}));
		while (CheckKeyword(ESymbol::SEMICOLON_KEYWORD)) {
			GetNextToken();
			Statement(followers + CSet(std::set<ESymbol> {ESymbol::SEMICOLON_KEYWORD, ESymbol::END_KEYWORD}));
		}
		Accept(ESymbol::END_KEYWORD);
		EndSkipErr(followers);
	}
}


// <statement>::=<assignment statement> | <compound statement> | 
// <condition statement> | <loop statement> | <empty statement>
void CParser::Statement(const CSet& followers) {
	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::IDENT_TOKEN, ESymbol::BEGIN_KEYWORD, ESymbol::IF_KEYWORD, ESymbol::FOR_KEYWORD, ESymbol::WHILE_KEYWORD, ESymbol::REPEAT_KEYWORD}),
		followers);
	if (Contains(CSet(std::set<ESymbol> {ESymbol::IDENT_TOKEN, ESymbol::BEGIN_KEYWORD, ESymbol::IF_KEYWORD, ESymbol::FOR_KEYWORD, ESymbol::WHILE_KEYWORD, ESymbol::REPEAT_KEYWORD})))
	{
		if (CheckTokenType(ESymbol::IDENT_TOKEN)) {
			AssignmentStatement(followers);
		}
		else if (CheckKeyword(ESymbol::BEGIN_KEYWORD)) {
			CompoundStatement(followers);
		}
		else if (CheckKeyword(ESymbol::IF_KEYWORD)) {
			ConditionStatement(followers);
		}
		else if (CheckKeyword(ESymbol::FOR_KEYWORD)
			|| CheckKeyword(ESymbol::WHILE_KEYWORD)
			|| CheckKeyword(ESymbol::REPEAT_KEYWORD))
		{
			LoopStatement(followers);
		}
		EndSkipErr(followers);
	}
}

// <condition statement>::= if <exression> then <statement> [else <statement>]
void CParser::ConditionStatement(const CSet& followers) {
	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::IF_KEYWORD}), followers);
	if (Contains(CSet(std::set<ESymbol> {ESymbol::IF_KEYWORD})))
	{
		Accept(ESymbol::IF_KEYWORD);
		Expression(followers + CSet(std::set<ESymbol> {ESymbol::THEN_KEYWORD, ESymbol::ELSE_KEYWORD}));
		Accept(ESymbol::THEN_KEYWORD);
		Statement(followers + CSet(std::set<ESymbol> {ESymbol::ELSE_KEYWORD}));
		if (CheckKeyword(ESymbol::ELSE_KEYWORD)) {
			GetNextToken();
			Statement(followers);
		}
		EndSkipErr(followers);
	}
}

// <assignment statement>::=<variable>:=<выражение> | <имя функции>:=<выражение>
void CParser::AssignmentStatement(const CSet& followers) {
	Variable(followers + CSet(std::set<ESymbol> {ESymbol::AOP_ASSIGN_KEYWORD}));
	Accept(ESymbol::AOP_ASSIGN_KEYWORD);
	Expression(followers);
}

// <переменная>::=<полная переменная>|<компонента переменной> | <указанная переменная>
// <обозначение функции>::=<имя функции>|<имя функции> (<фактический параметр>{, <фактический параметр>})
void CParser::Variable(const CSet& followers)
{
	BeginSkipErr(std::set<ESymbol> {ESymbol::IDENT_TOKEN}, followers);
	if (CheckTokenType(ESymbol::IDENT_TOKEN)) {
		Accept(ESymbol::IDENT_TOKEN);
		/*while (CheckKeyword(ESymbol::OPENING_BRACKET_KEYWORD)) {

		}*/
		EndSkipErr(followers);
	}
}

// <loop statement>::=<loop with precondition>|
//	<loop with postcontition> | <loop with parameter>
void CParser::LoopStatement(const CSet& followers) {
	if (CheckKeyword(ESymbol::FOR_KEYWORD)) {
		LoopWithParameter(followers);
	}
	else if (CheckKeyword(ESymbol::WHILE_KEYWORD)) {
		LoopWithPrecondition(followers);
	}
	else if (CheckKeyword(ESymbol::REPEAT_KEYWORD)) {
		LoopWithPostcondition(followers);
	}
}

// <loop with precondition>::= while <expression> do <statement>
void CParser::LoopWithPrecondition(const CSet& followers) {
	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::WHILE_KEYWORD}), followers);
	if (Contains(CSet(std::set<ESymbol> {ESymbol::WHILE_KEYWORD}))) {
		Accept(ESymbol::WHILE_KEYWORD);
		Expression(followers + CSet(std::set<ESymbol> {ESymbol::DO_KEYWORD}));
		Accept(ESymbol::DO_KEYWORD);
		Statement(followers);
		EndSkipErr(followers);
	}
}

// <loop with postcontition>::= repeat <statement>{;<statement>} until <expression>
void CParser::LoopWithPostcondition(const CSet& followers) {
	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::REPEAT_KEYWORD}), followers);
	if (Contains(CSet(std::set<ESymbol> {ESymbol::REPEAT_KEYWORD}))) {
		Accept(ESymbol::REPEAT_KEYWORD);
		Statement(followers + CSet(std::set<ESymbol> {ESymbol::SEMICOLON_KEYWORD, ESymbol::UNTIL_KEYWORD}));
		while (CheckKeyword(ESymbol::SEMICOLON_KEYWORD)) {
			GetNextToken();
			Statement(followers + CSet(std::set<ESymbol> {ESymbol::SEMICOLON_KEYWORD, ESymbol::UNTIL_KEYWORD}));
		}
		Accept(ESymbol::UNTIL_KEYWORD);
		Expression(followers);
		EndSkipErr(followers);
	}
}

// <loop with parameter>::= for <parameter name>:=<expression>
// <to|downto><expression> do <statement>
void CParser::LoopWithParameter(const CSet& followers) {
	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::FOR_KEYWORD}), followers);
	if (Contains(CSet(std::set<ESymbol> {ESymbol::FOR_KEYWORD}))) {
		Accept(ESymbol::FOR_KEYWORD);
		Accept(ESymbol::IDENT_TOKEN);
		Accept(ESymbol::AOP_ASSIGN_KEYWORD);
		Expression(followers + CSet(std::set<ESymbol> {ESymbol::TO_KEYWORD, ESymbol::DOWNTO_KEYWORD, ESymbol::DO_KEYWORD}));
		if (CheckKeyword(ESymbol::DOWNTO_KEYWORD)) {
			Accept(ESymbol::DOWNTO_KEYWORD);
		}
		else {
			Accept(ESymbol::TO_KEYWORD);
		}
		Expression(followers + CSet(std::set<ESymbol> {ESymbol::DO_KEYWORD}));
		Accept(ESymbol::DO_KEYWORD);
		Statement(followers);
		EndSkipErr(followers);
	}

}

// <expression>::=<simple expression>{<relation operation><simple expression>}
void CParser::Expression(const CSet& followers) {
	SimpleExpression(followers +
		CSet(std::set<ESymbol> {ESymbol::COP_EQ_KEYWORD,
			ESymbol::COP_NE_KEYWORD,
			ESymbol::COP_LT_KEYWORD,
			ESymbol::COP_LE_KEYWORD,
			ESymbol::COP_GE_KEYWORD,
			ESymbol::COP_GT_KEYWORD}));
	while (RelationOperation(followers)) {
		SimpleExpression(followers +
			CSet(std::set<ESymbol> {ESymbol::COP_EQ_KEYWORD,
				ESymbol::COP_NE_KEYWORD,
				ESymbol::COP_LT_KEYWORD,
				ESymbol::COP_LE_KEYWORD,
				ESymbol::COP_GE_KEYWORD,
				ESymbol::COP_GT_KEYWORD}));
	}
}

// <relation operation>::= = | <> | < | <= | >= | >
bool CParser::RelationOperation(const CSet& followers) {
	bool result = false;

	if (CheckKeyword(ESymbol::COP_EQ_KEYWORD)) {
		Accept(ESymbol::COP_EQ_KEYWORD);
		result = true;
	}
	else if (CheckKeyword(ESymbol::COP_NE_KEYWORD)) {
		Accept(ESymbol::COP_NE_KEYWORD);
		result = true;
	}
	else if (CheckKeyword(ESymbol::COP_LT_KEYWORD)) {
		Accept(ESymbol::COP_LT_KEYWORD);
		result = true;
	}
	else if (CheckKeyword(ESymbol::COP_LE_KEYWORD)) {
		Accept(ESymbol::COP_LE_KEYWORD);
		result = true;
	}
	else if (CheckKeyword(ESymbol::COP_GE_KEYWORD)) {
		Accept(ESymbol::COP_GE_KEYWORD);
		result = true;
	}
	else if (CheckKeyword(ESymbol::COP_GT_KEYWORD)) {
		Accept(ESymbol::COP_GT_KEYWORD);
		result = true;
	}
	return result;
}


// <simple expression>::=<+|-><term> {<+|-|or><term>}
void CParser::SimpleExpression(const CSet& followers) {
	if (CheckKeyword(ESymbol::AOP_SUM_KEYWORD)) {
		GetNextToken();
	}
	else if (CheckKeyword(ESymbol::AOP_SUB_KEYWORD)) {
		GetNextToken();
	}
	Term(followers + CSet(std::set<ESymbol> {ESymbol::AOP_SUM_KEYWORD, ESymbol::AOP_SUB_KEYWORD, ESymbol::LOP_OR_KEYWORD}));
	while (CheckKeyword(ESymbol::AOP_SUM_KEYWORD) || CheckKeyword(ESymbol::AOP_SUB_KEYWORD) || CheckKeyword(ESymbol::LOP_OR_KEYWORD))
	{
		GetNextToken();
		Term(followers + CSet(std::set<ESymbol> {ESymbol::AOP_SUM_KEYWORD, ESymbol::AOP_SUB_KEYWORD, ESymbol::LOP_OR_KEYWORD}));
	}
}

// <term>::=<factor>{<multiplicative operation><factor>}
void CParser::Term(const CSet& followers) {
	Factor(followers + CSet(std::set<ESymbol> {ESymbol::AOP_MULT_KEYWORD,
		ESymbol::AOP_DIV_REAL_KEYWORD,
		ESymbol::AOP_DIV_KEYWORD,
		ESymbol::AOP_MOD_KEYWORD,
		ESymbol::LOP_AND_KEYWORD}));
	while (MultiplicativeOperation(followers +
		CSet(std::set<ESymbol>{ESymbol::IDENT_TOKEN,
			ESymbol::CONST_TOKEN,
			ESymbol::OPENING_BRACKET_KEYWORD,
			ESymbol::LOP_NOT_KEYWORD})))
	{
		Factor(followers + CSet(std::set<ESymbol> {ESymbol::AOP_MULT_KEYWORD,
			ESymbol::AOP_DIV_REAL_KEYWORD,
			ESymbol::AOP_DIV_KEYWORD,
			ESymbol::AOP_MOD_KEYWORD,
			ESymbol::LOP_AND_KEYWORD}));
	}
}

// <multiplicative operation>::= *|/|div|mod|and
bool CParser::MultiplicativeOperation(const CSet& followers) {
	bool result = false;
	BeginSkipErr(
		CSet(std::set<ESymbol> {ESymbol::AOP_MULT_KEYWORD,
			ESymbol::AOP_DIV_REAL_KEYWORD,
			ESymbol::AOP_DIV_KEYWORD,
			ESymbol::AOP_MOD_KEYWORD,
			ESymbol::LOP_AND_KEYWORD}),
		followers);
	if (Contains(CSet(std::set<ESymbol> {ESymbol::AOP_MULT_KEYWORD,
		ESymbol::AOP_DIV_REAL_KEYWORD,
		ESymbol::AOP_DIV_KEYWORD,
		ESymbol::AOP_MOD_KEYWORD,
		ESymbol::LOP_AND_KEYWORD})))
	{
		if (CheckKeyword(ESymbol::AOP_MULT_KEYWORD)) {
			Accept(ESymbol::AOP_MULT_KEYWORD);
			result = true;
		}
		else if (CheckKeyword(ESymbol::AOP_DIV_REAL_KEYWORD)) {
			Accept(ESymbol::AOP_DIV_REAL_KEYWORD);
			result = true;
		}
		else if (CheckKeyword(ESymbol::AOP_DIV_KEYWORD)) { // div
			Accept(ESymbol::AOP_DIV_KEYWORD);
			result = true;
		}
		else if (CheckKeyword(ESymbol::AOP_MOD_KEYWORD)) { // mod
			Accept(ESymbol::AOP_MOD_KEYWORD);
			result = true;
		}
		else if (CheckKeyword(ESymbol::LOP_AND_KEYWORD)) {
			Accept(ESymbol::LOP_AND_KEYWORD);
			result = true;
		}
		
		EndSkipErr(followers);
	}
	return result;
}

// <factor>::=<переменная>|<константа без знака>|
//	(<expression>) | <обозначение функции> | not <factor>
void CParser::Factor(const CSet& followers) {

	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::IDENT_TOKEN,
		ESymbol::CONST_TOKEN,
		ESymbol::OPENING_BRACKET_KEYWORD,
		ESymbol::LOP_NOT_KEYWORD}),
		followers);
	if (Contains(CSet(std::set<ESymbol> {ESymbol::IDENT_TOKEN,
		ESymbol::CONST_TOKEN,
		ESymbol::OPENING_BRACKET_KEYWORD,
		ESymbol::LOP_NOT_KEYWORD})))
	{
		if (CheckTokenType(ESymbol::IDENT_TOKEN)) {
			// <переменная> | <function notation>
			Variable(followers);
		}
		else if (CheckTokenType(ESymbol::CONST_TOKEN)) {
			GetNextToken();
		}
		else if (CheckKeyword(ESymbol::OPENING_BRACKET_KEYWORD)) {
			GetNextToken();
			Expression(followers + CSet(std::set<ESymbol> {ESymbol::CLOSING_BRACKET_KEYWORD}));
			Accept(ESymbol::CLOSING_BRACKET_KEYWORD);
		}
		else if (CheckKeyword(ESymbol::LOP_NOT_KEYWORD)) {
			GetNextToken();
			Factor(followers);
		}
		EndSkipErr(followers);
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
void CParser::FunctionNotation(const CSet& followers) {
	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::IDENT_TOKEN}), followers);
	if (Contains(CSet(std::set<ESymbol> {ESymbol::IDENT_TOKEN}))) {
		Accept(ESymbol::IDENT_TOKEN);
		if (CheckKeyword(ESymbol::OPENING_BRACKET_KEYWORD)) {
			GetNextToken();
			Expression(followers + CSet(std::set<ESymbol> {ESymbol::COMMA_KEYWORD, ESymbol::CLOSING_BRACKET_KEYWORD}));
			while (CheckKeyword(ESymbol::COMMA_KEYWORD)) {
				GetNextToken();
				Expression(followers + CSet(std::set<ESymbol> {ESymbol::COMMA_KEYWORD, ESymbol::CLOSING_BRACKET_KEYWORD}));
			}
			Accept(ESymbol::CLOSING_BRACKET_KEYWORD);
		}
		EndSkipErr(followers);
	}
}


// <actual parameter>::=<expression>|<variable>|<имя функции>
void CParser::ActualParameter() {

}
