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
std::shared_ptr<Token> Token::create(TokenKind kind, const TokenLoc & loc, std::string lexeme, std::string original)
{
    return std::make_shared<Token>(kind, loc, lexeme, original);
}


/**
 * Create new token
 */
Token::Token(TokenKind kind, const TokenLoc & loc, std::string lexeme, std::string original)
: m_kind(kind),
  m_loc(loc),
  m_isValid(true),
  m_errorCode(0),
  m_lexeme(lexeme),
  m_original(original),
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
    out += " {\n\tline:    " + std::to_string(getLine())
         +   "\n\tcol:     " + std::to_string(getCol())
         +   "\n\tlen:     " + std::to_string(getLength())
         +   "\n\tendLine: " + std::to_string(getEndLine())
         +   "\n\tendCol:  " + std::to_string(getEndCol())
         +   "\n\tlexeme:  " + m_lexeme
         + "\n}";
    
    return out;
}