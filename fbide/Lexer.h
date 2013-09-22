//
//  Lexer.h
//  fbide
//
//  Created by Albert on 16/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//
#pragma once
#include "Token.h"


/**
 * lex input into sequence of tokens
 */
class Lexer
{
public:
    
    /**
     * create new instance of the lexer
     */
    Lexer(const char * input);
    
    /**
     * get next token from the stream
     */
    std::shared_ptr<Token> next();
    
private:
    
    /**
     * read number
     */
    std::shared_ptr<Token> number();
    
    /**
     * read string
     */
    std::shared_ptr<Token> string();
    
    /**
     * Read identifier. Might be a keyword
     */
    std::shared_ptr<Token> identifier();
    
    /**
     * create new token
     */
    std::shared_ptr<Token> token(TokenKind kind, std::string lexeme = "");
    
    /**
     * Read in current character from the input.
     */
    inline bool read();
    
    /**
     * Advance input by 1 and read in the character
     */
    inline bool readNext();
    
    /**
     * needed for consistent read / advance stateus
     */
    bool m_readOk;
    
    /**
     * current char
     */
    char m_ch;
    
    /**
     * next character
     */
    char m_next;
    
    /**
     * read position
     */
    TokenLoc m_loc;
    
    /**
     * token location
     */
    TokenLoc m_tokenLoc;
    
    
    /**
     * line has a possible statement
     */
    bool m_hasStmt;
    
    
    /**
     * the input text to tokenize
     */
    const char * m_input;
};
