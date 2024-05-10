#include <iostream>
#include "Lexer.h"

int main() {

    // Create an instance of Lexer with the input code
    Lexer lexer("test.cpp");

    // Tokenize the input code
    lexer.scan();

    // Get the tokens from the lexer
    std::vector<Token> tokens = lexer.getObjectTokens();

    // Print the tokens
    //lexer.printObjectTokens();
    lexer.printSymbolTable();
    return 0;
}
