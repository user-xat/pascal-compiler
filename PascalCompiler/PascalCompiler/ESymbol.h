#pragma once
#ifndef ESYMBOL_H
#define ESYMBOL_H

enum class ESymbol
{
	//////////////////ETokenType//////////////
	IDENT_TOKEN,
	KEYWORD_TOKEN,
	CONST_TOKEN,
	//////////////////EVariantType////////////
	INTEGER_VARIANT,
	REAL_VARIANT,
	STRING_VARIANT,
	BOOLEAN_VARIANT,
	//////////////////ETokenType//////////////
	// types
	INTEGER_KEYWORD,
	REAL_KEYWORD,
	STRING_KEYWORD,
	BOOLEAN_KEYWORD,
	// arithmetic operators
	AOP_ASSIGN_KEYWORD,
	AOP_SUM_KEYWORD,
	AOP_SUB_KEYWORD,
	AOP_MULT_KEYWORD,
	AOP_DIV_REAL_KEYWORD,
	AOP_DIV_KEYWORD,
	AOP_MOD_KEYWORD,
	// logical operators
	LOP_AND_KEYWORD,
	LOP_OR_KEYWORD,
	LOP_NOT_KEYWORD,
	LOP_XOR_KEYWORD,
	// comparison operators
	COP_LT_KEYWORD,
	COP_LE_KEYWORD,
	COP_GT_KEYWORD,
	COP_GE_KEYWORD,
	COP_EQ_KEYWORD,
	COP_NE_KEYWORD,
	// rest keywords
	PROGRAM_KEYWORD,
	SEMICOLON_KEYWORD,
	BEGIN_KEYWORD,
	END_KEYWORD,
	VAR_KEYWORD,
	TYPE_KEYWORD,
	IF_KEYWORD,
	THEN_KEYWORD,
	ELSE_KEYWORD,
	WHILE_KEYWORD,
	DO_KEYWORD,
	COLON_KEYWORD,
	OPENING_BRACKET_KEYWORD,
	CLOSING_BRACKET_KEYWORD,
	SINGLE_QUOTE_KEYWORD,
	COMMA_KEYWORD,
	DOT_KEYWORD,
	// individual 
	FUNCTION_KEYWORD,
	FOR_KEYWORD,
	TO_KEYWORD,
	DOWNTO_KEYWORD,
	REPEAT_KEYWORD,
	UNTIL_KEYWORD,
};

#endif // !ESYMBOL_H
