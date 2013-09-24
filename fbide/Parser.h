//
//  Parser.h
//  fbide
//
//  Created by Albert on 16/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//
#pragma once
#include "Token.h"


/**
 * process the tokens and analyze for syntax errors
 */
class Parser
{
    TokenPtr m_token;
    TokenPtr m_next;
    
public:
    
    /**
     * default constructor
     */
    Parser();
    
    /**
     * set root token
     */
    void setRoot(TokenPtr token);
    
    /**
     * return true if next token matches the type
     */
    bool match(TokenKind kind);
    
    /**
     * match the token and advance. Return result
     */
    bool accept(TokenKind kind);
    
    /**
     * expect next token to be of type. Mark as invalid if doesn't match
     */
    bool expect(TokenKind kind);
    
    /**
     * move to the next token
     */
    void move();
    
    /**
     * Parse
     */
    void parse();
    
    /**
     * parse DECLARE statement
     */
    void parseDeclare();
    
    /**
     * Parse DIM statement
     */
    void parseDim();

};
