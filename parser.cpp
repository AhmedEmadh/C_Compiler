//implementatoin of the parser class
#include <iostream>
#include "parser.h"
#include "Token.h"

// implementation of the parser constructor
Parser::Parser(std::string inputFilePath) {
    // Create an instance of Lexer with the input code
    lexer.setFilePath(inputFilePath);
    // Tokenize the input code
    lexer.scan();
    // Get the tokens from the lexer
    Parser::tokens = lexer.getObjectTokens();

}
//implementation of the parser empty constructor

Parser::Parser() {
    lexer = Lexer();
}

//implementation of the parser setInput function
void Parser::setInput(std::string input) {
    lexer.setInput(input);
    lexer.scan();
    Parser::tokens = lexer.getObjectTokens();
}

//implementation of the parser setFilePath function
void Parser::setFilePath(std::string inputFilePath) {
    lexer.setFilePath(inputFilePath);
    lexer.scan();
    Parser::tokens = lexer.getObjectTokens();
}


