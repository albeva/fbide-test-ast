//
//  Parser.cpp
//  fbide
//
//  Created by Albert on 16/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//

#include "Parser.h"
#include <exception>


#define EXPECT(_expr) if ((_expr) == false) return false;


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
bool Parser::parse()
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
    
    // sort
    std::unique(identifiers.begin(), identifiers.end());
    std::sort(identifiers.begin(), identifiers.end());
    
    // done
    return true;
}


/**
 * Parse any expression
 */
bool Parser::parseExpression()
{
    return accept(TokenKind::StringLiteral)
        || accept(TokenKind::NumberLiteral);
}


/**
 * Parse a type expression
 */
bool Parser::parseTypeExpression()
{
    return accept(TokenKind::INTEGER)
        || accept(TokenKind::STRING);
}


/**
 * parse DECLARE statement
 */
bool Parser::parseDeclare()
{
    // "DECLARE"
    EXPECT( accept(TokenKind::DECLARE) );
    
    // "FUNCTION
    EXPECT( accept(TokenKind::FUNCTION) );
    
    // identifier
    EXPECT( match(TokenKind::Identifier) );
    identifiers.push_back(m_token->getOriginal());
    move();
    
    // "("
    EXPECT( accept(TokenKind::ParenOpen) );
    
    // TODO parameters
    if (!match(TokenKind::ParenClose)) EXPECT( parseArguList() )
    
    // ")"
    EXPECT( accept(TokenKind::ParenClose) );
    
    // "AS"
    EXPECT( accept(TokenKind::AS) );
    
    // Type
    EXPECT( parseTypeExpression() );
    
    // done
    return match(TokenKind::EndOfLine);
}


/**
 * Parse agurment list
 */
bool Parser::parseArguList()
{
    // Argument { "," Argument }
    do {
        // Identifier
        EXPECT( accept(TokenKind::Identifier) );
        
        // "AS"
        EXPECT( accept(TokenKind::AS) );
        
        // Type
        EXPECT( parseTypeExpression() );
        
    } while (accept(TokenKind::Comma));
    
    return true;
}


/**
 * Parse DIM statement
 */
bool Parser::parseDim()
{
    // DIM
    EXPECT( accept(TokenKind::DIM) );
    
    // identifier
    EXPECT( match(TokenKind::Identifier) );
    identifiers.push_back(m_token->getOriginal());
    move();
    
    // AS
    EXPECT( accept(TokenKind::AS) );
    
    // Type
    EXPECT( parseTypeExpression() );
    
    // the end ?
    if (accept(TokenKind::Assign)) {
        // Expression
        EXPECT( parseExpression() );
    }
    
    // done ok
    return match(TokenKind::EndOfLine);
}
