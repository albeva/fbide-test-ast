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
    TokenPtr next();
    
private:
    
    /**
     * read number
     */
    TokenPtr number();
    
    /**
     * read string
     */
    TokenPtr string();
    
    /**
     * Read identifier. Might be a keyword
     */
    TokenPtr identifier();
    
    /**
     * create new token
     */
    TokenPtr token(TokenKind kind, std::string lexeme = "", std::string original = "");
    
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
     * Token start character position
     */
    const char * m_tokenStart;
    
    
    /**
     * line has a possible statement
     */
    bool m_hasStmt;
    
    
    /**
     * the input text to tokenize
     */
    const char * m_input;
};
