#pragma once
#ifndef _KEY_WORDS_H_
#define _KEY_WORDS_H_

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
	DO
};

#endif // !_KEY_WORDS_H_
