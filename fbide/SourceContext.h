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
    const Ast * analyze(int line, unsigned int offset, int length);
    
private:
    
    /**
     * input source
     */
    const char * m_buffer;
};