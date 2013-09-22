//
//  Token.h
//  fbide
//
//  Created by Albert on 21/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//
#pragma once
#include "Token.def.h"

/**
 * Token type
 */
enum class TokenKind
{
    #define TOKEN_ENUM(_tkn, ...) _tkn,
    TOKEN_ALL(TOKEN_ENUM)
    #undef TOKEN_ENUM
};


/**
 * Represnt token location in a source
 */
struct TokenLoc
{
    /**
     * Create location
     */
    TokenLoc(int line = 0, int col = 0, int pos = 0, int length = 0)
    : line(line), col(col), pos(pos), length(length)
    {}
    
    int line;
    int col;
    int pos;
    int length;
};


/**
 * Represent a single token.
 */
struct Token
{
    
    /**
     * Create new token
     */
    Token(TokenKind kind, const TokenLoc & loc, std::string lexeme = "");
    
    
    /**
     * create pooled token object
     */
    static std::shared_ptr<Token> create(TokenKind kind, const TokenLoc & loc, std::string lexeme = "");
    
    
    /**
     * Clean up
     */
    ~Token();
    
    
    /**
     * Token kind
     */
    const TokenKind m_kind;
    
    /**
     * Token location
     */
    const TokenLoc m_loc;
    
    /**
     * token lexeme
     */
    const std::string m_lexeme;
    
    /**
     * next token. Linked list
     */
    std::shared_ptr<Token> m_next;
    
    /**
     * print out token information
     */
    std::string toString();
};
