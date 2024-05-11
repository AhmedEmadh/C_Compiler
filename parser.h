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
#include "TreeNode.h"

using namespace std;


class Parser {
private:
    Lexer lexer;
    Token current_token;
    int token_index = 0;
    std::vector<Token> tokens;
    TreeNode::treeNode* root;
    treeNode* current_node; //will I be needing this? i think not
    bool error = false;

    // Helper functions
    void next_token();
    bool match(std::string expected);
    void error_message(std::string expected);

    // Recursive descent functions
    treeNode* program();
    treeNode* declaration_list();
    treeNode* declaration();
    treeNode* variable_declaration();
    treeNode* identifier_list();
    treeNode* data_type();
    treeNode* function_declaration();
    treeNode* parameter_list();
    treeNode* parameter();
    treeNode* compound_statement();
    treeNode* statement_list();
    treeNode* statement();
    treeNode* expression_statement();
    treeNode* if_statement();
    treeNode* while_statement();
    treeNode* for_statement();
    treeNode* return_statement();
    treeNode* expression();
    treeNode* assignment_expression();
    treeNode* logical_or_expression();
    treeNode* logical_and_expression();
    treeNode* equality_expression();
    treeNode* relational_expression();
    treeNode* additive_expression();
    treeNode* multiplicative_expression();
    treeNode* primary_expression();
    treeNode* function_call();
    treeNode* argument_list();
    treeNode* identifier();
    treeNode* literal();
    treeNode* number();
    treeNode* string();

    // Recursive descent parser implementation, should return the root node of the parse tree
    treeNode* recursively_parse();
    
public:
    Parser(std::string inputFilePath);//file path
    Parser();
    void setFilePath(std::string inputFilePath);
    void setInput(std::string input); //string

    void parse();   
    void print_parse_tree();
    void print_error();
    bool has_error();
};
