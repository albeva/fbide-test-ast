//
//  Token.cpp
//  fbide
//
//  Created by Albert on 21/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//

#include "Token.h"


namespace {
    static const char * const _tokenNames[] = {
        #define _tkn_names(_tkn, _str, ...) _str,
        TOKEN_ALL(_tkn_names)
        #undef _tkn_names
    };
}


/**
 * TODO: create & return a pooled token object.
 */
std::shared_ptr<Token> Token::create(TokenKind kind, const TokenLoc & loc, std::string lexeme)
{
    return std::make_shared<Token>(kind, loc, lexeme);
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
    out += " {" + std::to_string(getLine())
         + ", " + std::to_string(getCol())
         + ", " + std::to_string(getLength())
         + "} ";
    out += m_lexeme;
    
    return out;
}