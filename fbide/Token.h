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
 * Forward ref
 */
class Token;

/**
 * Token access type
 */
typedef std::shared_ptr<Token> TokenPtr;


/**
 * Token type
 */
enum class TokenKind
{
    #define _tkn_enums(_tkn, ...) _tkn,
    TOKEN_ALL(_tkn_enums)
    #undef _tkn_enums
};


/**
 * Represnt token location in a source
 */
struct TokenLoc
{
    
    /**
     * Default construct
     */
    TokenLoc() = default;
    
    /**
     * Create location
     */
    TokenLoc(int line, int col, int length, int endLine = -1, int endCol = -1)
    : line(line),
      col(col),
      length(length),
      endLine(endLine == -1 ? line : endLine),
      endCol(endCol == -1 ? col : endCol)
    {}
    
    // members
    int line        = 0;
    int col         = 0;
    int length      = 0;
    int endLine     = 0;
    int endCol      = 0;
};


/**
 * Represent a single token.
 */
struct Token
{
    
    /**
     * create pooled token object
     */
    static TokenPtr create(TokenKind kind, const TokenLoc & loc, std::string lexeme = "");
    
    
    /**
     * Create new token
     */
    Token(TokenKind kind, const TokenLoc & loc, std::string lexeme = "");
    
    
    /**
     * Clean up
     */
    ~Token();
    
    
    /**
     * Get token type
     */
    inline TokenKind getKind() const { return m_kind; }
    
    
    /**
     * Get token line
     */
    inline int getLine() const { return m_loc.line; }
    
    /**
     * get line where token ends
     */
    inline int getEndLine() const { return m_loc.endLine; }
    
    
    /**
     * Get token offset column in the line
     */
    inline int getCol() const { return m_loc.col; }
    
    /**
     * get end column of the token
     */
    inline int getEndCol() const { return m_loc.endCol; }
    
    
    /**
     * Get token length
     */
    inline int getLength() const { return m_loc.length; }
    
    
    /**
     * Get token lexeme
     */
    inline const std::string & getLexeme() const { return m_lexeme; }
    
    
    /**
     * Get next token
     */
    inline TokenPtr getNext() const { return m_next; }
    
    
    /**
     * Set the next token
     */
    inline void setNext(TokenPtr next) { m_next = next; }
    
    
    /**
     * print out token information
     */
    std::string toString();
    
private:
    
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
    TokenPtr m_next;

};
