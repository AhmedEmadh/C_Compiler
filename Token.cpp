/*
 ============================================================================
 File Name		: Token.cpp
 Author			: Ahmed Emad Hassan
 Version		: 1.0
 Date			: 24/3/2023
 Description	: This file contains The Implementation of Token Class methods
 Notes			: The class prototypes and members are in the "Token.h" file
 ============================================================================
 */


#include "Token.h"

Token::Token(int id, std::string value, TokenType type, int number) {
	this->id = id;
	this->value = value;
	this->type = type;
	this->number = number;
}