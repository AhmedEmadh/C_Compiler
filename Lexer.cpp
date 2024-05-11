/*
 ============================================================================
 File Name		: Lexer.cpp
 Author			: Ahmed Emad Hassan
 Version		: 1.1
 Date			: 14/3/2023
					
 Description	: This file containes a Class that has the implementation of lexer
 Notes			: The lexer takes input string that contains C language or file that containes the language.
				  Example of file Name: main.c, header.h
				  Example of string contains C language: "int main(){retrun 0;}"
				  It also detects errors when there is '@' symbol or variable starts with a number
 ============================================================================
 */


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Lexer.h"
#include "Token.h"
#include "TokenType.h"
template <typename T>
T Lexer::pop(queue<T>& q) {
	T temp = q.front();
	q.pop();
	return temp;
}
template <typename T>
T Lexer::pop(stack<T>& s) {
	T temp = s.top();
	s.pop();
	return temp;
}

string Lexer::getStringFromCharQueue(queue<char>& qchar) {
	string result;
	//empty the queue into the sting
	while (!qchar.empty()) {
		result += pop(qchar);
	}

	return result;
}
bool Lexer::is_letter(char c) {//tested
	bool result = false;
	if ((c >= 'a' && c <= 'z')|| (c >= 'A' && c <= 'Z')) {
		result = true;
	}
	else if (c == '_') {
		result = true;
	}
	else {
		result = false;
	}

	return result;
}
bool Lexer::is_number(char c) {//tested
	bool result = false;
	if ((c>='0')&&(c<='9')) {
		result = true;
	}
	else {
		result = false;
	}
	return result;
}
bool Lexer::is_bin_number(char c) {
	bool result = false;
	if (c == '0' || c == '1') {
		result = true;
	}
	else {
		result = false;
	}
	return result;
}
bool Lexer::is_oct_number(char c) {
	bool result = false;
	if (c >= '0' && c <= '7') {
		result = true;
	}
	else {
		result = false;
	}
	return result;
}
bool Lexer::is_hex_number(char c) {
	bool result = false;
	if (is_number(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) {
		result = true;
	}
	else {
		result = false;
	}
	return result;
}
bool Lexer::is_whitespace(char c) {//tested
	bool result = false;
	if ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\r')) {
		return true;
	}
	else {
		return false;
	}
	return result;
}
bool Lexer::is_operator(char c) {
	bool result = false;
	for (char Operator : first_char_operators) {
		if (c == Operator) {
			result = true;
			break;
		}
	}
	return result;
}
bool Lexer::is_specialChar(char c) {
	bool result = false;
	for (char special_char: special_characters) {
		if (c == special_char) {
			result = true;
		}
	}
	return result;
}
bool Lexer::is_exp(char c) {
	bool result = false;
	if ((c == 'e' || c == 'E')) {
		result = true;
	}
	else {
		result = false;
	}
	return result;
}
bool Lexer::is_sign(char c) {
	bool result = false;
	if (c == '+' || c == '-') {
		result = true;
	}
	else {
		result = false;
	}
	return result;
}
vector<string> Lexer::getTokens() {
	return tokens;
}
Lexer::Lexer(std::string local_input) {
	setFilePath(local_input);
	this->input = this->input + "          ";
	state = State::start;
}
Lexer::Lexer() {
	input = "";
	state = State::start;
}
bool Lexer::is_reserved(string str) {
	bool result = false;
	for (string reserved_word : reserved_words) {
		if (str == reserved_word) {
			result = true;
			break;
		}
	}
	return result;
}
void Lexer::setInput(string local_input) {
	this->input = local_input;
	this->input = this->input + "          ";
}
void Lexer::setFilePath(const string& filePath) {
	ifstream file(filePath);
	stringstream buffer;

	if (file) {
		buffer << file.rdbuf();
		file.close();
		input = buffer.str();
	}
	else {
		cerr << "Error: Could not open file " << filePath << endl;
		input = "";
	}
}
void Lexer::scan() {
	i = 0;
	while(input[i] != '\0') {
		switch (state) {
		case Lexer::start:
			state_start();
			break;
		case Lexer::num:
			state_num();
			break;
		case Lexer::id:
			state_id();
			break;
		case Lexer::lineComment:
			state_lineComment();
			break;
		case Lexer::multiLineComment:
			state_multiLineComment();
			break;
		case Lexer::error:
			state_error();
			break;
		case Lexer::stringInput:
			state_string();
			break;
		case Lexer::reserved:
			state_reserved();
			break;
		case Lexer::hash:
			state_hash();
			break;
		case Lexer::operatorInput:
			state_operator();
			break;
		case Lexer::specialCharacter:
			state_specialCharacter();
			break;
		default:
			break;
		}
	}
	createObjectTokens();
}

void Lexer::PrintOutput() {
	cout << "The tokens are: " << endl;
	for (string token : tokens) {
		cout << token <<endl;
	}
}
int Lexer::TokenNumber(vector<string>& tokens) {
	return 0;
}
/* States */
void Lexer::state_start() {
	if (is_letter(input[i]) || input[i] == '_') {//if letter found then go to state letter
		state = State::id;
	}
	else if (is_number(input[i])) {//if number is found then go to state number
		state = State::num;
	}
	else if (is_whitespace(input[i])) {//if white space found stay in start state and advance to the next character
		//advance and stay in the same state
		i++;
	}
	else if ((input[i] == '\"')|| (input[i] == '\'')) {//when string or char starting char found go to string state
		state = State::stringInput;
	}
	else if (input[i] == '@') {//if @ is found at any moment in the code then go to error state
		state = State::error;
	}
	else if ((input[i] == '/')&&(input[i+1] == '*')) {//when the multi line comment start found
		state = State::multiLineComment;
	}
	else if ((input[i] == '/') && ((input[i + 1] == '/'))) {//when the line comment start found
		state = State::lineComment;
	}
	else if (input[i] == '#') {
		state = State::hash;
	}
	else if (is_operator(input[i])) {
		state = State::operatorInput;
	}
	else if (is_specialChar(input[i])) {
		state = State::specialCharacter;
	}
	else {//if unknown char is found then go to error state
		state = State::error;
	}
}
void Lexer::state_num(){
	static enum NumberType { DEC, BIN, OCT, HEX } number_type = NumberType::DEC;
	static bool is_first_char = true;
	if (input[i] == '0' && is_first_char) {
		if (is_number(input[i+1])) {//octa
			Qchar.push(input[i]);
			i++;
			is_first_char = false;
			number_type = NumberType::OCT;
		}
		else if (input[i + 1] == 'x' || input[i + 1] == 'X') {//Hexa
			Qchar.push(input[i]);
			i++;
			is_first_char = false;
			Qchar.push(input[i]);
			i++;
			number_type = NumberType::HEX;
		}
		else if (input[i + 1] == 'b' || input[i + 1] == 'B') {//Binary
			Qchar.push(input[i]);
			i++;
			is_first_char = false;
			Qchar.push(input[i]);
			i++;
			number_type = NumberType::BIN;
		}
	}
	//taking numbers
	if ((number_type == DEC)&&is_number(input[i]))
	{

		if (is_number(input[i]) && is_exp(input[i + 1]) && ((is_sign(input[i + 2]) && is_number(input[i + 3])) || is_number(input[i + 2]))) {
			Qchar.push(input[i]);
			i++;
			is_first_char = false;
			Qchar.push(input[i]);
			i++;
			Qchar.push(input[i]);
			i++;
		}
		while (is_number(input[i])) {
			Qchar.push(input[i]);
			i++;
			is_first_char = false;
			if (is_exp(input[i])) {
				if (is_sign(input[i+1])) {
					Qchar.push(input[i]);
					i++;
				}
				Qchar.push(input[i]);
				i++;

			}
		}
		if (is_bin_number(input[i]) || is_hex_number(input[i]) || is_oct_number(input[i]) || is_letter(input[i])) {
			state = State::error;
			number_type = NumberType::DEC;
			is_first_char = true;
			return;
		}

	}else if (number_type == BIN && is_bin_number(input[i]))
	{
		while (is_bin_number(input[i])) {
			Qchar.push(input[i]);
			i++;
			is_first_char = false;
		}
		if (is_number(input[i]) || is_hex_number(input[i]) || is_oct_number(input[i]) || is_letter(input[i])) {
			state = State::error;
			number_type = NumberType::DEC;
			is_first_char = true;
			return;
		}

	}else if(number_type == OCT && is_oct_number(input[i]) ) {
		while (is_oct_number(input[i])) {
			Qchar.push(input[i]);
			i++;
			is_first_char = false;
		}
		if (is_number(input[i]) || is_hex_number(input[i]) || is_bin_number(input[i]) || is_letter(input[i])) {
			state = State::error;
			number_type = NumberType::DEC;
			is_first_char = true;
			return;
		}


		
	}else if (number_type == HEX && is_hex_number(input[i])) {
		while (is_hex_number(input[i])) {
			Qchar.push(input[i]);
			i++;
			is_first_char = false;
		}
		if (is_number(input[i]) || is_bin_number(input[i]) || is_oct_number(input[i]) || is_letter(input[i])) {
			state = State::error;
			number_type = NumberType::DEC;
			is_first_char = true;
			return;
		}


	}else if (is_letter(input[i])) {
		state = State::error;
	}else{
		tokens.push_back(getStringFromCharQueue(Qchar));
		is_first_char = true;
		number_type = NumberType::DEC;
		state = State::start;
	}
}
void Lexer::state_id(){
	if (is_letter(input[i]) || input[i] == '_') {
		Qchar.push(input[i]);
		i++;
	}
	else if (is_number(input[i])) {
		Qchar.push(input[i]);
		i++;
	}else{
		tokens.push_back(getStringFromCharQueue(Qchar));
		if (is_reserved(tokens.back())) {
			state = State::reserved;
		}
		else {
			state = State::start;
		}
	}
	
}
void Lexer::state_lineComment(){
	if (input[i] == '/' && input[i+1] == '/') {
		while ((input[i] != '\n') && (input[i] != '\0')) {
			i++;
		}
	}
	state = State::start;
}
void Lexer::state_multiLineComment(){
	if (input[i] == '/' && input[i + 1] == '*') {
		while ((input[i] != '*')||(input[i+1] != '/')) {
			i++;
		}
		if ((input[i] == '*') && (input[i + 1] == '/')) {
			i += 2;
		}
		state = State::start;
	}
}
void Lexer::state_error(){
	static bool is_passed_once = false;
	if (is_passed_once == false) {
		cout << "error occured";
		is_passed_once = true;
	}
	else {
		while (1);
	}

}
void Lexer::state_string(){
	if (input[i] == '\"') {//string
		do{
			Qchar.push(input[i]);
			i++;
		}while (input[i] != '\"');
		Qchar.push(input[i]);
		i++;
		tokens.push_back(getStringFromCharQueue(Qchar));
		state = State::start;
	}
	else if (input[i] == '\'') {//charater
			Qchar.push(input[i]);
			i++;
			if (input[i] == '\\') {
				Qchar.push(input[i]);
				i++;
			}
			//for after slash
			Qchar.push(input[i]);
			i++;
			if (input[i] == '\'') {
				Qchar.push(input[i]);
				i++;
				tokens.push_back(getStringFromCharQueue(Qchar));
				state = State::start;
			}else{
				state = State::error;
			}
	}else {
		state = State::error;
	}
}
void Lexer::state_reserved() {
	state = State::start;
}
void Lexer::state_hash() {
	if (input[i] == '#') {
		while ((input[i] != '\n')&& (input[i] != '\0')) {
			i++;
		}
		if (input[i] != '\0') {
			i++;
		}
		state = State::start;
	}
	else {
		state = State::error;
	}
}
void Lexer::state_operator() {
	if (input[i] == '+' && input[i + 1] == '+') 
	{// ++
		Qchar.push(input[i]);
		i++;
		Qchar.push(input[i]);
		i++;
		tokens.push_back(getStringFromCharQueue(Qchar));
		state = State::start;
	}
	else if (input[i] == '-' && input[i + 1] == '-') 
	{// --
		Qchar.push(input[i]);
		i++;
		Qchar.push(input[i]);
		i++;
		tokens.push_back(getStringFromCharQueue(Qchar));
		state = State::start;
	}
	else if (input[i] == '<' && input[i + 1] == '=') 
	{// <=
		Qchar.push(input[i]);
		i++;
		Qchar.push(input[i]);
		i++;
		tokens.push_back(getStringFromCharQueue(Qchar));
		state = State::start;
	}
	else if (input[i] == '>' && input[i + 1] == '=') 
	{// >=
		Qchar.push(input[i]);
		i++;
		Qchar.push(input[i]);
		i++;
		tokens.push_back(getStringFromCharQueue(Qchar));
		state = State::start;
	}
	else if (input[i] == '=' && input[i + 1] == '=')
	{// ==
		Qchar.push(input[i]);
		i++;
		Qchar.push(input[i]);
		i++;
		tokens.push_back(getStringFromCharQueue(Qchar));
		state = State::start;
	}
	else if (input[i] == '!' && input[i + 1] == '=')
	{// !=
		Qchar.push(input[i]);
		i++;
		Qchar.push(input[i]);
		i++;
		tokens.push_back(getStringFromCharQueue(Qchar));
		state = State::start;
	}
	else if (input[i] == '&' && input[i + 1] == '&')
	{// &&
		Qchar.push(input[i]);
		i++;
		Qchar.push(input[i]);
		i++;
		tokens.push_back(getStringFromCharQueue(Qchar));
		state = State::start;
	}
	else if (input[i] == '|' && input[i + 1] == '|')
	{// ||
		Qchar.push(input[i]);
		i++;
		Qchar.push(input[i]);
		i++;
		tokens.push_back(getStringFromCharQueue(Qchar));
		state = State::start;
	}
	else if (input[i] == '<' && input[i + 1] == '<')
	{// <<
		Qchar.push(input[i]);
		i++;
		Qchar.push(input[i]);
		i++;
		tokens.push_back(getStringFromCharQueue(Qchar));
		state = State::start;
	}
	else if (input[i] == '>' && input[i + 1] == '>')
	{// >>
		Qchar.push(input[i]);
		i++;
		Qchar.push(input[i]);
		i++;
		tokens.push_back(getStringFromCharQueue(Qchar));
		state = State::start;
	}
	else if (input[i] == '+' && input[i + 1] == '=')
	{// +=
		Qchar.push(input[i]);
		i++;
		Qchar.push(input[i]);
		i++;
		tokens.push_back(getStringFromCharQueue(Qchar));
		state = State::start;
	}
	else if (input[i] == '-' && input[i + 1] == '=')
	{// -=
		Qchar.push(input[i]);
		i++;
		Qchar.push(input[i]);
		i++;
		tokens.push_back(getStringFromCharQueue(Qchar));
		state = State::start;
	}
	else if (input[i] == '*' && input[i + 1] == '=')
	{// *=
		Qchar.push(input[i]);
		i++;
		Qchar.push(input[i]);
		i++;
		tokens.push_back(getStringFromCharQueue(Qchar));
		state = State::start;
	}
	else if (input[i] == '/' && input[i + 1] == '=')
	{// /=
		Qchar.push(input[i]);
		i++;
		Qchar.push(input[i]);
		i++;
		tokens.push_back(getStringFromCharQueue(Qchar));
		state = State::start;
	}
	else if (input[i] == '%' && input[i + 1] == '=')
	{// %=
		Qchar.push(input[i]);
		i++;
		Qchar.push(input[i]);
		i++;
		tokens.push_back(getStringFromCharQueue(Qchar));
		state = State::start;
	}
	else if (input[i] == '-' && input[i + 1] == '>')
	{// ->
		Qchar.push(input[i]);
		i++;
		Qchar.push(input[i]);
		i++;
		tokens.push_back(getStringFromCharQueue(Qchar));
		state = State::start;
	}
	else if (is_operator(input[i]))
	{//single operator
		Qchar.push(input[i]);
		i++;
		tokens.push_back(getStringFromCharQueue(Qchar));
	}


	else {//error
		state = State::error;
	}

	state = State::start;
}
void Lexer::state_specialCharacter() {
	Qchar.push(input[i]);
	i++;
	tokens.push_back(getStringFromCharQueue(Qchar));
	state = State::start;
}

int Lexer::GetTokenNumber(string Token,int id) {
	int counter = 0;
	for (int i = 0; i < id;i++) {
		if (Token == tokens[i]) {
			counter++;
		}
	}
	return counter;
}
bool Lexer::is_identifier(string token) {
	bool result = true;
	int i = 0;
	for (char letter : token) {
		if (i == 0 && is_letter(letter)) {
			//nothing
		}
		else if(i != 0 && (is_letter(letter)||is_number(letter))) {
			//nothing
		}
		else {
			/* not a letter or a number */
			result = false;
			return result;
		}
		i++;
	}
	return result;
}
bool is_string(string token)
{
	if (token[0] == '\"' && token[token.length() - 1] == '\"') {
			return true;
		} 
		else {
			return false;
		}
}
bool is_char(string token) {
	for (int i = 0; i < token.length(); i++) {
		if (i == 0 && token[i] == '\'') {
			//do nothing
		}
		else if (i == token.length() - 1 && token[i] == '\'') {
			//do nothing
		}
		else {
			return false;
		}
	}
	return true;
}
bool Lexer::is_number(string token) {
	bool result = false;
	if (token[0]> '0' && token[0]<='9') {
		result = true;
	}
	else {
		result = false;
	}
	return result;
}
bool Lexer::is_operator(string token) {
	bool result = false;
	for (char c : first_char_operators) {
		if (c == token[0]) {
			result = true;
		}
	}
	return result;
}
bool Lexer::is_specialChar(string token) {
	bool result = false;
	for (char c : special_characters) {
		if (c == token[0]) {
			result = true;
		}
	}
	return result;
}
void Lexer::createObjectTokens(){
	int id = 0;
	for (string local_token : tokens) {
		if (is_reserved(local_token))
		{
			Tokens.push_back(Token(id, local_token, TokenType::Reserved, GetTokenNumber(local_token,id)));
		}
		else if (is_identifier(local_token)) {
			Tokens.push_back(Token(id, local_token, TokenType::Identifier, GetTokenNumber(local_token, id)));
			//adding the identifier to the symbol table
			addTokenToSymbolTable(local_token, GetTokenNumber(local_token, id));
		}
		else if (is_string(local_token)) {
			// Number, Operator
			Tokens.push_back(Token(id, local_token, TokenType::String, GetTokenNumber(local_token, id)));
		}
		else if (is_char(local_token)) {
			Tokens.push_back(Token(id, local_token, TokenType::Char, GetTokenNumber(local_token, id)));
		}
		else if (is_number(local_token)) {
			Tokens.push_back(Token(id, local_token, TokenType::Number, GetTokenNumber(local_token, id)));
		}
		else if (is_operator(local_token)) {
			Tokens.push_back(Token(id, local_token, TokenType::Operator, GetTokenNumber(local_token, id)));
		}
		else if (is_specialChar(local_token)) {
			Tokens.push_back(Token(id, local_token, TokenType::SpecialChar, GetTokenNumber(local_token, id)));
		}
		id++;
	}
}

vector<Token> Lexer::getObjectTokens() {
	return Tokens;
}
void Lexer::printObjectTokens() {
	for (Token object_token : Tokens) {
		cout << "Token ID: " << object_token.id<<" ";
		cout << "Token Number: " << object_token.number<<" ";
		cout << "Token Type: ";
		if (object_token.type == TokenType::Char)
		{
			cout << "Char";
		}
		else if (object_token.type == TokenType::Identifier)
		{
			cout << "Identifier";
		}
		else if (object_token.type == TokenType::Number) {
			cout << "Number";
		}
		else if (object_token.type == TokenType::Operator)
		{
			cout << "Operator";
		}
		else if (object_token.type == TokenType::Reserved)
		{
			cout << "Reserved";
		}
		else if (object_token.type == TokenType::SpecialChar)
		{
			cout << "SpecialChar";
		}
		else if (object_token.type == TokenType::String)
		{
			cout << "String";
		}
		cout << "Token Value: " << object_token.value;
		cout << endl;
	}
}

//using hash to add to add iddentifier to symbol table
int Lexer::addTokenToSymbolTable(string tokenValue, int offset) {
	int index = 0;
		for (auto& symbol : symbolTable) {
			if (tokenValue == symbol.first) {
				symbol.second.push_back(offset);
				return index;
			}
			index++;
		}
		symbolTable.push_back({ tokenValue, { offset } });
		return index;
}

void Lexer::printSymbolTable() {
	for (pair<string, vector<int>> symbol : symbolTable) {
		cout << "Token: " << symbol.first << " ";
		cout << "Offsets: ";
		for (int offset : symbol.second) {
			cout << offset << " ";
		}
		cout << endl;
	}
}

string Lexer::getTokenIndexFromSymbolTable(int index) {
	if (index >= 0 && index < symbolTable.size()) {
		return symbolTable[index].first;
	}
	return "";
}

int Lexer::getTokenIndexFromSymbolTable(string value) {
	int i = 0;
	for (pair<string, vector<int>> symbol : symbolTable) {
		if (value == symbol.first) {
			return i;
		}
		i++;
	}
	return -1;
}