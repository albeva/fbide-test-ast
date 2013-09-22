//
//  SourceContext.cpp
//  fbide
//
//  Created by Albert on 15/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//
#include "SourceContext.h"
#include "Ast.h"
#include "Lexer.h"
#include "Parser.h"
#include "Token.h"


/**
 * Create
 */
SourceContext::SourceContext() : m_buffer(nullptr)
{}


/**
 * Run the internal analyses machinery and return
 * the first suitable ast node for that line
 * or null. Ast node return must not be
 * deleted or modified.
 */
std::shared_ptr<Token> SourceContext::analyze(int line, int offset, int length)
{
    // create the lexer
    Lexer lexer(this->getBuffer());
    
    // reset current token tree
    m_root = nullptr;
    
    // out token
    std::shared_ptr<Token> out;
    
    // tokens
    std::shared_ptr<Token> token;
    do {
        auto t = lexer.next();
        if (!m_root) m_root = t;
        if (token) token->m_next = t;
        token = t;
        if (!out && token->m_loc.line == line) out = token;
    } while (token->m_kind != TokenKind::EndOfFile);
    
    // done
    return out;
}


/**
 * Get first token that is at the start of the line
 */
std::shared_ptr<Token> SourceContext::getLine(int line, int last)
{
    auto token = m_root;
    while (token) {
        int l = token->m_loc.line;
        if (l == line) return token;
        if (l > line) {
            if (l <= last) return token;
            return nullptr;
        }
        token = token->m_next;
    }
    return nullptr;
}



