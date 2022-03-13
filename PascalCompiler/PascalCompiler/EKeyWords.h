#pragma once
#ifndef KEY_WORDS_H
#define KEY_WORDS_H

enum class EKeyWords
{
	// types
	INTEGER,
	REAL,
	STRING,
	BOOLEAN,
	// arithmetic operators
	AOP_ASSIGN,
	AOP_SUM,
	AOP_SUB,
	AOP_MULT,
	AOP_DIV,
	// logical operators
	LOP_AND,
	LOP_OR,
	LOP_NOT,
	LOP_XOR,
	// comparison operators
	COP_LT,
	COP_LE,
	COP_GT,
	COP_GE,
	COP_EQ,
	COP_NE,
	// rest keywords
	END_OF_STATEMENT,
	BEGIN,
	END,
	VAR,
	TYPE,
	IF,
	THEN,
	ELSE,
	WHILE,
	DO,
	COLON,
	OPENING_BRACKET,
	CLOSING_BRACKET,
	SINGLE_QUOTE,
	COMMA,
	// individual 
	FUNCTION,
	FOR,
	TO,
	DOWNTO,
	REPEAT,
	UNTIL,
};

#endif // !_KEY_WORDS_H_
