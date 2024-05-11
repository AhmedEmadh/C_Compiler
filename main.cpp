#include <iostream>
#include "Lexer.h"

int main() {

    // Create an instance of Lexer with the input code
    Lexer lexer("tests/test1.cpp");

    // Tokenize the input code
    lexer.scan();

    // Get the tokens from the lexer
    std::vector<string> tokens = lexer.getTokens();
    lexer.PrintOutput();
    cout << "------------------------object tokens---------------------------------" << endl;
    
    // Print the tokens
    lexer.printObjectTokens();
    lexer.printSymbolTable();
    return 0;
}
