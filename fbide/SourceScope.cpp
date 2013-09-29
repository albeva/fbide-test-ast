//
//  SourceScope.cpp
//  fbide
//
//  Created by Albert on 28/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//

#include "SourceScope.h"
#include "Token.h"


/**
 * Create new scopre with type and a parent scope
 */
SourceScopePtr SourceScope::create(ScopeType type, SourceScopePtr parent)
{
    auto p = std::make_shared<SourceScope>(type);
    if (parent) {
        p->m_parent = parent;
        parent->m_children.push_back(p);
    }
    return p;
}


/**
 * find matching symbol
 */
TokenPtr SourceScope::findSymbol(TokenPtr token)
{
    // find symbol in current scope
    for (auto & sym : m_symbols) {
        if (sym->getLexeme() == token->getLexeme()) return sym;
    }
    
    // parent scope
    if (auto p = m_parent.lock()) return p->findSymbol(token);
    
    // didn't find
    return nullptr;
}


/**
 * Add new identifier.
 * return false if identifier with the given name already in the list
 */
bool SourceScope::addSymbol(TokenPtr token)
{
    // find existing
    auto iter = std::find_if(m_symbols.begin(), m_symbols.end(), [&](const TokenPtr & p) {
        return p->getLexeme() == token->getLexeme();
    });
    
    // foung means it contains symbol already
    if (iter != m_symbols.end()) return false;
    
    // add it
    m_symbols.push_back(token);
    
    // success
    return true;
}


/**
 * find inner-most scope that contains given line & position
 */
SourceScopePtr SourceScope::findScope(int line, int pos)
{
    // no token?
    if (!m_firstToken) return nullptr;
    
    // before last line
    if (m_lastToken) {
        if (m_lastToken->getEndLine() < line) return nullptr;
    
        // before current position on the line
        // if (m_lastToken->getEndLine() == line && m_lastToken->getEndCol() < pos) return nullptr;
    }
    
    // starts after ?
    if (m_firstToken->getLine() > line) return nullptr;
    
    // starts after current position
    // if (m_firstToken->getLine() == line && m_firstToken->getCol() > pos) return nullptr;
    
    // see if any of the child scopes returns more specific scope
    for (auto scope : m_children) {
        auto s = scope->findScope(line, pos);
        if (s) {
            return s;
        }
    }
    
    // return self
    return shared_from_this();
}












