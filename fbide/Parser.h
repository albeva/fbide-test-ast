//
//  Parser.h
//  fbide
//
//  Created by Albert on 16/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//
#pragma once

/**
 * Forward reference
 */
enum class TokenKind : int;
class SourceScope;
class Token;
typedef std::shared_ptr<Token> TokenPtr;
typedef std::shared_ptr<SourceScope> SourceScopePtr;


/**
 * process the tokens and analyze for syntax errors
 */
class Parser
{
    TokenPtr m_token;
    TokenPtr m_next;
    TokenPtr m_lastToken; // last non null token
    std::stack<TokenKind> m_scopeStack;
    SourceScopePtr m_scope;
    
public:
    
    /**
     * default constructor
     */
    Parser(SourceScopePtr scope);
    
    /**
     * set root token
     */
    void setRoot(TokenPtr token);
    
    /**
     * return true if next token matches the type
     */
    bool match(TokenKind kind);
    
    /**
     * match the token and advance. Return result
     */
    bool accept(TokenKind kind);
    
    /**
     * move to the next token
     */
    void move();
    
    /**
     * Parse
     */
    bool parse();
    
    /**
     * parse scoped block
     */
    bool parseBlock();
    
    /**
     * parse assignment or an expression
     */
    bool parseAssignmentOrExpression();
    
    /**
     * parse the expression
     */
    bool parseExpression();
    
    /**
     * parse function argument list in a declaration
     */
    bool parseArgList();
    
    /**
     * Parse function parameter list. At the call site in a func
     * call expression
     */
    bool parseParamList();
    
    /**
     * parse type expression
     */
    bool parseTypeExpression();
    
    /**
     * parse DECLARE statement
     */
    bool parseDeclare();
    
    /**
     * parse FUNCTION statement
     */
    bool parseFunction();
    
    /**
     * Parse DIM statement
     */
    bool parseDim();

};
