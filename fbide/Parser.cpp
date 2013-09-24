//
//  Parser.cpp
//  fbide
//
//  Created by Albert on 16/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//

#include "Parser.h"
#include <exception>


/**
 * Skip comment nodes
 */
static inline TokenPtr skipComments(TokenPtr token)
{
    while (token && token->getKind() == TokenKind::Comment) token = token->getNext();
    return token;
}


Parser::Parser()
: m_token(nullptr), m_next(nullptr)
{
}


/**
 * Set root node
 */
void Parser::setRoot(TokenPtr token)
{
    m_next = skipComments(token);
    move();
}


/**
 * return true if next token matches the type
 */
bool Parser::match(TokenKind kind)
{
    return m_token && m_token->getKind() == kind;
}


/**
 * match the token and advance. Return result
 */
bool Parser::accept(TokenKind kind)
{
    if (match(kind)) {
        move();
        return true;
    }
    return false;
}


/**
 * expect next token to be of type. Mark as invalid if doesn't match
 */
bool Parser::expect(TokenKind kind)
{
    if (!match(kind)) {
        // throw std::exception(); ?
    }
    return false;
}


/**
 * move to the next token
 */
void Parser::move()
{
    m_token = m_next;
    m_next  = m_next ? skipComments(m_next->getNext()) : nullptr;
}


/**
 * Parse
 */
void Parser::parse()
{
    while (m_token) {
        // DIM
        if      (match(TokenKind::DIM))     parseDim();
        // DECLARE
        else if (match(TokenKind::DECLARE)) parseDeclare();
        
        // not end of the line?
        if (!match(TokenKind::EndOfLine)) {
            m_token->setValid(false);
        }
        move();
    };
}


/**
 * parse DECLARE statement
 */
void Parser::parseDeclare()
{
    
}


/**
 * Parse DIM statement
 */
void Parser::parseDim()
{
    // DIM
    if (!accept(TokenKind::DIM)) return;
    
    // identifier
    if (!accept(TokenKind::Identifier)) return;
    
    // AS
    if (!accept(TokenKind::AS)) return;
    
    // STRING | INTEGER
    if (match(TokenKind::INTEGER) || match(TokenKind::STRING)) {
        move();
    } else {
        return;
    }
    
    // [ = Expression ]
    if (!accept(TokenKind::Assign)) return;
    
    // IntegerLiteral | StringLiteral
    if (match(TokenKind::NumberLiteral) || match(TokenKind::StringLiteral)) {
        move();
    } else {
        return;
    }
}






