//implementatoin of the parser class

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

// implementation of match function
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
    std::cout << "Error: Expected " << expected << " at token " << current_token.value << std::endl;
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
TreeNode* Parser::declaration_list() {
    // Create a new declaration list node
    TreeNode* decl_list = new TreeNode("Declaration List");
    // Add the first declaration node as a child of the declaration list node
    decl_list->addChild(declaration());
    // Loop to add more declaration nodes
    while (match(";")) {
        decl_list->addChild(declaration());
    }
    // Return the declaration list node
    return decl_list;
}

TreeNode* Parser::declaration() {
    // Create a new declaration node
    TreeNode* decl = new TreeNode("Declaration");
    // Check if the token is a data type
    if (match("int") || match("float") || match("char") || match("double")) {
        // Add the data type node as a child of the declaration node
        decl->addChild(data_type());
        // Check if the next token is an identifier (for variable declaration) or '(' (for function declaration)
        if (current_token.type == TokenType::Identifier) {
            
            // Add the variable declaration node as a child of the declaration node
            decl->addChild(variable_declaration());
        } else if (match("(")) {
            
            // Add the function declaration node as a child of the declaration node
            decl->addChild(function_declaration());
        } else {
            // Error handling
            error_message("identifier or '('");
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
TreeNode* Parser::variable_declaration() {
    // Create a new variable declaration node
    TreeNode* var_decl = new TreeNode("Variable Declaration");
    // Add the identifier list node as a child of the variable declaration node
    var_decl->addChild(identifier_list());
    // Return the variable declaration node
    return var_decl;
}

// implementation of identifier_list function
TreeNode* Parser::identifier_list() {
    // Create a new identifier list node
    TreeNode* id_list = new TreeNode("Identifier List");
    // Add the first identifier node as a child of the identifier list node
    id_list->addChild(identifier());
    // Loop to add more identifier nodes
    while (match(",")) {
        id_list->addChild(identifier());
    }
    // Return the identifier list node
    return id_list;
}

// implementation of data_type function
TreeNode* Parser::data_type() {
    // Create a new data type node
    TreeNode* type = new TreeNode("Data Type");
    // Check if the token is a data type
    if (current_token.value == "int" || current_token.value == "float" || current_token.value == "char" || current_token.value == "void") {
        // Add the data type token as a child of the data type node
        type->addChild(new TreeNode(current_token.value));
        // Move to the next token
        next_token();
    }
    else {
        // Error handling
        error_message("data type");
    }
    // Return the data type node
    return type;
}

// implementation of function_declaration function
TreeNode* Parser::function_declaration() {
    // Create a new function declaration node
    TreeNode* func_decl = new TreeNode("Function Declaration");
    // Add the identifier node as a child of the function declaration node
    func_decl->addChild(identifier());
    // Add the parameter list node as a child of the function declaration node
    func_decl->addChild(parameter_list());
    // Add the compound statement node as a child of the function declaration node
    func_decl->addChild(compound_statement());
    // Return the function declaration node
    return func_decl;
}

// implementation of parameter_list function
TreeNode* Parser::parameter_list() {
    // Create a new parameter list node
    TreeNode* param_list = new TreeNode("Parameter List");
    // Check if the token is an opening parenthesis
    if (match("(")) {
        // Add the first parameter node as a child of the parameter list node
        param_list->addChild(parameter());
        // Loop to add more parameter nodes
        while (match(",")) {
            param_list->addChild(parameter());
        }
        // Check if the token is a closing parenthesis
        if (!match(")")) {
            // Error handling
            error_message(")");
        }
    }
    else {
        // Error handling
        error_message("(");
    }
    // Return the parameter list node
    return param_list;
}

// implementation of parameter function
TreeNode* Parser::parameter() {
    // Create a new parameter node
    TreeNode* param = new TreeNode("Parameter");
    // Add the data type node as a child of the parameter node
    param->addChild(data_type());
    // Add the identifier node as a child of the parameter node
    param->addChild(identifier());
    // Return the parameter node
    return param;
}

// implementation of compound_statement function
TreeNode* Parser::compound_statement() {
    // Create a new compound statement node
    TreeNode* comp_stmt = new TreeNode("Compound Statement");
    // Check if the token is an opening curly brace
    if (match("{")) {
        // Add the statement list node as a child of the compound statement node
        comp_stmt->addChild(statement_list());
        // Check if the token is a closing curly brace
        if (!match("}")) {
            // Error handling
            error_message("}");
        }
    }
    else {
        // Error handling
        error_message("{");
    }
    // Return the compound statement node
    return comp_stmt;
}

// implementation of statement_list function
TreeNode* Parser::statement_list() {
    // Create a new statement list node
    TreeNode* stmt_list = new TreeNode("Statement List");
    // Add the first statement node as a child of the statement list node
    stmt_list->addChild(statement());

    
    // Loop to add more statement nodes
    while (current_token.value != "}") {
        stmt_list->addChild(statement());
    }
    // Return the statement list node
    return stmt_list;
}

// implementation of statement function
TreeNode* Parser::statement() {
    // Create a new statement node
    TreeNode* stmt = new TreeNode("Statement");
    // Check the type of statement
    if (match("if")) {
        // Add the if statement node as a child of the statement node
        stmt->addChild(if_statement());
    }
    else if (match("while")) {
        // Add the while statement node as a child of the statement node
        stmt->addChild(while_statement());
    }
    else if (match("for")) {
        // Add the for statement node as a child of the statement node
        stmt->addChild(for_statement());
    }
    else if (match("return")) {
        // Add the return statement node as a child of the statement node
        stmt->addChild(return_statement());
    }
    else if (match("{")) {
        // Add the compound statement node as a child of the statement node
        stmt->addChild(compound_statement());
    }
    else {
        // Add the expression statement node as a child of the statement node
        stmt->addChild(expression_statement());
    }
    // Return the statement node
    return stmt;
}

// implementation of expression_statement function
TreeNode* Parser::expression_statement() {
    // Create a new expression statement node
    TreeNode* expr_stmt = new TreeNode("Expression Statement");
    // Check if the token is a semicolon
    if (match(";")) {
        // Return the expression statement node
        return expr_stmt;
    }
    else {
        // Add the expression node as a child of the expression statement node
        expr_stmt->addChild(expression());
        // Check if the token is a semicolon
        if (!match(";")) {
            // Error handling
            error_message(";");
        }
        // Return the expression statement node
        return expr_stmt;
    }
}

// implementation of if_statement function
TreeNode* Parser::if_statement() {
    // Create a new if statement node
    TreeNode* if_stmt = new TreeNode("If Statement");
    // Check if the token is an opening parenthesis
    if (match("(")) {
        // Add the expression node as a child of the if statement node
        if_stmt->addChild(expression());
        // Check if the token is a closing parenthesis
        if (!match(")")) {
            // Error handling
            error_message(")");
        }
    }
    else {
        // Error handling
        error_message("(");
    }
    // Add the statement node as a child of the if statement node
    if_stmt->addChild(statement());
    // Check if the token is "else"
    if (match("else")) {
        // Add the statement node as a child of the if statement node
        if_stmt->addChild(statement());
    }
    // Return the if statement node
    return if_stmt;
}

// implementation of while_statement function
TreeNode* Parser::while_statement() {
    // Create a new while statement node
    TreeNode* while_stmt = new TreeNode("While Statement");
    // Check if the token is an opening parenthesis
    if (match("(")) {
        // Add the expression node as a child of the while statement node
        while_stmt->addChild(expression());
        // Check if the token is a closing parenthesis
        if (!match(")")) {
            // Error handling
            error_message(")");
        }
    }
    else {
        // Error handling
        error_message("(");
    }
    // Add the statement node as a child of the while statement node
    while_stmt->addChild(statement());
    // Return the while statement node
    return while_stmt;
}

// implementation of for_statement function
TreeNode* Parser::for_statement() {
    // Create a new for statement node
    TreeNode* for_stmt = new TreeNode("For Statement");
    // Check if the token is an opening parenthesis
    if (match("(")) {
        // Add the expression statement node as a child of the for statement node
        for_stmt->addChild(expression_statement());
        // Add the expression statement node as a child of the for statement node
        for_stmt->addChild(expression_statement());
        // Add the expression node as a child of the for statement node
        for_stmt->addChild(expression());
        // Check if the token is a closing parenthesis
        if (!match(")")) {
            // Error handling
            error_message(")");
        }
    }
    else {
        // Error handling
        error_message("(");
    }
    // Add the statement node as a child of the for statement node
    for_stmt->addChild(statement());
    // Return the for statement node
    return for_stmt;
}

// implementation of return_statement function
TreeNode* Parser::return_statement() {
    // Create a new return statement node
    TreeNode* return_stmt = new TreeNode("Return Statement");
    // Check if the token is a semicolon
    if (match(";")) {
        // Return the return statement node
        return return_stmt;
    }
    else {
        // Add the expression node as a child of the return statement node
        return_stmt->addChild(expression());
        // Check if the token is a semicolon
        if (!match(";")) {
            // Error handling
            error_message(";");
        }
        // Return the return statement node
        return return_stmt;
    }
}

// implementation of expression function
TreeNode* Parser::expression() {
    // Create a new expression node
    TreeNode* expr = new TreeNode("Expression");
    // Add the assignment expression node as a child of the expression node
    expr->addChild(assignment_expression());
    // Return the expression node
    return expr;
}

// implementation of assignment_expression function
TreeNode* Parser::assignment_expression() {
    // Create a new assignment expression node
    TreeNode* assign_expr = new TreeNode("Assignment Expression");
    // Add the logical or expression node as a child of the assignment expression node
    assign_expr->addChild(logical_or_expression());
    // Check if the token is an assignment operator
    if (match("=")) {
        // Add the assignment operator token as a child of the assignment expression node
        assign_expr->addChild(new TreeNode("="));
        // Add the assignment expression node as a child of the assignment expression node
        assign_expr->addChild(assignment_expression());
    }
    // Return the assignment expression node
    return assign_expr;
}

// implementation of logical_or_expression function
TreeNode* Parser::logical_or_expression() {
    // Create a new logical or expression node
    TreeNode* or_expr = new TreeNode("Logical OR Expression");
    // Add the logical and expression node as a child of the logical or expression node
    or_expr->addChild(logical_and_expression());
    // Loop to check for logical OR operators
    while (match("||")) {
        // Add the logical OR operator token as a child of the logical or expression node
        or_expr->addChild(new TreeNode("||"));
        // Add the logical and expression node as a child of the logical or expression node
        or_expr->addChild(logical_and_expression());
    }
    // Return the logical or expression node
    return or_expr;
}

// implementation of logical_and_expression function
TreeNode* Parser::logical_and_expression() {
    // Create a new logical and expression node
    TreeNode* and_expr = new TreeNode("Logical AND Expression");
    // Add the equality expression node as a child of the logical and expression node
    and_expr->addChild(equality_expression());
    // Loop to check for logical AND operators
    while (match("&&")) {
        // Add the logical AND operator token as a child of the logical and expression node
        and_expr->addChild(new TreeNode("&&"));
        // Add the equality expression node as a child of the logical and expression node
        and_expr->addChild(equality_expression());
    }
    // Return the logical and expression node
    return and_expr;
}

// implementation of equality_expression function
TreeNode* Parser::equality_expression() {
    // Create a new equality expression node
    TreeNode* eq_expr = new TreeNode("Equality Expression");
    // Add the relational expression node as a child of the equality expression node
    eq_expr->addChild(relational_expression());
    // Loop to check for equality operators
    while (match("==") || match("!=")) {
        // Add the equality operator token as a child of the equality expression node
        eq_expr->addChild(new TreeNode(current_token.value));
        // Add the relational expression node as a child of the equality expression node
        eq_expr->addChild(relational_expression());
    }
    // Return the equality expression node
    return eq_expr;
}

// implementation of relational_expression function
TreeNode* Parser::relational_expression() {
    // Create a new relational expression node
    TreeNode* rel_expr = new TreeNode("Relational Expression");
    // Add the additive expression node as a child of the relational expression node
    rel_expr->addChild(additive_expression());
    // Loop to check for relational operators
    while (match("<") || match(">") || match("<=") || match(">=")) {
        // Add the relational operator token as a child of the relational expression node
        rel_expr->addChild(new TreeNode(current_token.value));
        // Add the additive expression node as a child of the relational expression node
        rel_expr->addChild(additive_expression());
    }
    // Return the relational expression node
    return rel_expr;
}

// implementation of additive_expression function
TreeNode* Parser::additive_expression() {
    // Create a new additive expression node
    TreeNode* add_expr = new TreeNode("Additive Expression");
    // Add the multiplicative expression node as a child of the additive expression node
    add_expr->addChild(multiplicative_expression());
    // Loop to check for additive operators
    while (match("+") || match("-")) {
        // Add the additive operator token as a child of the additive expression node
        add_expr->addChild(new TreeNode(current_token.value));
        // Add the multiplicative expression node as a child of the additive expression node
        add_expr->addChild(multiplicative_expression());
    }
    // Return the additive expression node
    return add_expr;
}

// implementation of multiplicative_expression function
TreeNode* Parser::multiplicative_expression() {
    // Create a new multiplicative expression node
    TreeNode* mult_expr = new TreeNode("Multiplicative Expression");
    // Add the primary expression node as a child of the multiplicative expression node
    mult_expr->addChild(primary_expression());
    // Loop to check for multiplicative operators
    while (match("*") || match("/") || match("%")) {
        // Add the multiplicative operator token as a child of the multiplicative expression node
        mult_expr->addChild(new TreeNode(current_token.value));
        // Add the primary expression node as a child of the multiplicative expression node
        mult_expr->addChild(primary_expression());
    }
    // Return the multiplicative expression node
    return mult_expr;
}

// implementation of primary_expression function
TreeNode* Parser::primary_expression() {
    // Create a new primary expression node
    TreeNode* prim_expr = new TreeNode("Primary Expression");
    // Check the type of primary expression
    if (match("identifier")) {
        // Add the identifier node as a child of the primary expression node
        prim_expr->addChild(identifier());
    }
    else if (match("literal")) {
        // Add the literal node as a child of the primary expression node
        prim_expr->addChild(literal());
    }
    else if (match("(")) {
        // Add the expression node as a child of the primary expression node
        prim_expr->addChild(expression());
        // Check if the token is a closing parenthesis
        if (!match(")")) {
            // Error handling
            error_message(")");
        }
    }
    else {
        // Error handling
        error_message("identifier, literal, or (");
    }
    // Return the primary expression node
    return prim_expr;
}

// implementation of function_call function
TreeNode* Parser::function_call() {
    // Create a new function call node
    TreeNode* func_call = new TreeNode("Function Call");
    // Add the identifier node as a child of the function call node
    func_call->addChild(identifier());
    // Check if the token is an opening parenthesis
    if (match("(")) {
        // Add the argument list node as a child of the function call node
        func_call->addChild(argument_list());
        // Check if the token is a closing parenthesis
        if (!match(")")) {
            // Error handling
            error_message(")");
        }
    }
    else {
        // Error handling
        error_message("(");
    }
    // Return the function call node
    return func_call;
}

// implementation of argument_list function
TreeNode* Parser::argument_list() {
    // Create a new argument list node
    TreeNode* arg_list = new TreeNode("Argument List");
    // Check if the token is not a closing parenthesis
    if (current_token.value != ")") {
        // Add the expression node as a child of the argument list node
        arg_list->addChild(expression());
        // Loop to add more expression nodes
        while (match(",")) {
            arg_list->addChild(expression());
        }
    }
    // Return the argument list node
    return arg_list;
}

// implementation of identifier function
TreeNode* Parser::identifier() {
    // Create a new identifier node
    TreeNode* id = new TreeNode("Identifier");
    // Check if the token is an identifier
    if (current_token.type == TokenType::Identifier) {
        // Add the identifier token as a child of the identifier node
        id->addChild(new TreeNode(current_token.value));
        // Move to the next token
        next_token();
    }
    else {
        // Error handling
        error_message("identifier");
    }
    // Return the identifier node
    return id;
}

// implementation of literal function
TreeNode* Parser::literal() {
    // Create a new literal node
    TreeNode* lit = new TreeNode("Literal");
    // Check if the token is a literal
    if (current_token.type == TokenType::Number || current_token.type == TokenType::String) {
        // Add the literal token as a child of the literal node
        lit->addChild(new TreeNode(current_token.value));
        // Move to the next token
        next_token();
    }
    else {
        // Error handling
        error_message("literal");
    }
    // Return the literal node
    return lit;
}

// implementation of number function
TreeNode* Parser::number() {
    // Create a new number node
    TreeNode* num = new TreeNode("Number");
    // Check if the token is a number
    if (current_token.type == TokenType::Number) {
        // Add the number token as a child of the number node
        num->addChild(new TreeNode(current_token.value));
        // Move to the next token
        next_token();
    }
    else {
        // Error handling
        error_message("number");
    }
    // Return the number node
    return num;
}

// implementation of string function
TreeNode* Parser::string() {
    // Create a new string node
    TreeNode* str = new TreeNode("String");
    // Check if the token is a string
    if (current_token.type == TokenType::String) {
        // Add the string token as a child of the string node
        str->addChild(new TreeNode(current_token.value));
        // Move to the next token
        next_token();
    }
    else {
        // Error handling
        error_message("string");
    }
    // Return the string node
    return str;
}


TreeNode* Parser::recursively_parse() {
    // next_token();
    return program();
}

TreeNode* Parser::parse() {
    token_index = 0;
    error = false;
    return recursively_parse();
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