//
//  LexFreeBasic.cpp
//  fbide
//
//  Created by Albert on 13/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//
#include "LexFreeBasic.h"
#include <map>
#include <iostream>
#include "scintilla/WordList.h"
#include "scintilla/LexAccessor.h"
#include "scintilla/StyleContext.h"
#include "scintilla/CharacterSet.h"
#include "scintilla/LexerModule.h"
#include "scintilla/OptionSet.h"
#include "scintilla/Catalogue.h"


/**
 * Describe keyword list
 */
static const char * const freebasicWordListDesc[] = {
	"FreeBasic Keywords",
	"FreeBasic PreProcessor Keywords",
	"user defined 1",
	"user defined 2",
	0
};


/**
 * register the lexer with scintilla
 */
void LexerFreeBasic::registerLexer()
{
    static LexerModule module(5000, LexerFreeBasic::Create, "fbide-freebasic", freebasicWordListDesc);
    Catalogue::AddLexerModule(&module);
}


/**
 * create new lexer instance
 */
ILexer * LexerFreeBasic::Create()
{
    return new LexerFreeBasic();
}


/**
 * Release the instance
 */
void LexerFreeBasic::Release()
{
    delete this;
}


/**
 * communicate with the lexer from outside
 */
void * LexerFreeBasic::PrivateCall(int, void *)
{
    std::cout << __PRETTY_FUNCTION__ << '\n';
    return nullptr;
}


/**
 * Lex the source
 */
void LexerFreeBasic::Lex(unsigned int startPos, int length, int initStyle, IDocument *pAccess)
{
    // the accessor
	LexAccessor styler(pAccess);
    
    // start at
	styler.StartAt(startPos);
    
    // context
	StyleContext sc(startPos, length, initStyle, styler);

	// Can't use sc.More() here else we miss the last character
	for (; ; sc.Forward()) {
        
        // set defaulr
        sc.SetState(SCE_B_DEFAULT);
        
        // break when done
		if (!sc.More()) break;
	}
    
    // done
	sc.Complete();
}


/**
 * Fold the source
 */
void LexerFreeBasic::Fold(unsigned int startPos, int length, int /* initStyle */, IDocument *pAccess)
{
	return;
}
