#include "CKeywordToken.h"

CKeywordToken::CKeywordToken(EKeyWords keyword) : CToken(ETokenType::KEYWORD)
{
	this->m_keyword = keyword;
}

EKeyWords CKeywordToken::GetKeyword()
{
	return this->m_keyword;
}

std::string CKeywordToken::ToString()
{
	std::string result("KEYWORD:\t");

	switch (m_keyword)
	{
	case EKeyWords::INTEGER:
		result += "integer";
		break;
	case EKeyWords::REAL:
		result += "real";
		break;
	case EKeyWords::STRING:
		result += "string";
		break;
	case EKeyWords::BOOLEAN:
		result += "boolean";
		break;
	case EKeyWords::AOP_ASSIGN:
		result += ":=";
		break;
	case EKeyWords::AOP_SUM:
		result += "+";
		break;
	case EKeyWords::AOP_SUB:
		result += "-";
		break;
	case EKeyWords::AOP_MULT:
		result += "*";
		break;
	case EKeyWords::AOP_DIV:
		result += "/";
		break;
	case EKeyWords::LOP_AND:
		result += "and";
		break;
	case EKeyWords::LOP_OR:
		result += "or";
		break;
	case EKeyWords::LOP_NOT:
		result += "not";
		break;
	case EKeyWords::LOP_XOR:
		result += "xor";
		break;
	case EKeyWords::COP_LT:
		result += "<";
		break;
	case EKeyWords::COP_LE:
		result += "<=";
		break;
	case EKeyWords::COP_GT:
		result += ">";
		break;
	case EKeyWords::COP_GE:
		result += ">=";
		break;
	case EKeyWords::COP_EQ:
		result += "=";
		break;
	case EKeyWords::COP_NE:
		result += "<>";
		break;
	case EKeyWords::PROGRAM:
		result += "program";
		break;
	case EKeyWords::END_OF_STATEMENT:
		result += ";";
		break;
	case EKeyWords::BEGIN:
		result += "begin";
		break;
	case EKeyWords::END:
		result += "end";
		break;
	case EKeyWords::VAR:
		result += "var";
		break;
	case EKeyWords::TYPE:
		result += "type";
		break;
	case EKeyWords::IF:
		result += "if";
		break;
	case EKeyWords::THEN:
		result += "then";
		break;
	case EKeyWords::ELSE:
		result += "else";
		break;
	case EKeyWords::WHILE:
		result += "while";
		break;
	case EKeyWords::DO:
		result += "do";
		break;
	case EKeyWords::COLON:
		result += ":";
		break;
	case EKeyWords::OPENING_BRACKET:
		result += "(";
		break;
	case EKeyWords::CLOSING_BRACKET:
		result += ")";
		break;
	case EKeyWords::SINGLE_QUOTE:
		result += "'";
		break;
	case EKeyWords::COMMA:
		result += ",";
		break;
	case EKeyWords::DOT:
		result += ".";
		break;
	case EKeyWords::FUNCTION:
		result += "function";
		break;
	case EKeyWords::FOR:
		result += "for";
		break;
	case EKeyWords::TO:
		result += "to";
		break;
	case EKeyWords::DOWNTO:
		result += "downto";
		break;
	case EKeyWords::REPEAT:
		result += "repeat";
		break;
	case EKeyWords::UNTIL:
		result += "until";
		break;
	default:
		result = "[ERROR] Unknown keyword!";
		break;
	}

	return result;
}
