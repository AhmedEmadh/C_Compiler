/*
 ============================================================================
 File Name		: Token.h
 Author			: Ahmed Emad Hassan
 Version		: 1.0
 Date			: 24/3/2023
 Description	: This file contains The Token type (Class) carries the string of the token, id, type, number and Token constructor
 Notes			: the number of the token is the number of similar tokens before it
				  Example: 0 if first token, 1 if repeated once, 2 if repeated twice and so on
 ============================================================================
 */


#pragma once
#include <string>
#include <vector>
#include "TokenType.h"
class Token {
public:
	int id;
	std::string value;
	TokenType type;
	int number;
	Token(int id,std::string value,TokenType type,int number);
};

