//
//  Lexer.cpp
//  fbide
//
//  Created by Albert on 16/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//

#include "Lexer.h"

namespace {
    
    /**
     * keywords map
     */
    static const std::unordered_map<std::string, TokenKind> _keywords = {
        {"DIM",     TokenKind::DIM},
        {"AS",      TokenKind::AS},
        {"STRING",  TokenKind::STRING},
        {"INTEGER", TokenKind::INTEGER},
        {"FUNCTION",TokenKind::FUNCTION},
        {"DECLARE", TokenKind::DECLARE},
        {"END",     TokenKind::END},
    };
    
    /**
     * simple operators
     */
    static const std::unordered_map<char, TokenKind> _operators = {
        {':', TokenKind::Colon},
        {'=', TokenKind::Assign},
        {',', TokenKind::Comma},
        {'(', TokenKind::ParenOpen},
        {')', TokenKind::ParenClose},
    };
    
    /**
     * is this a whitespace?
     */
    static inline bool is_space(char ch)
    {
        return ch == ' ' || ch == '\t';
    }
    
    /**
     * is char valid in identifier body?
     */
    static inline bool is_ident_body(char ch)
    {
        return std::isalnum(ch) || ch == '_';
    }
    
    
    /**
     * is char valid at the beginning of the identifier ?
     */
    static inline bool is_ident_head(char ch)
    {
        return std::isalpha(ch) || ch == '_';
    }
}


/**
 * create new instance of the lexer
 */
Lexer::Lexer(const char * input)
: m_input(input),
  m_ch(0),
  m_next(0),
  m_loc(0, -1, 0),
  m_tokenStart(input),
  m_hasStmt(false),
  m_readOk(false)
{
}


/**
 * Read in next character
 */
bool Lexer::read()
{
    if (m_readOk == false) {
        m_readOk = true;
        // line ending?
        if (m_ch == '\n') {
            m_loc.line++;
            m_loc.col = 0;
        } else {
            m_loc.col++;
        }
    
        // read character
        m_ch = *m_input;
    
        // read next
        if (m_ch == '\0') m_next = '\0';
        else              m_next = m_input[1];
    }
    
    // is end of input?
    return m_ch != '\0';
}


/**
 * advance by 1 and read in the next character
 */
bool Lexer::readNext()
{
    if (*m_input != '\0') {
        if (!m_readOk) read();
        m_readOk = false;
        m_input++;
    }
    return read();
}


/**
 * get next token from the stream
 */
TokenPtr Lexer::next()
{
    while (read()) {
        
        // space
        if (is_space(m_ch)) {
            while(readNext() && is_space(m_ch));
            if (m_ch == '\0') break;
        }
        
        // mark token location
        m_tokenLoc = m_loc;
        
        // is line end
        if (m_ch == '\n') {
            readNext();
            m_hasStmt = false;
            return token(TokenKind::EndOfLine);
        }
        
        // there is something here
        m_hasStmt    = true;
        m_tokenStart = m_input;
        
        // single line comment ?
        if (m_ch == '\'') {
            while (readNext() && m_ch != '\n');
            return token(TokenKind::Comment);
        }
        
        // multiline comment
        if (m_ch == '/' && m_next == '\'') {
            readNext();
            int level = 1;
            while (readNext() && level > 0) {
                if (m_ch == '\'' && m_next == '/') {
                    readNext();
                    level--;
                } else if (m_ch == '/' && m_next == '\'') {
                    readNext();
                    level++;
                }
            }
            if (m_ch == '/') readNext();
            return token(TokenKind::Comment);
        }
        
        // possible identifier?
        if (is_ident_head(m_ch)) return identifier();
        
        // possible number ?
        if (std::isdigit(m_ch)) return number();
        
        // is it a string ?
        if (m_ch == '"') return string();
        
        // an operator ?
        auto iter = _operators.find(m_ch);
        if (iter != _operators.end()) {
            readNext();
            return token(iter->second);
        }
        
        // an invalid character
        auto c = m_ch;
        readNext();
        return token(TokenKind::Invalid, std::string() + c);
    }
    
    // finished. send of of statement if needed
    if (m_hasStmt) {
        m_hasStmt = false;
        return token(TokenKind::EndOfLine);
    }
    
    // really the end
    return nullptr;
}


/**
 * read number
 */
TokenPtr Lexer::number()
{
    // iterate while character is a number
    while (readNext() && std::isdigit(m_ch));
    // done
    return token(TokenKind::NumberLiteral);
}


/**
 * read string
 */
TokenPtr Lexer::string()
{
    while (readNext() && m_ch != '"' && m_ch != '\n');
    if (m_ch == '"') readNext();
    
    return token(TokenKind::StringLiteral);
}


/**
 * Read identifier. Might be a keyword
 */
TokenPtr Lexer::identifier()
{
    // iterate input while there is a number or a letter
    while (readNext() && is_ident_body(m_ch));
    
    // get uppercased lexeme
    std::string lexeme;
    std::transform(m_tokenStart, m_input, std::back_inserter(lexeme), ::toupper);
    
    // is this a keyword?
    auto iter = _keywords.find(lexeme);
    return token(iter == _keywords.end() ? TokenKind::Identifier : iter->second, lexeme, std::string(m_tokenStart, m_input));
}


/**
 * create new token
 */
TokenPtr Lexer::token(TokenKind kind, std::string lexeme, std::string original)
{
    if (kind != TokenKind::EndOfLine) {
        m_tokenLoc.length  = (int)(m_input - m_tokenStart);
        m_tokenLoc.endLine = m_loc.line;
        m_tokenLoc.endCol  = m_loc.col;
    } else {
        m_tokenLoc.endLine = m_tokenLoc.line;
        m_tokenLoc.col     = m_tokenLoc.col;
    }
    return Token::create(kind, m_tokenLoc, lexeme, original);
}





































