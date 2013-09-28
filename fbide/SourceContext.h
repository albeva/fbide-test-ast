//
//  SourceContext.h
//  fbide
//
//  Created by Albert on 15/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//
#pragma once

/**
 * Abstract syntax tree node
 */
class Ast;
class Token;
class SourceScope;


/**
 * Source context holds all the information available
 * about 1 source file. It provides source lexing,
 * syntax and semantic analyses, query interface ...
 */
class SourceContext
{
public:
    
    /**
     * Create instance of the context
     */
    SourceContext();
    
    /**
     * Set the buffer
     */
    void setBuffer(const char * buffer) { m_buffer = buffer; }
    
    /**
     * get the current buffer
     */
    const char * getBuffer() const { return m_buffer; }
    
    /**
     * Run the internal analyses machinery and return
     * the first suitable ast node for that line
     * or null. Ast node return must not be
     * deleted or modified.
     */
    void analyze(int line, int offset, int length);
    
    /**
     * Get first token that is at the start of the line
     *
     * if last > line then find a first token between the given range
     */
    std::shared_ptr<Token> getLine(int line, int last = INT_MAX);
    
    
    /**
     * Get all available identifiers
     */
    std::vector<std::string> getIdentifiers(int line, int pos);
    
private:
    
    /**
     * input source
     */
    const char * m_buffer;
    
    /**
     * root token
     */
    std::shared_ptr<Token> m_root;
    
    /**
     * The root scope
     */
    std::shared_ptr<SourceScope> m_rootScope;
};



