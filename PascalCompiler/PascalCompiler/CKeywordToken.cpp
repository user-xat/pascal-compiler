#include "CKeywordToken.h"

CKeywordToken::CKeywordToken(EKeyWords keyword) : CToken(ETokenType::KEYWORD)
{
	this->keyword = keyword;
}

EKeyWords CKeywordToken::GetKeyword()
{
	return this->keyword;
}

std::string CKeywordToken::ToString()
{
	std::string result;

	switch (keyword)
	{
	case EKeyWords::INTEGER:
		result = "integer";
		break;
	case EKeyWords::REAL:
		result = "real";
		break;
	case EKeyWords::STRING:
		result = "string";
		break;
	case EKeyWords::BOOLEAN:
		result = "boolean";
		break;
	case EKeyWords::AOP_ASSIGN:
		result = ":=";
		break;
	case EKeyWords::AOP_SUM:
		result = "+";
		break;
	case EKeyWords::AOP_SUB:
		result = "-";
		break;
	case EKeyWords::AOP_MULT:
		result = "*";
		break;
	case EKeyWords::AOP_DIV:
		result = "/";
		break;
	case EKeyWords::LOP_AND:
		result = "and";
		break;
	case EKeyWords::LOP_OR:
		result = "or";
		break;
	case EKeyWords::LOP_NOT:
		result = "not";
		break;
	case EKeyWords::LOP_XOR:
		result = "xor";
		break;
	case EKeyWords::COP_LT:
		result = "<";
		break;
	case EKeyWords::COP_LE:
		result = "<=";
		break;
	case EKeyWords::COP_GT:
		result = ">";
		break;
	case EKeyWords::COP_GE:
		result = ">=";
		break;
	case EKeyWords::COP_EQ:
		result = "=";
		break;
	case EKeyWords::COP_NE:
		result = "<>";
		break;
	case EKeyWords::END_OF_STATEMENT:
		result = ";";
		break;
	case EKeyWords::BEGIN:
		result = "begin";
		break;
	case EKeyWords::END:
		result = "end";
		break;
	case EKeyWords::VAR:
		result = "var";
		break;
	case EKeyWords::TYPE:
		result = "type";
		break;
	case EKeyWords::IF:
		result = "if";
		break;
	case EKeyWords::THEN:
		result = "then";
		break;
	case EKeyWords::ELSE:
		result = "else";
		break;
	case EKeyWords::WHILE:
		result = "while";
		break;
	case EKeyWords::DO:
		result = "do";
		break;
	default:
		break;
	}

	return result;
}
