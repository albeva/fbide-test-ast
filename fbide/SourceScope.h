//
//  SourceScope.h
//  fbide
//
//  Created by Albert on 28/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//
#pragma once
#include "Token.h"

class SourceScope;
typedef std::shared_ptr<SourceScope> SourceScopePtr;
typedef std::vector<TokenPtr> SymbolList;


/**
 * Parser scope state. This state
 * defines parser and semantic analyser behaviours accordingly
 * this is also used to compose proper code completition options
 */
enum class ScopeType
{
    SourceRoot,
    FunctionDeclaration,
    FunctionImplementation,
    Expression
};



/**
 * Source scope contains information
 * about block level scopes.
 * Every token belongs to one scope
 * and scopes belong to root scopes
 */
class SourceScope : public std::enable_shared_from_this<SourceScope>
{
public:
    
    /**
     * construct with a parent scope
     */
    SourceScope(ScopeType type) : m_type(type) {}
    
    /**
     * Create new scope object
     */
    static SourceScopePtr create(ScopeType type = ScopeType::SourceRoot, SourceScopePtr parent = nullptr);
    
    /**
     * Get scope type
     */
    inline ScopeType getType() const { return m_type; }
    
    /**
     * Get the parent scope
     */
    SourceScopePtr getParent() const { return m_parent.lock(); }
    
    /**
     * find inner-most scope that contains given line & position
     */
    SourceScopePtr findScope(int line, int pos);
    
    /**
     * Add new identifier.
     * return false if identifier with the given name already in the list
     */
    bool addSymbol(TokenPtr token);
    
    /**
     * Get declared symbols
     */
    const SymbolList & getSymbols() const { return m_symbols; }
    
    /**
     * find symbol
     */
    TokenPtr findSymbol(TokenPtr token);
    
    /**
     * Set first token
     */
    void inline setFirstToken(TokenPtr token) { m_firstToken = token; }
    
    /**
     * Set last token
     */
    void inline setLastToken(TokenPtr token) { m_lastToken = token; }
    
private:
    
    
    /**
     * Scope state
     */
    ScopeType m_type;
    
    /**
     * Parent scope
     */
    std::weak_ptr<SourceScope> m_parent;
    
    /**
     * child scopes
     */
    std::vector<SourceScopePtr> m_children;
    
    /**
     * List of tokens that declare identifier in the scope
     */
    SymbolList m_symbols;
    
    /**
     * first token that defines the scope
     */
    TokenPtr m_firstToken;
    
    /**
     * last token that ends scope range
     */
    TokenPtr m_lastToken;
};













