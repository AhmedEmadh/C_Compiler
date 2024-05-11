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

// Nodes in the parse tree can be represented by this very very very minimal class
class parse_tree_node {
public:
    bool is_terminal;
    std::string label;
    std::deque<parse_tree_node*> children;
    parse_tree_node(std::string l, bool t) : label(l), is_terminal(t) {}
};

class Parser {
private:
    Lexer lexer;
    Token current_token;
    int token_index = 0;
    std::vector<Token> tokens;
    parse_tree_node* root;
    parse_tree_node* current_node;
    bool error = false;

    // Helper functions
    void next_token();
    bool match(std::string expected);
    void error_message(std::string expected);

    // Recursive descent functions
    parse_tree_node* program();
    parse_tree_node* declaration_list();
    parse_tree_node* declaration();
    parse_tree_node* variable_declaration();
    parse_tree_node* identifier_list();
    parse_tree_node* data_type();
    parse_tree_node* function_declaration();
    parse_tree_node* parameter_list();
    parse_tree_node* parameter();
    parse_tree_node* compound_statement();
    parse_tree_node* statement_list();
    parse_tree_node* statement();
    parse_tree_node* expression_statement();
    parse_tree_node* if_statement();
    parse_tree_node* while_statement();
    parse_tree_node* for_statement();
    parse_tree_node* return_statement();
    parse_tree_node* expression();
    parse_tree_node* assignment_expression();
    parse_tree_node* logical_or_expression();
    parse_tree_node* logical_and_expression();
    parse_tree_node* equality_expression();
    parse_tree_node* relational_expression();
    parse_tree_node* additive_expression();
    parse_tree_node* multiplicative_expression();
    parse_tree_node* primary_expression();
    parse_tree_node* function_call();
    parse_tree_node* argument_list();
    parse_tree_node* identifier();
    parse_tree_node* literal();
    parse_tree_node* number();
    parse_tree_node* string();

public:
    Parser(std::string input);
    void parse();
    void print_parse_tree();
    void print_error();
    bool has_error();
};
