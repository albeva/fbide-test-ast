//
//  LexFreeBasic.h
//  fbide
//
//  Created by Albert on 13/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//
#pragma once
#include "scintilla/ILexer.h"
#include "scintilla/Scintilla.h"
#include "scintilla/SciLexer.h"


/**
 * Lex freebasic source
 */
struct LexerFreeBasic : ILexer
{
    /**
     * Register the lexer with the scintilla
     */
    static void registerLexer();
    
    /**
     * Create new instance
     */
	static ILexer * Create();
    
    /**
     * Destroy this lexer
     */
	void Release();
    
    /**
     * Create new instance of the lexer
     */
	LexerFreeBasic() {}
    
    /**
     * Destroy the lexer
     */
	virtual ~LexerFreeBasic() {}

    /**
     * Get lexer? version
     */
	int Version() const
    {
		return -1;
	}
    
    /**
     * Get property names
     */
	const char * PropertyNames()
    {
		return nullptr;
	}
    
    /**
     * Get property type
     */
	int PropertyType(const char *name)
    {
		return -1;
	}
    
    /**
     * Describe proprty
     */
	const char * DescribeProperty(const char *name)
    {
		return nullptr;
	}
    
    /**
     * Set property
     */
	int PropertySet(const char *key, const char *val)
    {
        return 0;
    }
    
    /**
     * Describe keywords list
     */
	const char * DescribeWordListSets()
    {
		return nullptr;
	}
    
    /**
     * ?
     */
	int WordListSet(int n, const char *wl)
    {
        return 0;
    }
    
    /**
     * Lex
     */
	void Lex(unsigned int startPos, int length, int initStyle, IDocument *pAccess);
    
    /**
     * Fold
     */
	void Fold(unsigned int startPos, int length, int initStyle, IDocument *pAccess);
    
    /**
     * Communicate with lexer from outside
     */
	void * PrivateCall(int, void *);
    
};
