//this code is implimintation of the parser(syntax analyzer) according to grammar.txt
//we will be using Recursive Descent with Backtracking Implementation

/*
Grammar:
program            -> declaration_list

declaration_list   -> declaration declaration_list
                    | declaration

declaration        -> variable_declaration
                    | function_declaration

variable_declaration -> data_type identifier_list ;

identifier_list    -> identifier , identifier_list
                    | identifier

data_type          -> int
                    | double
                    | string
                    | float

function_declaration -> data_type identifier ( parameter_list ) compound_statement

parameter_list     -> parameter , parameter_list
                    | parameter

parameter          -> data_type identifier

compound_statement -> { statement_list }

statement_list     -> statement statement_list
                    | statement

statement          -> expression_statement
                    | if_statement
                    | while_statement
                    | for_statement
                    | return_statement
                    | declaration

expression_statement -> expression ;

if_statement       -> if ( expression ) statement
                    | if ( expression ) statement else statement

while_statement   -> while ( expression ) statement

for_statement     -> for ( expression_statement ; expression ; expression_statement ) statement

return_statement  -> return expression ;

expression         -> assignment_expression

assignment_expression -> logical_or_expression
                        | logical_or_expression = assignment_expression

logical_or_expression -> logical_and_expression
                        | logical_or_expression || logical_and_expression

logical_and_expression -> equality_expression
                         | logical_and_expression && equality_expression

equality_expression -> relational_expression
                      | equality_expression == relational_expression
                      | equality_expression != relational_expression

relational_expression -> additive_expression
                         | relational_expression < additive_expression
                         | relational_expression > additive_expression
                         | relational_expression <= additive_expression
                         | relational_expression >= additive_expression

additive_expression -> multiplicative_expression
                       | additive_expression + multiplicative_expression
                       | additive_expression - multiplicative_expression

multiplicative_expression -> primary_expression
                             | multiplicative_expression * primary_expression
                             | multiplicative_expression / primary_expression
                             | multiplicative_expression % primary_expression

primary_expression -> identifier
                     | literal
                     | ( expression )
                     | function_call

function_call      -> identifier ( argument_list )

argument_list      -> expression , argument_list
                     | expression

identifier         -> [a-zA-Z_][a-zA-Z0-9]*
literal            -> number | string

number             -> [0-9]+(\.[0-9]+)?([eE][-+]?[0-9]+)?
string             -> "([^"\\]*(\\.)*[^"\\]*)*"


*/
#pragma once
#include <string>
#include <iostream>
#include <regex>
#include <vector>
#include "Token.h"
#include "Lexer.h"

using namespace std;

class Parser {
private:
    vector<Token> tokens;
    int i = 0;
    Token currentToken;
    Token nextToken;
    bool error = false;
    bool isDataType();
    bool isIdentifier();
    bool isLiteral();
    bool isOperator();
    bool isSpecialChar();
    bool isReserved();
    bool isNumber();
    bool isString();
    bool isExpression();
    bool isAssignmentExpression();
    bool isLogicalOrExpression();
    bool isLogicalAndExpression();
    bool isEqualityExpression();
    bool isRelationalExpression();
    bool isAdditiveExpression();
    bool isMultiplicativeExpression();
    bool isPrimaryExpression();
    bool isFunctionCall();
    bool isArgumentList();
    bool isIdentifierList();
    bool isParameterList();
    bool isCompoundStatement();
    bool isStatementList();
    bool isStatement();
    bool isExpressionStatement();
    bool isIfStatement();
    bool isWhileStatement();
    bool isForStatement();
    bool isReturnStatement();
    bool isDeclaration();
    bool isVariableDeclaration();
    bool isFunctionDeclaration();
    bool isProgram();
    void match(string tokenType);
    void match(Token token);
    void match();
    void next();
    void next(int n);
    void error_msg(string msg);
    void error_msg(string msg, Token token);
    void error_msg(string msg, string tokenType);
    void error_msg(string msg, string tokenType, Token token);
   

public:
    Parser(vector<Token> tokens);
    void parse();
    bool isError();
};

