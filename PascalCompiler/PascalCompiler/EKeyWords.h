#pragma once
#ifndef _KEY_WORDS_H_
#define _KEY_WORDS_H_

enum EKeyWords
{
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
	COP_LTE,
	COP_GT,
	COP_GTE,
	COP_EQ,
	COP_NEQ,
	// rest keywords
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
