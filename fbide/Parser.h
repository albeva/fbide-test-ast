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
    
    std::vector<std::string> identifiers;
    
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
     * move to the next token
     */
    void move();
    
    /**
     * Parse
     */
    bool parse();
    
    /**
     * parse the expression
     */
    bool parseExpression();
    
    /**
     * parse function parameter list
     */
    bool parseArguList();
    
    /**
     * parse type expression
     */
    bool parseTypeExpression();
    
    /**
     * parse DECLARE statement
     */
    bool parseDeclare();
    
    /**
     * Parse DIM statement
     */
    bool parseDim();

};
