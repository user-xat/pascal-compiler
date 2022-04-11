#include "CKeywordToken.h"

CKeywordToken::CKeywordToken(ESymbol keyword) : CToken(ESymbol::KEYWORD_TOKEN)
{
	m_keyword = keyword;
}

ESymbol CKeywordToken::GetKeyword() const
{
	return m_keyword;
}

std::string CKeywordToken::ToString() const
{
	std::string result("KEYWORD:\t");
	//std::string result("");

	switch (m_keyword)
	{
	case ESymbol::INTEGER_KEYWORD:
		result += "integer";
		break;
	case ESymbol::REAL_KEYWORD:
		result += "real";
		break;
	case ESymbol::STRING_KEYWORD:
		result += "string";
		break;
	case ESymbol::BOOLEAN_KEYWORD:
		result += "boolean";
		break;
	case ESymbol::AOP_ASSIGN_KEYWORD:
		result += ":=";
		break;
	case ESymbol::AOP_SUM_KEYWORD:
		result += "+";
		break;
	case ESymbol::AOP_SUB_KEYWORD:
		result += "-";
		break;
	case ESymbol::AOP_MULT_KEYWORD:
		result += "*";
		break;
	case ESymbol::AOP_DIV_KEYWORD:
		result += "/";
		break;
	case ESymbol::LOP_AND_KEYWORD:
		result += "and";
		break;
	case ESymbol::LOP_OR_KEYWORD:
		result += "or";
		break;
	case ESymbol::LOP_NOT_KEYWORD:
		result += "not";
		break;
	case ESymbol::LOP_XOR_KEYWORD:
		result += "xor";
		break;
	case ESymbol::COP_LT_KEYWORD:
		result += "<";
		break;
	case ESymbol::COP_LE_KEYWORD:
		result += "<=";
		break;
	case ESymbol::COP_GT_KEYWORD:
		result += ">";
		break;
	case ESymbol::COP_GE_KEYWORD:
		result += ">=";
		break;
	case ESymbol::COP_EQ_KEYWORD:
		result += "=";
		break;
	case ESymbol::COP_NE_KEYWORD:
		result += "<>";
		break;
	case ESymbol::PROGRAM_KEYWORD:
		result += "program";
		break;
	case ESymbol::SEMICOLON_KEYWORD:
		result += ";";
		break;
	case ESymbol::BEGIN_KEYWORD:
		result += "begin";
		break;
	case ESymbol::END_KEYWORD:
		result += "end";
		break;
	case ESymbol::VAR_KEYWORD:
		result += "var";
		break;
	case ESymbol::TYPE_KEYWORD:
		result += "type";
		break;
	case ESymbol::IF_KEYWORD:
		result += "if";
		break;
	case ESymbol::THEN_KEYWORD:
		result += "then";
		break;
	case ESymbol::ELSE_KEYWORD:
		result += "else";
		break;
	case ESymbol::WHILE_KEYWORD:
		result += "while";
		break;
	case ESymbol::DO_KEYWORD:
		result += "do";
		break;
	case ESymbol::COLON_KEYWORD:
		result += ":";
		break;
	case ESymbol::OPENING_BRACKET_KEYWORD:
		result += "(";
		break;
	case ESymbol::CLOSING_BRACKET_KEYWORD:
		result += ")";
		break;
	case ESymbol::SINGLE_QUOTE_KEYWORD:
		result += "'";
		break;
	case ESymbol::COMMA_KEYWORD:
		result += ",";
		break;
	case ESymbol::DOT_KEYWORD:
		result += ".";
		break;
	case ESymbol::FUNCTION_KEYWORD:
		result += "function";
		break;
	case ESymbol::FOR_KEYWORD:
		result += "for";
		break;
	case ESymbol::TO_KEYWORD:
		result += "to";
		break;
	case ESymbol::DOWNTO_KEYWORD:
		result += "downto";
		break;
	case ESymbol::REPEAT_KEYWORD:
		result += "repeat";
		break;
	case ESymbol::UNTIL_KEYWORD:
		result += "until";
		break;
	default:
		result = "[ERROR] Unknown keyword!";
		break;
	}

	return result;
}
