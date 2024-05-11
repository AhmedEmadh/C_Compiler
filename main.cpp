#include <iostream>
#include "Lexer.h"
#include "Parser.h"

int main() {

    // Create an instance of Lexer with the input code
    Lexer lexer("tests/test1.cpp");

    // Tokenize the input code
    lexer.scan();
    
    // // Print the tokens
    // lexer.printSymbolTable();
    //print the tokens
    // lexer.printObjectTokens();
    
    //create an instance of Parser
    Parser parser(lexer.getObjectTokens());
    parser.visualizeParseTree(parser.parse());
    return 0;
}
