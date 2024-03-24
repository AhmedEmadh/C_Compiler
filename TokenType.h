/*
 ============================================================================
 File Name		: TokenType.h
 Author			: Ahmed Emad Hassan
 Version		: 1.0
 Date			: 24/3/2023
 Description	: This file containes Token Types used by other classes
 Notes			: This file included by Lexer and Token Class
 ============================================================================
 */


#pragma once
enum class TokenType {
	Identifier, Number, String, Char, Operator, Reserved,SpecialChar
};