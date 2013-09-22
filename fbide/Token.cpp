//
//  Token.cpp
//  fbide
//
//  Created by Albert on 21/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//

#include "Token.h"
#define BOOST_POOL_NO_MT
#include <boost/pool/object_pool.hpp>


namespace {
    static const char * const _tokenNames[] = {
        #define TOKEN_NAMES(_tkn, _str, ...) _str,
        TOKEN_ALL(TOKEN_NAMES)
        #undef TOKEN_NAMES
    };
    
    // token allocator
    boost::object_pool<Token> _allocator;
    
    // release
    void destroy(Token * token)
    {
        _allocator.destroy(token);
    }
}


/**
 * create pooled token object
 */
std::shared_ptr<Token> Token::create(TokenKind kind, const TokenLoc & loc, std::string lexeme)
{
    // return std::allocate_shared<Token>(_allocator, kind, loc, lexeme);
    return std::shared_ptr<Token>(_allocator.construct(kind, loc, lexeme), &destroy);
}


/**
 * Create new token
 */
Token::Token(TokenKind kind, const TokenLoc & loc, std::string lexeme)
: m_kind(kind),
  m_loc(loc),
  m_lexeme(lexeme),
  m_next(nullptr)
{}


/**
 * CLean up
 */
Token::~Token() = default;


/**
 * Turn token into a string
 */
std::string Token::toString()
{
    std::string out;
    
    out += _tokenNames[(int)m_kind];;
    out += " {";
    out += std::to_string(m_loc.line)
        + ", " + std::to_string(m_loc.col)
        + ", " + std::to_string(m_loc.length);
    out += "} ";
    out += m_lexeme;
    
    return out;
}