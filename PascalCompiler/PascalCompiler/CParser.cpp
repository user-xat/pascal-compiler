//#include "CParser.h"
//
//CParser::CParser(const std::string& filepath) {
//	m_lexer = std::make_unique<CLexer>(filepath);
//}
//
//void CParser::Parse()
//{
//	GetNextToken();
//	Program();
//}
//
//bool CParser::GetNextToken() {
//	m_cur_token = m_lexer->GetNextToken();
//	return true;
//}
//
//void CParser::Accept(EKeyWords keyword)
//{
//	if (m_cur_token == nullptr) {
//		throw "Current token is nullptr";
//	}
//	else if (m_cur_token->GetType() != ETokenType::KEYWORD) {
//		m_lexer->ThrowError("Expected token is not keyword");
//	}
//	else if (dynamic_cast<CKeywordToken*>(m_cur_token.get())->GetKeyword() != keyword) {
//		m_lexer->ThrowError("Expected token is " + m_cur_token->ToString());
//	}
//	GetNextToken();
//}
//
//void CParser::Accept(ETokenType type)
//{
//	if (m_cur_token == nullptr) {
//		throw "Current token is nullptr";
//	}
//	else if (m_cur_token->GetType() != type) {
//		std::string type_str;
//		switch (type)
//		{
//		case ETokenType::IDENT:
//			type_str = "identificator";
//			break;
//		case ETokenType::KEYWORD:
//			type_str = "keyword";
//			break;
//		case ETokenType::CONST:
//			type_str = "constant";
//			break;
//		}
//		m_lexer->ThrowError("Expected token is not type of " + type_str);
//	}
//	GetNextToken();
//}
//
//bool CParser::CheckTokenType(ETokenType type) const
//{
//	if (m_cur_token != nullptr) {
//		return m_cur_token->GetType() == type;
//	}
//	return false;
//}
//
//bool CParser::CheckKeyword(EKeyWords keyword) const
//{
//	return CheckTokenType(ETokenType::KEYWORD) && dynamic_cast<CKeywordToken*>(m_cur_token.get())->GetKeyword() == keyword;
//}
//
//bool CParser::CheckConstVariant(EVariantType variant) const
//{
//	return CheckTokenType(ETokenType::CONST) && dynamic_cast<CConstToken*>(m_cur_token.get())->GetVariant()->GetType() == variant;
//}
//
//void CParser::Type() {
//	if (m_cur_token == nullptr) {
//		throw "Current token is nullptr";
//	}
//	else if (m_cur_token->GetType() != ETokenType::KEYWORD) {
//		m_lexer->ThrowError("Expected token is not keyword");
//	}
//	else {
//		CKeywordToken* token = dynamic_cast<CKeywordToken*>(m_cur_token.get());
//		if (token->GetKeyword() != EKeyWords::INTEGER && token->GetKeyword() != EKeyWords::REAL
//			&& token->GetKeyword() != EKeyWords::STRING && token->GetKeyword() != EKeyWords::BOOLEAN)
//		{
//			m_lexer->ThrowError("Expected token is not a type");
//		}
//		GetNextToken();
//	}
//}
//
//bool CParser::Contains(const CSet& set) const {
//	return CheckTokenType(ETokenType::KEYWORD) && set.contains(dynamic_cast<CKeywordToken*>(m_cur_token.get())->GetKeyword());
//}
//
//bool CParser::Contains(const std::set<ETokenType>& set) const {
//	return m_cur_token != nullptr && set.find(m_cur_token->GetType()) != set.cend();
//}
//
//void CParser::BeginSkipErr(const CSet& starts, const CSet& followers) {
//	if (!Contains(starts) && !Contains(followers))
//	{
//		m_lexer->ThrowError("Unexpected token");
//		while (!Contains(starts) && !Contains(followers)) {
//			GetNextToken();
//		}
//	}
//}
//
//void CParser::BeginSkipErr(const std::set<ETokenType>& starts, const CSet& followers) {
//	if (!Contains(starts) && !Contains(followers))
//	{
//		m_lexer->ThrowError("Unexpected token");
//		while (!Contains(starts) && !Contains(followers)) {
//			GetNextToken();
//		}
//	}
//}
//
//void CParser::EndSkipErr(const CSet& followers) {
//	if (!Contains(followers)) {
//		m_lexer->ThrowError("Unexpected token");
//		while (!Contains(followers)) {
//			GetNextToken();
//		}
//	}
//}
//
//// <program>:: = [program <name>;]<block>.
//void CParser::Program()
//{
//	if (CheckKeyword(EKeyWords::PROGRAM)) {
//		Accept(EKeyWords::PROGRAM);		// get program's name
//		Accept(ETokenType::IDENT);
//		Accept(EKeyWords::SEMICOLON);
//	}
//	Block(std::set<EKeyWords> {EKeyWords::DOT});
//	Accept(EKeyWords::DOT);
//}
//
//// <block>::=<type section><variable section><function section><compound statement>
//void CParser::Block(const CSet& followers) {
//	BeginSkipErr(CSet(std::set<EKeyWords> {EKeyWords::TYPE, EKeyWords::VAR, EKeyWords::FUNCTION, EKeyWords::BEGIN}), followers);
//	TypeSection(followers + CSet(std::set<EKeyWords> {EKeyWords::VAR, EKeyWords::FUNCTION, EKeyWords::BEGIN}));
//	VariableSection(followers + CSet(std::set<EKeyWords> {EKeyWords::FUNCTION, EKeyWords::BEGIN}));
//	FunctionSection(followers + CSet(std::set<EKeyWords> {EKeyWords::BEGIN}));
//	CompoundStatement(followers);
//	EndSkipErr(followers);
//}
//
//// <type section>::=<empty>|type<type definition>;{<type definition>;}
//void CParser::TypeSection(const CSet& followers) {
//	BeginSkipErr(CSet(std::set<EKeyWords> {EKeyWords::TYPE}), followers);
//	if (Contains(CSet(std::set<EKeyWords> {EKeyWords::TYPE}))) {
//		Accept(EKeyWords::TYPE);
//		do
//		{
//			TypeDefinition(followers + CSet(std::set<EKeyWords> {EKeyWords::SEMICOLON}));
//			Accept(EKeyWords::SEMICOLON);
//		} while (CheckTokenType(ETokenType::IDENT));
//		EndSkipErr(followers);
//	}
//}
//
//// <type definition>::=<name>=<type>
//void CParser::TypeDefinition(const CSet& followers) {
//	BeginSkipErr(std::set<ETokenType> {ETokenType::IDENT}, followers);
//	if (CheckTokenType(ETokenType::IDENT)) {
//		Accept(ETokenType::IDENT);
//		Accept(EKeyWords::COP_EQ);
//		Type();
//		EndSkipErr(followers);
//	}
//}
//
//// <variable section>::= var <description of variables>;{<description of variables>;}|<empty>
//void CParser::VariableSection(const CSet& followers) {
//	BeginSkipErr(CSet(std::set<EKeyWords> {EKeyWords::VAR}), followers);
//	if (Contains(CSet(std::set<EKeyWords> {EKeyWords::VAR}))) {
//		Accept(EKeyWords::VAR);
//		do
//		{
//			DescriptionOfVariables(followers + CSet(std::set<EKeyWords> {EKeyWords::SEMICOLON}));
//			Accept(EKeyWords::SEMICOLON);
//		} while (CheckTokenType(ETokenType::IDENT));
//		EndSkipErr(followers);
//	}
//}
//
//// <description of variables>::=<name>{, <name>}:<type>
//void CParser::DescriptionOfVariables(const CSet& followers) {
//	BeginSkipErr(std::set<ETokenType> {ETokenType::IDENT}, followers);
//	if (CheckTokenType(ETokenType::IDENT)) {
//		Accept(ETokenType::IDENT);
//		while (CheckKeyword(EKeyWords::COMMA)) {
//			GetNextToken();
//			Accept(ETokenType::IDENT);
//		}
//		Accept(EKeyWords::COLON);
//		Type();
//		EndSkipErr(followers);
//	}
//}
//
//// <function section>::={function <function header><block>;}
//void CParser::FunctionSection(const CSet& followers) {
//	BeginSkipErr(CSet(std::set<EKeyWords> {EKeyWords::FUNCTION}), followers);
//	while (Contains(CSet(std::set<EKeyWords> {EKeyWords::FUNCTION}))) {
//		Accept(EKeyWords::FUNCTION);
//		FunctionHeader(followers + CSet(std::set<EKeyWords> {EKeyWords::TYPE, EKeyWords::VAR, EKeyWords::FUNCTION, EKeyWords::BEGIN}));
//		Block(std::set<EKeyWords> {EKeyWords::SEMICOLON});
//		Accept(EKeyWords::SEMICOLON);
//		BeginSkipErr(CSet(std::set<EKeyWords> {EKeyWords::FUNCTION}), followers);
//	}
//}
//
//// <function header>::= <name>[(<formal parameters section>{; <formal parameters section>})]:<result type>;
//void CParser::FunctionHeader(const CSet& followers) {
//	BeginSkipErr(std::set<ETokenType> {ETokenType::IDENT}, followers);
//	if (CheckTokenType(ETokenType::IDENT)) {
//		Accept(ETokenType::IDENT);
//		if (CheckKeyword(EKeyWords::OPENING_BRACKET)) {
//			GetNextToken();
//			do
//			{
//				FormalParametersSection(followers + CSet(std::set<EKeyWords> {EKeyWords::SEMICOLON, EKeyWords::CLOSING_BRACKET}));
//			} while (CheckKeyword(EKeyWords::SEMICOLON) && GetNextToken());
//			Accept(EKeyWords::CLOSING_BRACKET);
//		}
//		Accept(EKeyWords::COLON);
//		Type();
//		Accept(EKeyWords::SEMICOLON);
//		EndSkipErr(followers);
//	}
//}
//
//// НЕ РАБОТАЕТ НЕЙТРАЛИЗАЦИЯ СИНТАКСИЧЕСКИХ ОШИБОК
//// <formal parameters section>::= <parameters group> |
//// var <parameters group> | function <parameters group>
//void CParser::FormalParametersSection(const CSet& followers) {
//	BeginSkipErr(CSet(std::set<EKeyWords> {EKeyWords::VAR, EKeyWords::FUNCTION}), followers);
//	if (Contains(CSet(std::set<EKeyWords> {EKeyWords::VAR, EKeyWords::FUNCTION}))) {
//		GetNextToken();
//	}
//	BeginSkipErr(std::set<ETokenType> {ETokenType::IDENT}, followers);
//	ParametersGroup(followers);
//}
//
//// <parameters group>::=<name>{,<name>}:<type>
//void CParser::ParametersGroup(const CSet& followers) {
//	BeginSkipErr(std::set<ETokenType> {ETokenType::IDENT}, followers);
//	if (CheckTokenType(ETokenType::IDENT)) {
//		do
//		{
//			Accept(ETokenType::IDENT);
//		} while (CheckKeyword(EKeyWords::COMMA) && GetNextToken());
//		Accept(EKeyWords::COLON);
//		Type();
//		EndSkipErr(followers);
//	}
//}
//
//// <compound section>::= begin <statement>{;<statement>} end
//void CParser::CompoundStatement(const CSet& followers) {
//	BeginSkipErr(CSet(std::set<EKeyWords> {EKeyWords::BEGIN}), followers);
//	if (Contains(CSet(std::set<EKeyWords> {EKeyWords::BEGIN}))) {
//		Accept(EKeyWords::BEGIN);
//		Statement(followers + CSet(std::set<EKeyWords> {EKeyWords::SEMICOLON, EKeyWords::END}));
//		while (CheckKeyword(EKeyWords::SEMICOLON)) {
//			GetNextToken();
//			Statement(followers + CSet(std::set<EKeyWords> {EKeyWords::SEMICOLON, EKeyWords::END}));
//		}
//		Accept(EKeyWords::END);
//		EndSkipErr(followers);
//	}
//}
//
//
//// <statement>::=<assignment statement> | <compound statement> | 
//// <condition statement> | <loop statement> | <empty statement>
//void CParser::Statement(const CSet& followers) {
//	BeginSkipErr(std::set<ETokenType> {ETokenType::IDENT},
//		followers + CSet(std::set<EKeyWords> {EKeyWords::BEGIN, EKeyWords::IF, EKeyWords::FOR, EKeyWords::WHILE, EKeyWords::REPEAT}));
//	if (CheckTokenType(ETokenType::IDENT)) {
//		AssignmentStatement(followers);
//	}
//	BeginSkipErr(CSet(std::set<EKeyWords> {EKeyWords::BEGIN, EKeyWords::IF, EKeyWords::FOR, EKeyWords::WHILE, EKeyWords::REPEAT}), followers);
//	if (Contains(CSet(std::set<EKeyWords> {EKeyWords::BEGIN, EKeyWords::IF, EKeyWords::FOR, EKeyWords::WHILE, EKeyWords::REPEAT})))
//	{
//		if (CheckKeyword(EKeyWords::BEGIN)) {
//			CompoundStatement(followers);
//		}
//		else if (CheckKeyword(EKeyWords::IF)) {
//			ConditionStatement(followers);
//		}
//		else if (CheckKeyword(EKeyWords::FOR)
//			|| CheckKeyword(EKeyWords::WHILE)
//			|| CheckKeyword(EKeyWords::REPEAT))
//		{
//			LoopStatement(followers);
//		}
//	}
//}
//
//// <condition statement>::= if <exression> then <statement> [else <statement>]
//void CParser::ConditionStatement(const CSet& followers) {
//	BeginSkipErr(CSet(std::set<EKeyWords> {EKeyWords::IF}), followers);
//	if (Contains(CSet(std::set<EKeyWords> {EKeyWords::IF})))
//	{
//		Accept(EKeyWords::IF);
//		Expression(followers + CSet(std::set<EKeyWords> {EKeyWords::THEN, EKeyWords::ELSE}));
//		Accept(EKeyWords::THEN);
//		Statement(followers + CSet(std::set<EKeyWords> {EKeyWords::ELSE}));
//		if (CheckKeyword(EKeyWords::ELSE)) {
//			GetNextToken();
//			Statement(followers);
//		}
//		EndSkipErr(followers);
//	}
//}
//
//// <assignment statement>::=<variable>:=<выражение> | <имя функции>:=<выражение>
//void CParser::AssignmentStatement(const CSet& followers) {
//	Variable(followers + CSet(std::set<EKeyWords> {EKeyWords::AOP_ASSIGN}));
//	Accept(EKeyWords::AOP_ASSIGN);
//	Expression(followers);
//}
//
//// <переменная>::=<полная переменная>|<компонента переменной> | <указанная переменная>
//// <обозначение функции>::=<имя функции>|<имя функции> (<фактический параметр>{, <фактический параметр>})
//void CParser::Variable(const CSet& followers)
//{
//	BeginSkipErr(std::set<ETokenType> {ETokenType::IDENT}, followers);
//	if (CheckTokenType(ETokenType::IDENT)) {
//		Accept(ETokenType::IDENT);
//		while (CheckKeyword(EKeyWords::OPENING_BRACKET)) {
//
//		}
//		EndSkipErr(followers);
//	}
//}
//
//// <loop statement>::=<loop with precondition>|
////	<loop with postcontition> | <loop with parameter>
//void CParser::LoopStatement(const CSet& followers) {
//	if (CheckKeyword(EKeyWords::FOR)) {
//		LoopWithParameter(followers);
//	}
//	else if (CheckKeyword(EKeyWords::WHILE)) {
//		LoopWithPrecondition(followers);
//	}
//	else if (CheckKeyword(EKeyWords::REPEAT)) {
//		LoopWithPostcondition(followers);
//	}
//}
//
//// <loop with precondition>::= while <expression> do <statement>
//void CParser::LoopWithPrecondition(const CSet& followers) {
//	BeginSkipErr(CSet(std::set<EKeyWords> {EKeyWords::WHILE}), followers);
//	if (Contains(CSet(std::set<EKeyWords> {EKeyWords::WHILE}))) {
//		Accept(EKeyWords::WHILE);
//		Expression(followers + CSet(std::set<EKeyWords> {EKeyWords::DO}));
//		Accept(EKeyWords::DO);
//		Statement(followers);
//		EndSkipErr(followers);
//	}
//}
//
//// <loop with postcontition>::= repeat <statement>{;<statement>} until <expression>
//void CParser::LoopWithPostcondition(const CSet& followers) {
//	BeginSkipErr(CSet(std::set<EKeyWords> {EKeyWords::REPEAT}), followers);
//	if (Contains(CSet(std::set<EKeyWords> {EKeyWords::REPEAT}))) {
//		Accept(EKeyWords::REPEAT);
//		Statement(followers + CSet(std::set<EKeyWords> {EKeyWords::SEMICOLON, EKeyWords::UNTIL}));
//		while (CheckKeyword(EKeyWords::SEMICOLON)) {
//			GetNextToken();
//			Statement(followers + CSet(std::set<EKeyWords> {EKeyWords::SEMICOLON, EKeyWords::UNTIL}));
//		}
//		Accept(EKeyWords::UNTIL);
//		Expression(followers);
//		EndSkipErr(followers);
//	}
//}
//
//// <loop with parameter>::= for <parameter name>:=<expression>
//// <to|downto><expression> do <statement>
//void CParser::LoopWithParameter(const CSet& followers) {
//	BeginSkipErr(CSet(std::set<EKeyWords> {EKeyWords::FOR}), followers);
//	if (Contains(CSet(std::set<EKeyWords> {EKeyWords::FOR}))) {
//		Accept(EKeyWords::FOR);
//		Accept(ETokenType::IDENT);
//		Accept(EKeyWords::AOP_ASSIGN);
//		Expression(followers + CSet(std::set<EKeyWords> {EKeyWords::TO, EKeyWords::DOWNTO, EKeyWords::DO}));
//		if (CheckKeyword(EKeyWords::TO)) {
//			Accept(EKeyWords::TO);
//		}
//		else if (CheckKeyword(EKeyWords::DOWNTO)) {
//			Accept(EKeyWords::DOWNTO);
//		}
//		Expression(followers + CSet(std::set<EKeyWords> {EKeyWords::DO}));
//		Accept(EKeyWords::DO);
//		Statement(followers);
//		EndSkipErr(followers);
//	}
//
//}
//
//// <expression>::=<simple expression>{<relation operation><simple expression>}
//void CParser::Expression(const CSet& followers) {
//	SimpleExpression(followers);
//	while (RelationOperation(followers)) {
//		GetNextToken();
//		SimpleExpression(followers);
//	}
//}
//
//// <relation operation>::= = | <> | < | <= | >= | >
//bool CParser::RelationOperation(const CSet& followers) {
//	bool result = false;
//	/*if (!CheckKeyword(EKeyWords::COP_EQ) && !CheckKeyword(EKeyWords::COP_NE)
//		&& !CheckKeyword(EKeyWords::COP_LT) && !CheckKeyword(EKeyWords::COP_LE)
//		&& !CheckKeyword(EKeyWords::COP_GE) && !CheckKeyword(EKeyWords::COP_GT))
//	{
//		result = false;
//		m_lexer->ThrowError("Exepted token should be on of following: =, <>, <, <=, >=, >");
//	}*/
//
//	BeginSkipErr(
//		CSet(std::set<EKeyWords> {EKeyWords::COP_EQ, EKeyWords::COP_NE, EKeyWords::COP_LT, EKeyWords::COP_LE, EKeyWords::COP_GE, EKeyWords::COP_GT}),
//		followers);
//	if (Contains(CSet(std::set<EKeyWords> {EKeyWords::COP_EQ, EKeyWords::COP_NE, EKeyWords::COP_LT, EKeyWords::COP_LE, EKeyWords::COP_GE, EKeyWords::COP_GT}))) 
//	{
//		if (CheckKeyword(EKeyWords::COP_EQ)) {
//			result = true;
//		}
//		else if (CheckKeyword(EKeyWords::COP_NE)) {
//			result = true;
//		}
//		else if (CheckKeyword(EKeyWords::COP_LT)) {
//			result = true;
//		}
//		else if (CheckKeyword(EKeyWords::COP_LE)) {
//			result = true;
//		}
//		else if (CheckKeyword(EKeyWords::COP_GE)) {
//			result = true;
//		}
//		else if (CheckKeyword(EKeyWords::COP_GT)) {
//			result = true;
//		}
//		EndSkipErr(followers);
//	}
//	return result;
//}
//
//
//// <simple expression>::=<+|-><term> {<+|-|or><term>}
//void CParser::SimpleExpression(const CSet& followers) {
//	if (CheckKeyword(EKeyWords::AOP_SUM)) {
//		GetNextToken();
//	}
//	else if (CheckKeyword(EKeyWords::AOP_SUB)) {
//		GetNextToken();
//	}
//	Term(followers + CSet(std::set<EKeyWords> {EKeyWords::AOP_SUM, EKeyWords::AOP_SUB, EKeyWords::LOP_OR}));
//	while (CheckKeyword(EKeyWords::AOP_SUM) || CheckKeyword(EKeyWords::AOP_SUB) || CheckKeyword(EKeyWords::LOP_OR))
//	{
//		GetNextToken();
//		Term(followers + CSet(std::set<EKeyWords> {EKeyWords::AOP_SUM, EKeyWords::AOP_SUB, EKeyWords::LOP_OR}));
//	}
//}
//
//// <term>::=<factor>{<multiplicative operation><factor>}
//void CParser::Term(const CSet& followers) {
//	Factor(followers + CSet(std::set<EKeyWords> {EKeyWords::AOP_MULT, EKeyWords::AOP_DIV_REAL, EKeyWords::AOP_DIV, EKeyWords::AOP_MOD, EKeyWords::LOP_AND}));
//	while (MultiplicativeOperation(followers)) {
//		GetNextToken();
//		Factor(followers + CSet(std::set<EKeyWords> {EKeyWords::AOP_MULT, EKeyWords::AOP_DIV_REAL, EKeyWords::AOP_DIV, EKeyWords::AOP_MOD, EKeyWords::LOP_AND}));
//	}
//}
//
//// <multiplicative operation>::= *|/|div|mod|and
//bool CParser::MultiplicativeOperation(const CSet& followers) {
//	bool result = false;
//	BeginSkipErr(
//		CSet(std::set<EKeyWords> {EKeyWords::AOP_MULT, EKeyWords::AOP_DIV_REAL, EKeyWords::AOP_DIV, EKeyWords::AOP_MOD, EKeyWords::LOP_AND}),
//		followers);
//	if (Contains(CSet(std::set<EKeyWords> {EKeyWords::AOP_MULT, EKeyWords::AOP_DIV_REAL, EKeyWords::AOP_DIV, EKeyWords::AOP_MOD, EKeyWords::LOP_AND})))
//	{
//		if (CheckKeyword(EKeyWords::AOP_MULT)) {
//			result = true;
//		}
//		else if (CheckKeyword(EKeyWords::AOP_DIV_REAL)) {
//			result = true;
//		}
//		else if (CheckKeyword(EKeyWords::AOP_DIV)) { // div
//			result = true;
//		}
//		else if (CheckKeyword(EKeyWords::AOP_MOD)) { // mod
//			result = true;
//		}
//		else if (CheckKeyword(EKeyWords::LOP_AND)) {
//			result = true;
//		}
//		EndSkipErr(followers);
//	}
//	return result;
//}
//
//// <factor>::=<переменная>|<константа без знака>|
////	(<expression>) | <обозначение функции> | not <factor>
//void CParser::Factor(const CSet& followers) {
//
//	BeginSkipErr(std::set<ETokenType> {ETokenType::IDENT, ETokenType::CONST}, 
//		followers + CSet(std::set<EKeyWords> {EKeyWords::OPENING_BRACKET, EKeyWords::LOP_NOT}));
//	if (Contains(std::set<ETokenType> {ETokenType::IDENT, ETokenType::CONST})
//		|| Contains(CSet(std::set<EKeyWords> {EKeyWords::OPENING_BRACKET, EKeyWords::LOP_NOT}))) 
//	{
//		if (CheckTokenType(ETokenType::IDENT)) {
//			// <переменная> | <function notation>
//			Variable(followers);
//		}
//		else if (CheckTokenType(ETokenType::CONST)) {
//			GetNextToken();
//		}
//		else if (CheckKeyword(EKeyWords::OPENING_BRACKET)) {
//			GetNextToken();
//			Expression(followers + CSet(std::set<EKeyWords> {EKeyWords::CLOSING_BRACKET}));
//			Accept(EKeyWords::CLOSING_BRACKET);
//		}
//		else if (CheckKeyword(EKeyWords::LOP_NOT)) {
//			GetNextToken();
//			Factor(followers);
//		}
//		EndSkipErr(followers);
//	}
//}
//// <переменная>:: = <имя>
//// <константа без знака>::=<число без знака>|<строка>
//
//
//// <число без знака>::=<целое без знака>|
//// <вещественное без знака>
//// <целое без знака>:: = <цифра>{ <цифра> }
//// <вещественное без знака>:: = <целое без знака>.<цифра>{<цифра>} |
//// <целое без знака>.<цифра>{<цифра>}E<порядок> |
//// <целое без знака>E<порядок>
//// <строка>::='<символ>{<символ>}'
//
//
//// <function notation>::=<function name>|<function name>(<actual parameter>{, <actual parameter>})
//void CParser::FunctionNotation(const CSet& followers) {
//	BeginSkipErr(std::set<ETokenType> {ETokenType::IDENT}, followers);
//	if (Contains(std::set<ETokenType> {ETokenType::IDENT})) {
//		Accept(ETokenType::IDENT);
//		if (CheckKeyword(EKeyWords::OPENING_BRACKET)) {
//			GetNextToken();
//			Expression(followers + CSet(std::set<EKeyWords> {EKeyWords::COMMA, EKeyWords::CLOSING_BRACKET}));
//			while (CheckKeyword(EKeyWords::COMMA)) {
//				GetNextToken();
//				Expression(followers + CSet(std::set<EKeyWords> {EKeyWords::COMMA, EKeyWords::CLOSING_BRACKET}));
//			}
//			Accept(EKeyWords::CLOSING_BRACKET);
//		}
//		EndSkipErr(followers);
//	}
//}
//
//
//// <actual parameter>::=<expression>|<variable>|<имя функции>
//void CParser::ActualParameter() {
//
//}
