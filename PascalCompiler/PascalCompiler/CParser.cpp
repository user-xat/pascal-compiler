#include "CParser.h"

CParser::CParser(const std::string& filepath) {
	m_lexer = std::make_unique<CLexer>(filepath);
	m_scopes = std::make_unique<CScopeStack>();
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

void CParser::Error(const std::string& message)
{
	m_lexer->ThrowError(message);
}

void CParser::Error(const EDataType type)
{
	std::string mes;
	switch (type)
	{
	case EDataType::INTEGER:
		mes += "Expected integer type";
		break;
	case EDataType::REAL:
		mes += "Expected real type";
		break;
	case EDataType::BOOLEAN:
		mes += "Expected boolean type";
		break;
	case EDataType::STRING:
		mes += "Expected string type";
		break;
	case EDataType::FUNCTION:
		break;
	case EDataType::ERROR:
		mes += "Type error";
		break;
	default:
		break;
	}
	m_lexer->ThrowError(mes);
}

void CParser::Accept(ESymbol symbol)
{
	if (m_cur_token == nullptr) {
		throw "Current token is nullptr";
	}
	else if (symbol == ESymbol::IDENT_TOKEN || symbol == ESymbol::CONST_TOKEN)
	{
		if (m_cur_token->GetType() != symbol)
		{
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
	else if (m_cur_token->GetType() != ESymbol::KEYWORD_TOKEN || (CheckKeyword(ESymbol::KEYWORD_TOKEN) && dynamic_cast<CKeywordToken*>(m_cur_token.get())->GetKeyword() != symbol))
	{
		std::string mes_err;
		auto sym_iter = m_key_words.find(symbol);
		if (sym_iter == m_key_words.cend()) {
			mes_err = "unknown";
		}
		else {
			mes_err = sym_iter->second;
		}
		m_lexer->ThrowError("Expected token is " + mes_err);
	}
	GetNextToken();
}

bool CParser::CheckTokenType(ESymbol type) const
{
	if (m_cur_token != nullptr) {
		ESymbol sym = m_cur_token->GetType();
		auto temp = dynamic_cast<CConstToken*>(m_cur_token.get());
		return m_cur_token->GetType() == type;
	}
	return false;
}

bool CParser::CheckKeyword(ESymbol keyword) const
{
	return CheckTokenType(ESymbol::KEYWORD_TOKEN)
		&& dynamic_cast<CKeywordToken*>(m_cur_token.get())->GetKeyword() == keyword;
}

bool CParser::CheckConstVariant(ESymbol variant) const
{
	return CheckTokenType(ESymbol::CONST_TOKEN)
		&& dynamic_cast<CConstToken*>(m_cur_token.get())->GetVariant()->GetType() == variant;
}

void CParser::Type(const CSet& followers) {
	/*BeginSkipErr(
		CSet(std::set<ESymbol> {ESymbol::INTEGER_KEYWORD,
			ESymbol::REAL_KEYWORD,
			ESymbol::STRING_KEYWORD,
			ESymbol::BOOLEAN_KEYWORD}),
		followers);
	if (Contains(
		CSet(std::set<ESymbol> {ESymbol::INTEGER_KEYWORD,
			ESymbol::REAL_KEYWORD,
			ESymbol::STRING_KEYWORD,
			ESymbol::BOOLEAN_KEYWORD})))
	{
		if (CheckKeyword(ESymbol::INTEGER_KEYWORD)) {
			m_scopes->ReleaseIdent(m_base_types.find(EDataType::INTEGER)->second);
			Accept(ESymbol::INTEGER_KEYWORD);
		}
		else if (CheckKeyword(ESymbol::REAL_KEYWORD)) {
			m_scopes->ReleaseIdent(m_base_types.find(EDataType::REAL)->second);
			Accept(ESymbol::REAL_KEYWORD);
		}
		else if (CheckKeyword(ESymbol::STRING_KEYWORD)) {
			m_scopes->ReleaseIdent(m_base_types.find(EDataType::STRING)->second);
			Accept(ESymbol::STRING_KEYWORD);
		}
		else if (CheckKeyword(ESymbol::BOOLEAN_KEYWORD)) {
			m_scopes->ReleaseIdent(m_base_types.find(EDataType::BOOLEAN)->second);
			Accept(ESymbol::BOOLEAN_KEYWORD);
		}
		EndSkipErr(followers);
	}
	else {
		m_scopes->ReleaseIdent(m_base_types.find(EDataType::ERROR)->second);
	}*/

	BeginSkipErr(
		CSet(std::set<ESymbol> {ESymbol::IDENT_TOKEN}), followers);
	if (Contains(
		CSet(std::set<ESymbol> {ESymbol::IDENT_TOKEN})))
	{
		std::string identifier = dynamic_cast<CIdentToken*>(m_cur_token.get())->GetIdentifier();
		IDataTypePtr type = m_scopes->GetIdentType(identifier);
		if (type == nullptr) {
			type = m_base_types.find(EDataType::ERROR)->second;
		}
						
		m_scopes->ReleaseIdent(type);
		Accept(ESymbol::IDENT_TOKEN);
		EndSkipErr(followers);
	}
	else {
		m_scopes->ReleaseIdent(m_base_types.find(EDataType::ERROR)->second);
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
	if (!Contains(starts))
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
	// create fictious scope
	m_scopes->Push();
	m_scopes->AddIdent("integer", m_base_types.find(EDataType::INTEGER)->second);
	m_scopes->AddIdent("boolean", m_base_types.find(EDataType::BOOLEAN)->second);
	m_scopes->AddIdent("real", m_base_types.find(EDataType::REAL)->second);
	m_scopes->AddIdent("string", m_base_types.find(EDataType::STRING)->second);

	// create program scope
	m_scopes->Push();
	if (CheckKeyword(ESymbol::PROGRAM_KEYWORD)) {
		Accept(ESymbol::PROGRAM_KEYWORD);
		Accept(ESymbol::IDENT_TOKEN);
		Accept(ESymbol::SEMICOLON_KEYWORD);
	}
	Block(CSet(std::set<ESymbol> {ESymbol::DOT_KEYWORD}));
	Accept(ESymbol::DOT_KEYWORD);
	m_scopes->Pop(); // destroy program scope
	m_scopes->Pop(); // destroy fictious scope
}

// <block>::=<type section><variable section><function section><compound statement>
void CParser::Block(const CSet& followers) {
	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::TYPE_KEYWORD,
		ESymbol::VAR_KEYWORD,
		ESymbol::FUNCTION_KEYWORD,
		ESymbol::BEGIN_KEYWORD}),
		followers);
	if (CheckKeyword(ESymbol::TYPE_KEYWORD)) {
		TypeSection(followers + CSet(std::set<ESymbol> {ESymbol::VAR_KEYWORD, ESymbol::FUNCTION_KEYWORD, ESymbol::BEGIN_KEYWORD}));
	}
	if (CheckKeyword(ESymbol::VAR_KEYWORD)) {
		VariableSection(followers + CSet(std::set<ESymbol> {ESymbol::FUNCTION_KEYWORD, ESymbol::BEGIN_KEYWORD}));
	}
	if (CheckKeyword(ESymbol::FUNCTION_KEYWORD)) {
		FunctionSection(followers + CSet(std::set<ESymbol> {ESymbol::BEGIN_KEYWORD}));
	}
	if (CheckKeyword(ESymbol::BEGIN_KEYWORD)) {
		CompoundStatement(followers);
	}
	EndSkipErr(followers);
}

// <type section>::=<empty>|type<type definition>;{<type definition>;}
void CParser::TypeSection(const CSet& followers) {
	Accept(ESymbol::TYPE_KEYWORD);
	do
	{
		TypeDefinition(followers + CSet(std::set<ESymbol> {ESymbol::SEMICOLON_KEYWORD}));
		Accept(ESymbol::SEMICOLON_KEYWORD);
	} while (CheckTokenType(ESymbol::IDENT_TOKEN));
	EndSkipErr(followers);
}

// <type definition>::=<name>=<type>
void CParser::TypeDefinition(const CSet& followers) {
	BeginSkipErr(std::set<ESymbol> {ESymbol::IDENT_TOKEN}, followers);
	if (CheckTokenType(ESymbol::IDENT_TOKEN)) {

		m_scopes->AccumulateIdent(dynamic_cast<CIdentToken*>(m_cur_token.get())->GetIdentifier());
		Accept(ESymbol::IDENT_TOKEN);
		Accept(ESymbol::COP_EQ_KEYWORD);
		Type(followers);
		EndSkipErr(followers);
	}
}

// <variable section>::= var <description of variables>;{<description of variables>;}|<empty>
void CParser::VariableSection(const CSet& followers) {
	Accept(ESymbol::VAR_KEYWORD);
	do
	{
		DescriptionOfVariables(followers + CSet(std::set<ESymbol> {ESymbol::SEMICOLON_KEYWORD}));
		Accept(ESymbol::SEMICOLON_KEYWORD);
	} while (CheckTokenType(ESymbol::IDENT_TOKEN));
	EndSkipErr(followers);
}

// <description of variables>::=<name>{, <name>}:<type>
void CParser::DescriptionOfVariables(const CSet& followers) {
	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::IDENT_TOKEN}), followers);
	if (CheckTokenType(ESymbol::IDENT_TOKEN)) {

		m_scopes->AccumulateIdent(dynamic_cast<CIdentToken*>(m_cur_token.get())->GetIdentifier());
		Accept(ESymbol::IDENT_TOKEN);
		while (CheckKeyword(ESymbol::COMMA_KEYWORD)) {
			GetNextToken();

			m_scopes->AccumulateIdent(dynamic_cast<CIdentToken*>(m_cur_token.get())->GetIdentifier());
			Accept(ESymbol::IDENT_TOKEN);
		}
		Accept(ESymbol::COLON_KEYWORD);
		Type(followers);
		EndSkipErr(followers);
	}
}

// <function section>::={function <function header><block>;}
void CParser::FunctionSection(const CSet& followers) {
	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::FUNCTION_KEYWORD}), followers);
	while (Contains(CSet(std::set<ESymbol> {ESymbol::FUNCTION_KEYWORD}))) {
		Accept(ESymbol::FUNCTION_KEYWORD);
		FunctionHeader(followers +
			CSet(std::set<ESymbol> {ESymbol::TYPE_KEYWORD,
				ESymbol::VAR_KEYWORD,
				ESymbol::FUNCTION_KEYWORD,
				ESymbol::BEGIN_KEYWORD}));
		Block(followers + CSet(std::set<ESymbol> {ESymbol::SEMICOLON_KEYWORD}));
		Accept(ESymbol::SEMICOLON_KEYWORD);
		EndSkipErr(followers + CSet(std::set<ESymbol> {ESymbol::FUNCTION_KEYWORD}));
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
		Type(followers + CSet(std::set<ESymbol> {ESymbol::SEMICOLON_KEYWORD}));
		Accept(ESymbol::SEMICOLON_KEYWORD);
		EndSkipErr(followers);
	}
}

// <formal parameters section>::= <parameters group> | var <parameters group> | function <parameters group>
void CParser::FormalParametersSection(const CSet& followers) {
	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::IDENT_TOKEN, ESymbol::VAR_KEYWORD, ESymbol::FUNCTION_KEYWORD}), followers);
	if (Contains(CSet(std::set<ESymbol> {ESymbol::IDENT_TOKEN, ESymbol::VAR_KEYWORD, ESymbol::FUNCTION_KEYWORD}))) {
		if (CheckKeyword(ESymbol::VAR_KEYWORD) || CheckKeyword(ESymbol::FUNCTION_KEYWORD)) {
			GetNextToken();
		}
		ParametersGroup(followers);
		EndSkipErr(followers);
	}
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
		Type(followers);
		EndSkipErr(followers);
	}
}

// <compound section>::= begin <statement>{;<statement>} end
void CParser::CompoundStatement(const CSet& followers) {
	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::BEGIN_KEYWORD}), followers);
	if (Contains(CSet(std::set<ESymbol> {ESymbol::BEGIN_KEYWORD})))
	{
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
	if (CheckKeyword(ESymbol::END_KEYWORD) || CheckKeyword(ESymbol::UNTIL_KEYWORD))
		return;
	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::IDENT_TOKEN,
		ESymbol::BEGIN_KEYWORD,
		ESymbol::IF_KEYWORD,
		ESymbol::FOR_KEYWORD,
		ESymbol::WHILE_KEYWORD,
		ESymbol::REPEAT_KEYWORD}),
		followers);
	if (Contains(CSet(std::set<ESymbol> {ESymbol::IDENT_TOKEN,
		ESymbol::BEGIN_KEYWORD,
		ESymbol::IF_KEYWORD,
		ESymbol::FOR_KEYWORD,
		ESymbol::WHILE_KEYWORD,
		ESymbol::REPEAT_KEYWORD})))
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

		IDataTypePtr type = Expression(followers + CSet(std::set<ESymbol> {ESymbol::THEN_KEYWORD, ESymbol::ELSE_KEYWORD}));
		if (type->GetType() != EDataType::BOOLEAN) {
			Error(EDataType::BOOLEAN);
		}

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
	IDataTypePtr var_type = Variable(followers + CSet(std::set<ESymbol> {ESymbol::AOP_ASSIGN_KEYWORD}));
	Accept(ESymbol::AOP_ASSIGN_KEYWORD);
	IDataTypePtr exp_type = Expression(followers);

	if (var_type->GetType() != exp_type->GetType()) {
		if (!(var_type->GetType() == EDataType::REAL && exp_type->GetType() == EDataType::INTEGER))
			Error(var_type->GetType());
	}
}

// <переменная>::=<полная переменная>|<компонента переменной> | <указанная переменная>
// <обозначение функции>::=<имя функции>|<имя функции> (<фактический параметр>{, <фактический параметр>})
IDataTypePtr CParser::Variable(const CSet& followers)
{
	IDataTypePtr variable_type = nullptr;
	BeginSkipErr(std::set<ESymbol> {ESymbol::IDENT_TOKEN}, followers);
	if (CheckTokenType(ESymbol::IDENT_TOKEN)) {
		std::string ident_name = dynamic_cast<CIdentToken*>(m_cur_token.get())->GetIdentifier();
		IDataTypePtr ident_type = m_scopes->GetIdentType(ident_name);
		if (ident_type != nullptr) {
			variable_type = ident_type;
		}
		else {
			variable_type = m_base_types.find(EDataType::ERROR)->second;
			m_scopes->AddIdent(ident_name, variable_type);
		}

		Accept(ESymbol::IDENT_TOKEN);
		/*while (CheckKeyword(ESymbol::OPENING_BRACKET_KEYWORD)) {

		}*/
		EndSkipErr(followers);
	}
	return variable_type;
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

		IDataTypePtr type = Expression(followers + CSet(std::set<ESymbol> {ESymbol::DO_KEYWORD}));
		if (type->GetType() != m_base_types.find(EDataType::BOOLEAN)->second->GetType()) {
			Error(EDataType::BOOLEAN);
		}

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

		IDataTypePtr type = Expression(followers);
		if (type->GetType() != m_base_types.find(EDataType::BOOLEAN)->second->GetType()) {
			Error(EDataType::BOOLEAN);
		}

		EndSkipErr(followers);
	}
}

// <loop with parameter>::= for <parameter name>:=<expression>
// <to|downto><expression> do <statement>
void CParser::LoopWithParameter(const CSet& followers) {
	BeginSkipErr(CSet(std::set<ESymbol> {ESymbol::FOR_KEYWORD}), followers);
	if (Contains(CSet(std::set<ESymbol> {ESymbol::FOR_KEYWORD}))) {
		Accept(ESymbol::FOR_KEYWORD);

		// IDataTypePtr ident_type = m_scopes->GetIdentType(dynamic_cast<CIdentToken*>(m_cur_token.get())->GetIdentifier());
		// if (ident_type == nullptr)
		IDataTypePtr ident_type = Variable(followers + CSet(std::set<ESymbol> {ESymbol::AOP_ASSIGN_KEYWORD,
			ESymbol::TO_KEYWORD, ESymbol::DOWNTO_KEYWORD, ESymbol::DO_KEYWORD}));
		if (ident_type->GetType() != EDataType::INTEGER) {
			Error("Parameter of cycle must be integer");
		}

		Accept(ESymbol::AOP_ASSIGN_KEYWORD);

		IDataTypePtr exp_type = Expression(followers + CSet(std::set<ESymbol> {ESymbol::TO_KEYWORD, ESymbol::DOWNTO_KEYWORD, ESymbol::DO_KEYWORD}));

		if (ident_type->GetType() != exp_type->GetType()) {
			//Error("Types conflict");
			Error(ident_type->GetType());
		}

		if (CheckKeyword(ESymbol::DOWNTO_KEYWORD)) {
			Accept(ESymbol::DOWNTO_KEYWORD);
		}
		else {
			Accept(ESymbol::TO_KEYWORD);
		}

		exp_type = Expression(followers + CSet(std::set<ESymbol> {ESymbol::DO_KEYWORD}));
		if (ident_type->GetType() != exp_type->GetType()) {
			Error("Types conflict");
		}

		Accept(ESymbol::DO_KEYWORD);
		Statement(followers);
		EndSkipErr(followers);
	}

}

// <expression>::=<simple expression>[<relation operation><simple expression>]
IDataTypePtr CParser::Expression(const CSet& followers) 
{
	IDataTypePtr exp1_type = SimpleExpression(followers +
		CSet(std::set<ESymbol> {ESymbol::COP_EQ_KEYWORD,
			ESymbol::COP_NE_KEYWORD,
			ESymbol::COP_LT_KEYWORD,
			ESymbol::COP_LE_KEYWORD,
			ESymbol::COP_GE_KEYWORD,
			ESymbol::COP_GT_KEYWORD}));
	if (RelationOperation(followers))
	{
		ESymbol comp_op = dynamic_cast<CKeywordToken*>(m_cur_token.get())->GetKeyword();
		GetNextToken();
		IDataTypePtr exp2_type = SimpleExpression(followers +
			CSet(std::set<ESymbol> {ESymbol::COP_EQ_KEYWORD,
				ESymbol::COP_NE_KEYWORD,
				ESymbol::COP_LT_KEYWORD,
				ESymbol::COP_LE_KEYWORD,
				ESymbol::COP_GE_KEYWORD,
				ESymbol::COP_GT_KEYWORD}));

		/*if (exp1_type->GetType() != exp2_type->GetType()) {
			Error(EDataType::BOOLEAN);
		}*/
		if (comp_op == ESymbol::COP_EQ_KEYWORD || comp_op == ESymbol::COP_NE_KEYWORD) {
			if (exp1_type->GetType() != exp2_type->GetType()) {
				if (!(exp1_type->GetType() == EDataType::REAL && exp2_type->GetType() == EDataType::INTEGER
					|| exp1_type->GetType() == EDataType::INTEGER && exp2_type->GetType() == EDataType::REAL))
					Error(exp1_type->GetType());
			}
		}
		else {
			if (exp1_type->GetType() == EDataType::STRING || exp2_type->GetType() == EDataType::STRING)
			{
				Error("Unexepted string type");
			}
			else if (exp1_type->GetType() == EDataType::BOOLEAN || exp2_type->GetType() == EDataType::BOOLEAN) {
				Error("Unexepted boolean type");
			}
		}
		
		exp1_type = m_base_types.find(EDataType::BOOLEAN)->second;
	}
	return exp1_type;
}

// <relation operation>::= = | <> | < | <= | >= | >
bool CParser::RelationOperation(const CSet& followers) {
	bool result = false;

	if (CheckKeyword(ESymbol::COP_EQ_KEYWORD)) {
		//Accept(ESymbol::COP_EQ_KEYWORD);
		result = true;
	}
	else if (CheckKeyword(ESymbol::COP_NE_KEYWORD)) {
		//Accept(ESymbol::COP_NE_KEYWORD);
		result = true;
	}
	else if (CheckKeyword(ESymbol::COP_LT_KEYWORD)) {
		//Accept(ESymbol::COP_LT_KEYWORD);
		result = true;
	}
	else if (CheckKeyword(ESymbol::COP_LE_KEYWORD)) {
		//Accept(ESymbol::COP_LE_KEYWORD);
		result = true;
	}
	else if (CheckKeyword(ESymbol::COP_GE_KEYWORD)) {
		//Accept(ESymbol::COP_GE_KEYWORD);
		result = true;
	}
	else if (CheckKeyword(ESymbol::COP_GT_KEYWORD)) {
		//Accept(ESymbol::COP_GT_KEYWORD);
		result = true;
	}

	return result;
}


// <simple expression>::=<+|-><term> {<+|-|or><term>}
IDataTypePtr CParser::SimpleExpression(const CSet& followers) {
	bool add_operation = false;
	if (CheckKeyword(ESymbol::AOP_SUM_KEYWORD)) {
		add_operation = true;
		GetNextToken();
	}
	else if (CheckKeyword(ESymbol::AOP_SUB_KEYWORD)) {
		add_operation = true;
		GetNextToken();
	}

	IDataTypePtr term1_type = Term(followers + CSet(std::set<ESymbol> {ESymbol::AOP_SUM_KEYWORD, ESymbol::AOP_SUB_KEYWORD, ESymbol::LOP_OR_KEYWORD}));

	if (term1_type->GetType() != EDataType::INTEGER && term1_type->GetType() != EDataType::REAL) {
		if (add_operation) {
			Error("Unexpected operation");
		}
	}

	while (CheckKeyword(ESymbol::AOP_SUM_KEYWORD) || CheckKeyword(ESymbol::AOP_SUB_KEYWORD) || CheckKeyword(ESymbol::LOP_OR_KEYWORD))
	{
		ESymbol operation = dynamic_cast<CKeywordToken*>(m_cur_token.get())->GetKeyword();
		if (term1_type->GetType() == EDataType::BOOLEAN && operation != ESymbol::LOP_OR_KEYWORD) {
			Error("Incorrect operation type! Need 'or' operator.");
		}

		GetNextToken();
		IDataTypePtr term2_type = Term(followers + CSet(std::set<ESymbol> {ESymbol::AOP_SUM_KEYWORD, ESymbol::AOP_SUB_KEYWORD, ESymbol::LOP_OR_KEYWORD}));

		if (term1_type->GetType() != term2_type->GetType()) {
			if (!(term1_type->GetType() == EDataType::REAL && term2_type->GetType() == EDataType::INTEGER
				|| term1_type->GetType() == EDataType::INTEGER && term2_type->GetType() == EDataType::REAL))
				Error(term1_type->GetType());
			else if (term1_type->GetType() == EDataType::INTEGER) {
				term1_type = term2_type;
			}
		}
	}
	return term1_type;
}

// <term>::=<factor>{<multiplicative operation><factor>}
IDataTypePtr CParser::Term(const CSet& followers) {
	IDataTypePtr fact1_type = Factor(followers + CSet(std::set<ESymbol> {ESymbol::AOP_MULT_KEYWORD,
		ESymbol::AOP_DIV_REAL_KEYWORD,
		ESymbol::AOP_DIV_KEYWORD,
		ESymbol::AOP_MOD_KEYWORD,
		ESymbol::LOP_AND_KEYWORD}));
	auto temp = fact1_type->GetType();
	while (MultiplicativeOperation(followers +
		CSet(std::set<ESymbol>{ESymbol::IDENT_TOKEN,
			ESymbol::CONST_TOKEN,
			ESymbol::OPENING_BRACKET_KEYWORD,
			ESymbol::LOP_NOT_KEYWORD})))
	{
		ESymbol operation = dynamic_cast<CKeywordToken*>(m_cur_token.get())->GetKeyword();
		if (fact1_type->GetType() == EDataType::BOOLEAN && operation != ESymbol::LOP_AND_KEYWORD) {
			Error("Incorrect operation type! Need 'and' operator.");
		}
		else if (fact1_type->GetType() != EDataType::BOOLEAN && operation == ESymbol::LOP_AND_KEYWORD) {
			Error(EDataType::BOOLEAN);
		}


		GetNextToken();

		IDataTypePtr fact2_type = Factor(followers + CSet(std::set<ESymbol> {ESymbol::AOP_MULT_KEYWORD,
			ESymbol::AOP_DIV_REAL_KEYWORD,
			ESymbol::AOP_DIV_KEYWORD,
			ESymbol::AOP_MOD_KEYWORD,
			ESymbol::LOP_AND_KEYWORD}));
		temp = fact2_type->GetType();
		if (fact1_type->GetType() != fact2_type->GetType()) {
			if (!(fact1_type->GetType() == EDataType::REAL && fact2_type->GetType() == EDataType::INTEGER
				|| fact1_type->GetType() == EDataType::INTEGER && fact2_type->GetType() == EDataType::REAL))
				Error(fact1_type->GetType());
			else if (fact1_type->GetType() == EDataType::INTEGER) {
				fact1_type = fact2_type;
			}
		}
	}
	return fact1_type;
}

// <multiplicative operation>::= *|/|div|mod|and
bool CParser::MultiplicativeOperation(const CSet& followers) {
	bool result = false;
	if (CheckKeyword(ESymbol::AOP_MULT_KEYWORD)) {
		//Accept(ESymbol::AOP_MULT_KEYWORD);
		result = true;
	}
	else if (CheckKeyword(ESymbol::AOP_DIV_REAL_KEYWORD)) {
		//Accept(ESymbol::AOP_DIV_REAL_KEYWORD);
		result = true;
	}
	else if (CheckKeyword(ESymbol::AOP_DIV_KEYWORD)) { // div
		//Accept(ESymbol::AOP_DIV_KEYWORD);
		result = true;
	}
	else if (CheckKeyword(ESymbol::AOP_MOD_KEYWORD)) { // mod
		//Accept(ESymbol::AOP_MOD_KEYWORD);
		result = true;
	}
	else if (CheckKeyword(ESymbol::LOP_AND_KEYWORD)) {
		//Accept(ESymbol::LOP_AND_KEYWORD);
		result = true;
	}
	return result;
}

// <factor>::=<переменная>|<константа без знака>|
//	(<expression>) | <обозначение функции> | not <factor>
IDataTypePtr CParser::Factor(const CSet& followers) {
	IDataTypePtr fact_type = nullptr;
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
			fact_type = Variable(followers);
		}
		else if (CheckTokenType(ESymbol::CONST_TOKEN)) {
			ESymbol const_type = dynamic_cast<CConstToken*>(m_cur_token.get())->GetVariant()->GetType();
			switch (const_type)
			{
			case ESymbol::INTEGER_VARIANT:
				fact_type = m_base_types.find(EDataType::INTEGER)->second;
				break;
			case ESymbol::REAL_VARIANT:
				fact_type = m_base_types.find(EDataType::REAL)->second;
				break;
			case ESymbol::STRING_VARIANT:
				fact_type = m_base_types.find(EDataType::STRING)->second;
				break;
			case ESymbol::BOOLEAN_VARIANT:
				fact_type = m_base_types.find(EDataType::BOOLEAN)->second;
				break;
			}
			GetNextToken();
		}
		else if (CheckKeyword(ESymbol::OPENING_BRACKET_KEYWORD)) {
			GetNextToken();
			fact_type = Expression(followers + CSet(std::set<ESymbol> {ESymbol::CLOSING_BRACKET_KEYWORD}));
			auto temp = fact_type->GetType();
			Accept(ESymbol::CLOSING_BRACKET_KEYWORD);
		}
		else if (CheckKeyword(ESymbol::LOP_NOT_KEYWORD)) {
			GetNextToken();
			fact_type = Factor(followers);
			if (fact_type->GetType() != EDataType::BOOLEAN) {
				Error(EDataType::BOOLEAN);
			}
		}
		EndSkipErr(followers);
	}
	return fact_type;
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
