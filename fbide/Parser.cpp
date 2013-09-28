//
//  Parser.cpp
//  fbide
//
//  Created by Albert on 16/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//
#include "Parser.h"
#include "Token.h"
#include "SourceScope.h"

#define EXPECT(_expr) if ((_expr) == false) return false;


/**
 * Skip comment nodes
 */
static inline TokenPtr skipComments(TokenPtr token)
{
    while (token && token->getKind() == TokenKind::Comment) token = token->getNext();
    return token;
}


/**
 * Create Parser object
 */
Parser::Parser(SourceScopePtr scope)
: m_token(nullptr), m_next(nullptr), m_lastToken(nullptr), m_scope(scope)
{}


/**
 * Set root node
 */
void Parser::setRoot(TokenPtr token)
{
    m_next      = skipComments(token);
    m_lastToken = m_next;
    move();
}


/**
 * return true if next token matches the type
 */
bool Parser::match(TokenKind kind)
{
    if (m_token == nullptr) return false;
    return m_token->getKind() == kind;
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
    if (m_next) m_lastToken = m_next;
    m_token = m_next;
    m_next  = m_next ? skipComments(m_next->getNext()) : nullptr;
}


/**
 * Parse
 */
bool Parser::parse()
{
    // set first token
    m_scope->setFirstToken(m_token);
    
    // iterate through the lines
    while (m_token) {
        // DIM
        if      (match(TokenKind::DIM))      parseDim();
        // DECLARE
        else if (match(TokenKind::DECLARE))  parseDeclare();
        // FUNCTION
        else if (match(TokenKind::FUNCTION)) parseFunction();
        
        // not end of the line?
        if (!match(TokenKind::EndOfLine)) {
            if (m_token) m_token->setValid(false);
        }
        move();
    };
    
    // set last token
    m_scope->setLastToken(m_lastToken);
    
    // done
    return true;
}


/**
 * parse a scoped block
 */
bool Parser::parseBlock()
{
    while (m_token) {
        // DIM
        if (match(TokenKind::DIM)) parseDim();
        
        // END ?
        else if (accept(TokenKind::END)) {
            return accept(m_scopeStack.top());
        }
        
        // not end of the line?
        if (m_token) {
            if (!match(TokenKind::EndOfLine)) {
                m_token->setValid(false);
            }
        } else {
            break;
        }
        move();
    };
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
    if (!m_scope->addSymbol(m_token)) {
        m_token->setValid(false);
    }
    move();
    
    // "("
    EXPECT( accept(TokenKind::ParenOpen) );
    
    // TODO parameters
    if (!match(TokenKind::ParenClose)) EXPECT( parseArgList() );
    
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
bool Parser::parseArgList()
{
    // Argument { "," Argument }
    do {
        // Identifier
        EXPECT( match(TokenKind::Identifier) );
        if (m_scope->getType() == ScopeType::FunctionImplementation) {
            if (!m_scope->addSymbol(m_token)) {
                m_token->setValid(false);
            }
        }
        move();
        
        // "AS"
        EXPECT( accept(TokenKind::AS) );
        
        // Type
        EXPECT( parseTypeExpression() );
        
    } while (accept(TokenKind::Comma));
    
    return true;
}



/**
 * parse FUNCTION statement
 */
bool Parser::parseFunction()
{
    // FUNCTION
    EXPECT( accept(TokenKind::FUNCTION) );
    
    // identifier
    EXPECT( match(TokenKind::Identifier) );
    if (!m_scope->addSymbol(m_token)) {
        m_token->setValid(false);
    }
    move();
    
    // "("
    EXPECT( accept(TokenKind::ParenOpen) );
    
    // create new scope
    auto tmp = m_scope;
    m_scope = SourceScope::create(ScopeType::FunctionImplementation, m_scope);
    SCOPED_GUARD([&]{
        m_scope = tmp;
    });

    // TODO parameters
    if (!match(TokenKind::ParenClose)) EXPECT( parseArgList() );
        
    // ")"
    EXPECT( accept(TokenKind::ParenClose) );
    
    // "AS"
    EXPECT( accept(TokenKind::AS) );
    
    // Type
    EXPECT( parseTypeExpression() );
    
    // end of line
    EXPECT( accept(TokenKind::EndOfLine) );
    
    // expect END FUNCTION
    m_scopeStack.push(TokenKind::FUNCTION);
    m_scope->setFirstToken(m_token);
    bool success = parseBlock();
    m_scope->setLastToken(m_lastToken);
    m_scopeStack.pop();
    
    // done
    return success && match(TokenKind::EndOfLine);
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
    if (!m_scope->addSymbol(m_token)) {
        m_token->setValid(false);
    }
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








