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

#include <iostream>
#include "parser.h"
#include "Token.h"
#include "Lexer.h"




// implementation of the parser constructor
Parser::Parser(std::vector<Token> tokens) {
    // Set the tokens vector
    this->tokens = tokens;
    // Set the current token to the first token
    current_token = tokens[0];
    // Set the token index to 0
    token_index = 0;
    // Set the error flag to false
    error = false;

}

// implementation of next_token function
void Parser::next_token() {
    // Increment the token index
    token_index++;
    // Check if the token index is within the bounds of the tokens vector
    if (token_index < tokens.size()) {
        // Get the current token
        current_token = tokens[token_index];
    }
}

// implementation of match function to check if the current token matches the expected token and increment the token index
bool Parser::match(std::string expected) {
    // Check if the current token matches the expected token
    if (current_token.value == expected) {
        // Move to the next token
        next_token();
        return true;
    }
    return false;
}

// implementation of error_message function
void Parser::error_message(std::string expected) {
    // Print an error message
    std::cout << "Error: Expected " << expected << " at token " << current_token.value << "|| ID: " << current_token.id<< std::endl;
    // Set the error flag to true
    error = true;
}


// implementation of program function
TreeNode* Parser::program() {
    // Create a new root node
    TreeNode* root = new TreeNode("Program");
    // Add the declaration list node as a child of the root node
    root->addChild(declaration_list());
    // Return the root node
    return root;
}

// implementation of declaration_list function
//declaration_list   -> declaration declaration_list| declaration
TreeNode* Parser::declaration_list() {
    // Create a new declaration list node
    TreeNode* declaration_list_node = new TreeNode("Declaration List");

    // Continue adding declarations as long as the current token indicates a declaration
    while (true) {
        // Check if the current token indicates a declaration
        if (current_token.value == "int" || current_token.value == "double" || current_token.value == "string" || current_token.value == "float") {
            // Add the next declaration node as a child of the declaration list node
            declaration_list_node->addChild(declaration());
            next_token(); // Move to the next token
            
        } else {
            // No more declarations to parse, exit the loop
            break;
        }
    }

    // Return the declaration list node
    return declaration_list_node;
}



// implementation of declaration function
//declaration        -> variable_declaration | function_declaration
TreeNode* Parser::declaration() {
    // Create a new declaration node
    TreeNode* decl = new TreeNode("Declaration");
    // Check if the token is a data type
    if (match("int") || match("float") || match("char") || match("double")) {
        // // Add the data type node as a child of the declaration node
        // decl->addChild(data_type());

        //use match function to check for Identifier
        if ( current_token.type == TokenType::Identifier) {
            next_token();
            if (match("(")) {
                // Add the function declaration node as a child of the declaration node
                decl->addChild(function_declaration());
            } 
            else {
                // Add the variable declaration node as a child of the declaration node
                decl->addChild(variable_declaration());
            }
        } 
        else {
            // Error handling
            error_message("identifier");
        }
    } else if (match("void")) {
        // Add the data type node as a child of the declaration node
        decl->addChild(data_type());

        // Check if the next token is an identifier (for function declaration)
        if (current_token.type == TokenType::Identifier) {
            // Add the function declaration node as a child of the declaration node
            decl->addChild(function_declaration());
        } else {
            // Error handling
            error_message("identifier");
        }
    } else {
        // Error handling
        error_message("data type or void");
    }

    // Return the declaration node
    return decl;
}



// implementation of variable_declaration function
//variable_declaration -> data_type identifier_list ;
TreeNode* Parser::variable_declaration() {
    // Create a new variable declaration node
    TreeNode* var_decl = new TreeNode("Variable Declaration");

    // // Add the data type node as a child of the variable declaration node
    // var_decl->addChild(data_type());

    // Check if the next token is an identifier
    if (current_token.type == TokenType::Identifier) {
        // Add the identifier list node as a child of the variable declaration node
        var_decl->addChild(identifier_list());
    } else {
        // Error handling
        error_message("identifier");
    }

    // Check if the next token is a semicolon
    if (match(";")) {
        // Return the variable declaration node
        return var_decl;
    } else {
        // Error handling
        error_message("';'");
        return var_decl;
    }
}

// implementation of identifier_list function
//identifier_list    -> identifier , identifier_list | identifier
TreeNode* Parser::identifier_list() {
    // Create a new identifier list node
    TreeNode* id_list = new TreeNode("Identifier List");

    // Check if the current token is an identifier
    if (current_token.type == TokenType::Identifier) {
        // Add the identifier node as a child of the identifier list node
        id_list->addChild(identifier());

        // Check if the next token is a comma
        while (match(",")) {
            // Check if the next token is an identifier
            if (current_token.type == TokenType::Identifier) {
                // Add the identifier node as a child of the identifier list node
                id_list->addChild(identifier());
            } else {
                // Error handling
                error_message("identifier after comma");
                break; // Exit the loop to avoid infinite loop on error
            }
        }
    } else {
        // Error handling
        error_message("identifier");
    }

    // Return the identifier list node
    return id_list;
}

// implementation of data_type function
//data_type          -> int | double | string | float
TreeNode* Parser::data_type() {
    // Create a new data type node
    TreeNode* data_type_node = nullptr;

    // Check if the current token is a data type
    if (current_token.value == "int" || current_token.value == "double" || current_token.value == "string" || current_token.value == "float") {
        // Add the data type token as a child of the data type node
        data_type_node = new TreeNode("Data Type");
        data_type_node->addChild(new TreeNode(current_token.value));
        // Move to the next token
        next_token();
    } else {
        // Error handling
        error_message("int, double, string, or float");
    }

    // Return the data type node
    return data_type_node;
}

// implementation of function_declaration function
//function_declaration -> data_type identifier ( parameter_list ) compound_statement
TreeNode* Parser::function_declaration() {
    // Create a new function declaration node
    TreeNode* func_decl = new TreeNode("Function Declaration");

    // // Add the data type node as a child of the function declaration node
    // func_decl->addChild(data_type());

    // Check if the current token is an identifier
    if (current_token.type == TokenType::Identifier) {
        next_token();
        // Add the identifier node as a child of the function declaration node
        func_decl->addChild(identifier());
    } 
    else if(match(")")) {
        // Add the compound statement node as a child of the function declaration node
        func_decl->addChild(compound_statement());
        } 
    else
    {
        // Error handling
        error_message("')' or identifier");
    }

    // Return the function declaration node
    return func_decl;
}

// implementation of parameter_list function
//parameter_list     -> parameter , parameter_list | parameter
TreeNode* Parser::parameter_list() {
    // Create a new parameter list node
    TreeNode* param_list = new TreeNode("Parameter List");

    // Add the first parameter node as a child of the parameter list node
    param_list->addChild(parameter());

    // Check if the next token is a comma
    while (match(",")) {
        // Check if the next token is a data type
        if (current_token.value == "int" || current_token.value == "double" || current_token.value == "string" || current_token.value == "float") {
            // Add the parameter node as a child of the parameter list node
            param_list->addChild(parameter());
        } else {
            // Error handling
            error_message("data type after comma");
            break; // Exit the loop to avoid infinite loop on error
        }
    }

    // Return the parameter list node
    return param_list;
}

// implementation of parameter function
//parameter          -> data_type identifier
TreeNode* Parser::parameter() {
    // Create a new parameter node
    TreeNode* param = new TreeNode("Parameter");

    // Check if the current token is a data type
    if (current_token.value == "int" || current_token.value == "double" || current_token.value == "string" || current_token.value == "float") {
        // Add the data type node as a child of the parameter node
        param->addChild(data_type());

        // Check if the next token is an identifier
        if (current_token.type == TokenType::Identifier) {
            // Add the identifier node as a child of the parameter node
            param->addChild(identifier());
        } else {
            // Error handling
            error_message("identifier");
        }
    } else {
        // Error handling
        error_message("data type");
    }

    // Return the parameter node
    return param;
}

// implementation of compound_statement function
//compound_statement -> { statement_list }
TreeNode* Parser::compound_statement() {
    // Create a new compound statement node
    TreeNode* compound_stmt = new TreeNode("Compound Statement");

    // Check if the next token is an opening brace
    if (match("{")) {
        // Add the statement list node as a child of the compound statement node
        compound_stmt->addChild(statement_list());

        // Check if the next token is a closing brace
        if (!match("}")) {
            // Error handling
            error_message("'}'");
        }
    } else {
        // Error handling
        error_message("'{'");
    }

    // Return the compound statement node
    return compound_stmt;
}

// implementation of statement_list function
//statement_list     -> statement statement_list | statement
TreeNode* Parser::statement_list() {
    // Create a new statement list node
    TreeNode* stmt_list = new TreeNode("Statement List");

    // Add the first statement node as a child of the statement list node
    stmt_list->addChild(statement());

    // Continue adding statements as long as the current token indicates a statement
    while (current_token.value == "if" || current_token.value == "while" || current_token.value == "for" || current_token.value == "return" || current_token.value == "int" || current_token.value == "double" || current_token.value == "string" || current_token.value == "float") {
        // Add the next statement node as a child of the statement list node
        stmt_list->addChild(statement());
    }

    // Return the statement list node
    return stmt_list;
}

// implementation of statement function
//statement          -> expression_statement | if_statement | while_statement | for_statement | return_statement | declaration
TreeNode* Parser::statement() {
    // Create a new statement node
    TreeNode* stmt = new TreeNode("Statement");

    // Check if the current token is an expression
    if (current_token.type == TokenType::Identifier || current_token.type == TokenType::Number || current_token.type == TokenType::String || current_token.value == "(") {
        // Add the expression statement node as a child of the statement node
        stmt->addChild(expression_statement());
    } else if (current_token.value == "if") {
        // Add the if statement node as a child of the statement node
        stmt->addChild(if_statement());
    } else if (current_token.value == "while") {
        // Add the while statement node as a child of the statement node
        stmt->addChild(while_statement());
    } else if (current_token.value == "for") {
        // Add the for statement node as a child of the statement node
        stmt->addChild(for_statement());
    } else if (current_token.value == "return") {
        // Add the return statement node as a child of the statement node
        stmt->addChild(return_statement());
    } else if (current_token.value == "int" || current_token.value == "double" || current_token.value == "string" || current_token.value == "float") {
        // Add the declaration node as a child of the statement node
        stmt->addChild(declaration());
    } else {
        // Error handling
        error_message("expression, if, while, for, return, or declaration");
    }

    // Return the statement node
    return stmt;
}

// implementation of expression_statement function
//expression_statement -> expression ;
TreeNode* Parser::expression_statement() {
    // Create a new expression statement node
    TreeNode* expr_stmt = new TreeNode("Expression Statement");

    // Add the expression node as a child of the expression statement node
    expr_stmt->addChild(expression());

    // Check if the next token is a semicolon
    if (match(";")) {
        // Return the expression statement node
        return expr_stmt;
    } else {
        // Error handling
        error_message("';'");
        return expr_stmt;
    }
}

// implementation of if_statement function
//if_statement       -> if ( expression ) statement | if ( expression ) statement else statement
TreeNode* Parser::if_statement() {
    // Create a new if statement node
    TreeNode* if_stmt = new TreeNode("If Statement");

    // Check if the next token is an opening parenthesis
    if (match("(")) {
        // Add the expression node as a child of the if statement node
        if_stmt->addChild(expression());

        // Check if the next token is a closing parenthesis
        if (match(")")) {
            // Add the statement node as a child of the if statement node
            if_stmt->addChild(statement());

            // Check if the next token is an else
            if (match("else")) {
                // Add the statement node as a child of the if statement node
                if_stmt->addChild(statement());
            }
        } else {
            // Error handling
            error_message("')'");
        }
    } else {
        // Error handling
        error_message("'('");
    }

    // Return the if statement node
    return if_stmt;
}

// implementation of while_statement function
//while_statement   -> while ( expression ) statement
TreeNode* Parser::while_statement() {
    // Create a new while statement node
    TreeNode* while_stmt = new TreeNode("While Statement");

    // Check if the next token is an opening parenthesis
    if (match("(")) {
        // Add the expression node as a child of the while statement node
        while_stmt->addChild(expression());

        // Check if the next token is a closing parenthesis
        if (match(")")) {
            // Add the statement node as a child of the while statement node
            while_stmt->addChild(statement());
        } else {
            // Error handling
            error_message("')'");
        }
    } else {
        // Error handling
        error_message("'('");
    }

    // Return the while statement node
    return while_stmt;
}

// implementation of for_statement function
//for_statement     -> for ( expression_statement ; expression ; expression_statement ) statement
TreeNode* Parser::for_statement() {
    // Create a new for statement node
    TreeNode* for_stmt = new TreeNode("For Statement");

    // Check if the next token is an opening parenthesis
    if (match("(")) {
        // Add the initialization expression statement node as a child of the for statement node
        for_stmt->addChild(expression_statement());

        // Check if the next token is an expression (condition)
        for_stmt->addChild(expression());

        // Check if the next token is a semicolon
        if (match(";")) {
            // Add the increment expression statement node as a child of the for statement node
            for_stmt->addChild(expression_statement());

            // Check if the next token is a closing parenthesis
            if (match(")")) {
                // Add the statement node as a child of the for statement node
                for_stmt->addChild(statement());
            } else {
                // Error handling
                error_message("')'");
            }
        } else {
            // Error handling
            error_message("';'");
        }
    } else {
        // Error handling
        error_message("'('");
    }

    // Return the for statement node
    return for_stmt;
}

// implementation of return_statement function
//return_statement  -> return expression ;
TreeNode* Parser::return_statement() {
    // Create a new return statement node
    TreeNode* return_stmt = new TreeNode("Return Statement");

    // Check if the next token is the "return" keyword
    if (!match("return")) {
        // Error handling
        error_message("'return'");
        return return_stmt;
    }

    if (current_token.value == ";") {
        // Return the return statement node
        next_token();
        return return_stmt;
    }

    // Add the expression node as a child of the return statement node
    return_stmt->addChild(expression());

    // Check if the next token is a semicolon
    if (match(";")) {
        // Return the return statement node
        return return_stmt;
    } else {
        // Error handling
        error_message("';'");
        return return_stmt;
    }
}

// implementation of expression function
//expression         -> assignment_expression
TreeNode* Parser::expression() {
    // Create a new expression node
    TreeNode* expr = new TreeNode("Expression");

    // Add the assignment expression node as a child of the expression node
    expr->addChild(assignment_expression());

    // Return the expression node
    return expr;
}

// implementation of assignment_expression function
//assignment_expression -> logical_or_expression | logical_or_expression = assignment_expression
TreeNode* Parser::assignment_expression() {
    // Create a new assignment expression node
    TreeNode* assign_expr = new TreeNode("Assignment Expression");

    // Add the logical or expression node as a child of the assignment expression node
    assign_expr->addChild(logical_or_expression());

    // Check if the next token is an assignment operator
    if (match("=")) {
        // Add the assignment expression node as a child of the assignment expression node
        assign_expr->addChild(assignment_expression());
    }

    // Return the assignment expression node
    return assign_expr;
}

// implementation of logical_or_expression function
//logical_or_expression -> logical_and_expression | logical_or_expression || logical_and_expression
TreeNode* Parser::logical_or_expression() {
    // Create a new logical or expression node
    TreeNode* logical_or_expr = new TreeNode("Logical Or Expression");

    // Add the logical and expression node as a child of the logical or expression node
    logical_or_expr->addChild(logical_and_expression());

    // Check if the next token is a logical OR operator
    while (match("||")) {
        // Add the logical OR operator as a child of the logical or expression node
        TreeNode* logical_or_op = new TreeNode("Logical OR");
        logical_or_op->addChild(new TreeNode("||"));

        // Add the logical and expression node as a child of the logical or expression node
        logical_or_expr->addChild(logical_and_expression());
        logical_or_expr->addChild(logical_or_op);
    }

    // Return the logical or expression node
    return logical_or_expr;
}


// implementation of logical_and_expression function
//logical_and_expression -> equality_expression | logical_and_expression && equality_expression
TreeNode* Parser::logical_and_expression() {
    // Create a new logical and expression node
    TreeNode* logical_and_expr = new TreeNode("Logical And Expression");

    // Add the equality expression node as a child of the logical and expression node
    logical_and_expr->addChild(equality_expression());

    // Check if the next token is a logical AND operator
    if (match("&&")) {
        // Add the logical and expression node recursively
        logical_and_expr->addChild(logical_and_expression());
    }

    // Return the logical and expression node
    return logical_and_expr;
}

// implementation of equality_expression function
//equality_expression -> relational_expression | equality_expression == relational_expression | equality_expression != relational_expression
TreeNode* Parser::equality_expression() {
    // Create a new equality expression node
    TreeNode* equality_expr = new TreeNode("Equality Expression");

    // Add the relational expression node as a child of the equality expression node
    equality_expr->addChild(relational_expression());

    // Check if the next token is an equality operator
    while (current_token.value == "==" || current_token.value == "!=") {
        // Add the equality operator as a child of the equality expression node
        TreeNode* equality_op = new TreeNode("Equality Operator");
        equality_op->addChild(new TreeNode(current_token.value));
        next_token(); // Move to the next token after consuming the operator

        // Add the relational expression node as a child of the equality expression node
        equality_expr->addChild(equality_op);
        equality_expr->addChild(relational_expression());
    }

    // Return the equality expression node
    return equality_expr;
}

// implementation of relational_expression function
//relational_expression -> additive_expression | relational_expression < additive_expression | relational_expression > additive_expression | relational_expression <= additive_expression | relational_expression >= additive_expression
TreeNode* Parser::relational_expression() {
    // Create a new relational expression node
    TreeNode* relational_expr = new TreeNode("Relational Expression");

    // Add the additive expression node as a child of the relational expression node
    relational_expr->addChild(additive_expression());

    // Check if the next token is a relational operator
    while (current_token.value == "<" || current_token.value == ">" || current_token.value == "<=" || current_token.value == ">=") {
        // Add the relational operator as a child of the relational expression node
        TreeNode* relational_op = new TreeNode("Relational Operator");
        relational_op->addChild(new TreeNode(current_token.value));
        next_token(); // Move to the next token after consuming the operator

        // Add the additive expression node as a child of the relational expression node
        relational_expr->addChild(relational_op);
        relational_expr->addChild(additive_expression());
    }

    // Return the relational expression node
    return relational_expr;
}

// implementation of additive_expression function
//additive_expression -> multiplicative_expression | additive_expression + multiplicative_expression | additive_expression - multiplicative_expression
TreeNode* Parser::additive_expression() {
    // Create a new additive expression node
    TreeNode* additive_expr = new TreeNode("Additive Expression");

    // Add the multiplicative expression node as a child of the additive expression node
    additive_expr->addChild(multiplicative_expression());

    // Check if the next token is an additive operator
    while (current_token.value == "+" || current_token.value == "-") {
        // Add the additive operator as a child of the additive expression node
        TreeNode* additive_op = new TreeNode("Additive Operator");
        additive_op->addChild(new TreeNode(current_token.value));
        next_token(); // Move to the next token after consuming the operator

        // Add the multiplicative expression node as a child of the additive expression node
        additive_expr->addChild(additive_op);
        additive_expr->addChild(multiplicative_expression());
    }

    // Return the additive expression node
    return additive_expr;
}


// implementation of multiplicative_expression function
//multiplicative_expression -> primary_expression | multiplicative_expression * primary_expression | multiplicative_expression / primary_expression | multiplicative_expression % primary_expression
TreeNode* Parser::multiplicative_expression() {
    // Create a new multiplicative expression node
    TreeNode* multiplicative_expr = new TreeNode("Multiplicative Expression");

    // Add the primary expression node as a child of the multiplicative expression node
    multiplicative_expr->addChild(primary_expression());

    // Check if the next token is a multiplicative operator
    while (current_token.value == "*" || current_token.value == "/" || current_token.value == "%") {
        // Add the multiplicative operator as a child of the multiplicative expression node
        TreeNode* multiplicative_op = new TreeNode("Multiplicative Operator");
        multiplicative_op->addChild(new TreeNode(current_token.value));
        next_token(); // Move to the next token after consuming the operator

        // Add the primary expression node as a child of the multiplicative expression node
        multiplicative_expr->addChild(multiplicative_op);
        multiplicative_expr->addChild(primary_expression());
    }

    // Return the multiplicative expression node
    return multiplicative_expr;
}

// implementation of primary_expression function
//primary_expression -> identifier | literal | ( expression ) | function_call
TreeNode* Parser::primary_expression() {
    // Create a new primary expression node
    TreeNode* primary_expr = new TreeNode("Primary Expression");

    // Check if the current token is literal, or opening parenthesis
    if (current_token.type == TokenType::Identifier) {
        next_token();
        if(match("(")) {
            // Add the function call node as a child of the primary expression node
            primary_expr->addChild(function_call());
        } else {
            // Add the identifier node as a child of the primary expression node
            primary_expr->addChild(identifier());
        }
    } else if (current_token.type == TokenType::Number || current_token.type == TokenType::String) {
        // Add the literal node as a child of the primary expression node
        primary_expr->addChild(literal());
    } else if (match("(")) {
        // Add the expression node as a child of the primary expression node
        primary_expr->addChild(expression());

        // Check if the next token is a closing parenthesis
        if (!match(")")) {
            // Error handling
            error_message("')'");
            // Add an empty node to indicate an error
            primary_expr->addChild(new TreeNode("Error"));
        }
    } else {
        // Error handling
        error_message("identifier, literal, or '('");
        // Add an empty node to indicate an error
        primary_expr->addChild(new TreeNode("Error"));
    }

    // Return the primary expression node
    return primary_expr;
}

// implementation of function_call function
//function_call      -> identifier ( argument_list )
TreeNode* Parser::function_call() {
    // Create a new function call node
    TreeNode* func_call = new TreeNode("Function Call");
    func_call->addChild(argument_list());


    // Return the function call node
    return func_call;
}

// implementation of argument_list function
//argument_list      -> expression , argument_list | expression
TreeNode* Parser::argument_list() {
    // Create a new argument list node
    TreeNode* arg_list = new TreeNode("Argument List");

    // Add the first expression node as a child of the argument list node
    arg_list->addChild(expression());

    // Check if the next token is a comma
    while (match(",")) {
        // Add the expression node as a child of the argument list node
        arg_list->addChild(expression());
    }

    // Check if the next token is not a closing parenthesis
    if (!match(")")) {
        // Error handling
        error_message("')' or expression");
        // Add an empty node to indicate an error
        arg_list->addChild(new TreeNode("Error"));
    }

    // Return the argument list node
    return arg_list;
}


// implementation of identifier function
//identifier         -> [a-zA-Z_][a-zA-Z0-9]*
TreeNode* Parser::identifier() {
    // Create a new identifier node
    TreeNode* id = new TreeNode("Identifier");

    // Check if the current token is an identifier
    if (current_token.type == TokenType::Identifier) {
        // Add the identifier token as a child of the identifier node
        id->addChild(new TreeNode(current_token.value));
        // Move to the next token
        next_token();
    } else {
        // Error handling
        error_message("identifier");
        // Add an empty node to indicate an error
        id->addChild(new TreeNode("Error"));
        // Move to the next token to recover from the error
        next_token();
    }

    // Return the identifier node
    return id;
}


// implementation of literal function
//literal            -> number | string
TreeNode* Parser::literal() {
    // Create a new literal node
    TreeNode* lit = new TreeNode("Literal");

    // Check if the current token is a number or string
    if (current_token.type == TokenType::Number || current_token.type == TokenType::String) {
        // Add the number or string token as a child of the literal node
        lit->addChild(new TreeNode(current_token.value));
        // Move to the next token
        next_token();
    } else {
        // Error handling
        error_message("number or string");
        // Add an empty node to indicate an error
        lit->addChild(new TreeNode("Error"));
        // Move to the next token to recover from the error
        next_token();
    }

    // Return the literal node
    return lit;
}


// implementation of number function
// number             -> [0-9]+(\.[0-9]+)?([eE][-+]?[0-9]+)?
TreeNode* Parser::number() {
    // Create a new number node
    TreeNode* num = new TreeNode("Number");

    // Check if the current token is a number
    if (current_token.type == TokenType::Number) {
        // Add the number token as a child of the number node
        num->addChild(new TreeNode(current_token.value));
        // Move to the next token
        next_token();
    } else {
        // Error handling
        error_message("number");
        // Add an empty node to indicate an error
        num->addChild(new TreeNode("Error"));
        // Move to the next token to recover from the error
        next_token();
    }

    // Return the number node
    return num;
}

// implementation of string function
// string             -> "([^"\\]*(\\.)*[^"\\]*)*
TreeNode* Parser::_string() {
    // Create a new string node
    TreeNode* str = new TreeNode("String");

    // Check if the current token is a string
    if (current_token.type == TokenType::String) {
        // Add the string token as a child of the string node
        str->addChild(new TreeNode(current_token.value));
        // Move to the next token
        next_token();
    } else {
        // Error handling
        error_message("string");
        // Add an empty node to indicate an error
        str->addChild(new TreeNode("Error"));
        // Move to the next token to recover from the error
        next_token();
    }

    // Return the string node
    return str;
}


// implementation of parse function
TreeNode* Parser::parse() {
    // Parse the program
    TreeNode* root = program();

    // Check if there are any tokens left
    if (token_index < tokens.size()) {
        // Print an error message
        std::cout << "Error: Unexpected token " << current_token.value << std::endl;
        // Set the error flag to true
        error = true;
    }

    // Return the root node of the parse tree
    return root;
}

// implementation of recursiveDescentParser function
void Parser::recursiveDescentParser() {
    // Parse the program
    TreeNode* root = parse();

    // Check if there was an error during parsing
    if (error) {
        // Print an error message
        std::cout << "Error: Parsing failed" << std::endl;
    }
}





//using grphviz to visualize the parse tree
void Parser::visualizeParseTree(TreeNode* root) {

    std::ofstream dot_file_out("parse_tree.dot", std::ios::trunc);

    // TODO: Would be nice if you decorated your parse tree, lookup how to do so in graphviz dot notation
    dot_file_out << "digraph G {\n";
    visualize_parse_tree_aux(root, dot_file_out);
    dot_file_out << "}";
    dot_file_out.close();

    // Convert the dot file into a SVG vector image
    system("dot -Tsvg ./parse_tree.dot -oparse_tree.svg");

    // Open the vector file
    // TODO: Integrate this with your GUI...
    // You'll need to lookup how to open an .svg file inside Qt or whatever GUI framework you are using
    system("parse_tree.svg");

}

void Parser::visualize_parse_tree_aux(TreeNode* node, std::ostream& out) {

    // Need to uniquely identify each node in graphviz
    out << "  " << node << (node->hasChildren() ? ";\n": "[fontcolor=\"red\"][color=\"red\"];\n");

    for (auto child : node->getChildren()) {

        bool child_has_children = child->hasChildren();
        if (!child->hasChildren() || (child_has_children)) {

            // Need to uniquely identify each child node too
            out << "  " << child << ";\n";

            // Create edge between node and child unique addresses
            out << "  \"" << (void*) node << "\" -> \"" << (void*) child << "\";\n";

            // Recursively build rest of tree
            visualize_parse_tree_aux(child, out);

        }

    }

}
