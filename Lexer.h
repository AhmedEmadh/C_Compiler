/*
 ============================================================================
 File Name		: Lexer.h
 Author			: Ahmed Emad Hassan
 Version		: 1.1
 Date			: 14/3/2023
 Description	: This file containes a Class that have the implementation of lexer
 Notes			: The lexer takes input string that contains C language or file that containes the language
				  Example of file Name: main.c, header.h
				  Example of string contains C language "int main(){retrun 0;}"
				  It also detects errors when there is '@' symbol or variable starts with a number
 ============================================================================
 */

#pragma once
#include <string>
#include <iostream>
#include <regex>
#include <vector>
#include <queue>
#include <stack>
#include "Token.h"
using namespace std;
class Lexer {
private:
	int i = 0;
	string input;
	vector<string> tokens;
	queue<char> Qchar;
	enum State {start,num,id,lineComment,multiLineComment,error,stringInput,reserved,hash,operatorInput,specialCharacter} state;
	vector<string> reserved_words = {"auto",	"else",		"long",		"switch"
									"break",	"enum",		"register",	"typedef",
									"case",		"extern",	"return",	"union",
									"char",		"float",	"short",	"unsigned",
									"const",	"for",		"signed",	"void",
									"continue",	"goto",		"sizeof",	"volatile",
									"default",	"if",		"static",	"while",
									"do",		"int",		"struct",	"_Packed",	"double"};
	vector<char> first_char_operators = { '+','-','*','/'/*no two slashes out string*/,'%','>','<','=','&','|','!','~','^','?',':','.'};
	vector<char> special_characters = { '(', ')', '[', ']', '{', '}', ',', ';' };

	/* nameing values */
	vector<string> symbolTable;
	vector<Token> Tokens;
	int TokenNumber(vector<string>& tokens);
	int GetTokenNumber(string Token,int id);
	//checker functions
	bool is_letter(char c);
	bool is_number(char c);
	bool is_bin_number(char c);
	bool is_oct_number(char c);
	bool is_hex_number(char c);
	bool is_whitespace(char c);
	bool is_reserved(string str);
	bool is_operator(char c);
	bool is_specialChar(char c);
	bool is_exp(char c);
	bool is_sign(char c);

	bool is_identifier(string token);
	bool is_operator(string token);
	bool is_specialChar(string token);
	bool is_number(string token);
	//state functions
	void state_start();
	void state_num();
	void state_id();
	void state_lineComment();
	void state_multiLineComment();
	void state_error();
	void state_string();
	void state_reserved();
	void state_hash();
	void state_operator();
	void state_specialCharacter();
	//operation functions
	template <typename T>
	T pop(stack<T>& s);
	template <typename T>
	T pop(queue<T>& q);
	string getStringFromCharQueue(queue<char>& qchar);
	
public:
	Lexer(std::string local_input);
	Lexer();
	void setInput(string local_input);
	void setFilePath(const string& filePath);
	void scan();
	//getter functions
	vector<string> getTokens();
	//print function
	void PrintOutput();
	//get objects
	void createObjectTokens();
	vector<Token> getObjectTokens();
	void printObjectTokens();
};
