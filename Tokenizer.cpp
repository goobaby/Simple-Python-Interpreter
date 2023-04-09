//
// Created by Ali Kooshesh on 4/4/23.
//

#include <iostream>
#include <string>
#include "Tokenizer.hpp"

std::string Tokenizer::readName() {
    // This function is called when it is known that
    // the first character in input is an alphabetic character.
    // The function reads and returns all characters of the name.

    std::string name;
    char c;

    // isalnum checks if 'c' is a decimal dig, upper/lower case letter
    // instream.get(c) takes one character and will either return true/false in
    // the case of our while loop

    while( inStream.get(c) && isalnum(c) ) {
        name += c;
    }

    // WHITESPACE CHECKER
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    
    return name;
}

int Tokenizer::readInteger() {
    // This function is called when it is known that
    // the first character in input is a digit.
    // The function reads and returns all remaining digits.

    int intValue = 0;
    char c;
    while( inStream.get(c) && isdigit(c) ) {
        //This seems to be a conversion from character to int 
        intValue = intValue * 10 + c - '0';
    }

    // WHITESPACE CHECKER
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return intValue;
}

std::string Tokenizer::readRelationalOperator() {
    // This function is called when it is known that
    // the first character is a relational operator that is not ==
    // The function reads and returns >, >=, <, or <=

    std::string relationOp = "";
    char c;
   
    inStream.get(c);
    relationOp += c;
    inStream.get(c);
    if(c == '=' && relationOp.size() == 1)
         relationOp += c;
       
    inStream.get(c);
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return relationOp;
}

//CONSTRUCTOR 
// - sets a reference to the passed inputstream reference to inStream in private section of class
// - defaults ungottenToken to false
// - lastToken is set to null
Tokenizer::Tokenizer(std::ifstream &stream): ungottenToken{false}, inStream{stream}, lastToken{} {}

Token Tokenizer::getToken() {

    if(ungottenToken) {
        ungottenToken = false;
        return lastToken;
    }

    char c;
    
    //while( inStream.get(c) && isspace(c) && c != '\n' )  // Skip spaces but not new-line chars.
        //;
    
    while( inStream.get(c) && isspace(c) )  // Skip spaces including the new-line chars.
        ;
    

    if(inStream.bad()) {
        std::cout << "Error while reading the input stream in Tokenizer.\n";
        exit(1);
    }

    //std::cout << "c = " << c << std::endl;

    Token token;
    if( inStream.eof()) {
        token.eof() = true;
    } else if( c == '\n' ) {  // will not ever be the case unless new-line characters are not supressed.
        token.eol() = true;
    } else if( isdigit(c) ) { // a integer?
        // put the digit back into the input stream so
        // we read the entire number in a function
        inStream.putback(c);
        token.setWholeNumber( readInteger() );

    } else if( reachedRelationalOperator(c)) {
        // put the relation operator back into the input stream so
        // we read the entire operator in a function
        // does NOT include ==
        inStream.putback(c);
        token.setRelationalOperator( readRelationalOperator() );
    }
    
    // All else ifs from here are assigning symbol inside of token, implying
    // that there will be a whitespace behind the symbol 
    else if( c == '=' ){
        // Step 2
        // Here we check if it is == or just =
        // NOTE: will NOT check for ===
        if (inStream.peek() == '=')
        {
            std::string relOp;
            relOp += c;
            inStream.get(c);
            relOp += c;
            token.setRelationalOperator( relOp );
        }
        else 
        {
            token.symbol(c);
        }
    }
    else if( c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
        token.symbol(c);
    else if( c == ';' )
        token.symbol(c);
    else if( c == '(' || c == ')')
        token.symbol(c);
    else if(isalpha(c)) {  // an identifier?
        // put c back into the stream so we can read the entire name in a function.
        inStream.putback(c);
        token.setName( readName() );
        if(token.isKeyword())
            token.setIsKeyword();
    } else {
        std::cout << "Unknown character in input. ->" << c << "<-" << std::endl;
        exit(1);
    }
    // Vector of all tokens from the file we are reading
    _tokens.push_back(token);
    return lastToken = token;
}

void Tokenizer::ungetToken() {
    ungottenToken = true;
}

// iter is a reference to the base address of the first element of _tokens(vector of tokens)
// Calls print() in token.cpp
void Tokenizer::printProcessedTokens() {
    for(auto iter = _tokens.begin(); iter != _tokens.end(); ++iter) {
        iter->print();
        std::cout << std::endl;
    }
}
