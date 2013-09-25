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
void SourceContext::analyze(int line, int offset, int length)
{
    // create the lexer
    Lexer lexer(this->getBuffer());
    
    // reset current token tree
    m_root = nullptr;
    m_identifiers.clear();
    
    // tokens
    std::shared_ptr<Token> token, tmp;
    while ((tmp = lexer.next())) {
        if (!m_root) m_root = tmp;
        if (token) token->setNext(tmp);
        token = tmp;
    };
    
    // parse the tokens
    Parser parser;
    parser.setRoot(m_root);
    parser.parse();
    m_identifiers = parser.identifiers;
}


/**
 * Get first token that is at the start of the line
 */
std::shared_ptr<Token> SourceContext::getLine(int line, int last)
{
    auto token = m_root;
    while (token) {
        int l = token->getLine();
        if (l == line) return token;
        if (l > line) {
            if (l <= last) return token;
            return nullptr;
        }
        if (token->getEndLine() >= line) return token;
        token = token->getNext();
    }
    return nullptr;
}


/**
 * get available identifiers
 */
const std::vector<std::string> & SourceContext::getIdentifiers() const
{
    return m_identifiers;
}








































