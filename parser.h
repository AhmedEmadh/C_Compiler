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
#include "Token.h"
#include "TreeNode.h"
#include "Lexer.h"


#include <graphviz/gvc.h>
#include <graphviz/gvplugin.h>
#include <graphviz/cgraph.h>
#include <graphviz/cdt.h>

#include <fstream> // Include the necessary header files
using namespace std;


class Parser {
private:
    std::vector<Token> tokens;
    Token current_token;
    int token_index = 0;
    bool error = false;

    // Helper functions
    void next_token();
    bool match(std::string expected);
    void error_message(std::string expected);

    // Recursive descent functions
    TreeNode* program();
    TreeNode* declaration_list();
    TreeNode* declaration();
    TreeNode* variable_declaration();
    TreeNode* identifier_list();
    TreeNode* data_type();
    TreeNode* function_declaration();
    TreeNode* parameter_list();
    TreeNode* parameter();
    TreeNode* compound_statement();
    TreeNode* statement_list();
    TreeNode* statement();
    TreeNode* expression_statement();
    TreeNode* if_statement();
    TreeNode* while_statement();
    TreeNode* for_statement();
    TreeNode* return_statement();
    TreeNode* expression();
    TreeNode* assignment_expression();
    TreeNode* logical_or_expression();
    TreeNode* logical_and_expression();
    TreeNode* equality_expression();
    TreeNode* relational_expression();
    TreeNode* additive_expression();
    TreeNode* multiplicative_expression();
    TreeNode* primary_expression();
    TreeNode* function_call();
    TreeNode* argument_list();
    TreeNode* identifier();
    TreeNode* literal();
    TreeNode* number();
    TreeNode* string();

    
public:
    Parser(std::vector<Token> tokens);//file path
    void setFilePath(std::string inputFilePath);
    void setInput(std::string input); //string

    TreeNode* parse();   
    TreeNode* recursively_parse();
    void visualizeParseTree(TreeNode* root);
    void visualize_parse_tree_aux(TreeNode* node, std::ostream& out);
    void print_error();
    bool has_error();
};
