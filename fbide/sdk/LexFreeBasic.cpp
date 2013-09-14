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
    "Keywords",
    nullptr
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
    return nullptr;
}


/**
 * Set keywords
 */
int LexerFreeBasic::WordListSet(int n, const char *wl)
{
    m_keywords.Set(wl);
    return 0;
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
        
        // current state
        if (sc.state == Style::Number) {
            if (!std::isdigit(sc.ch)) {
                sc.SetState(Style::Default);
            }
        } else if (sc.state == Style::Identifier) {
            if (!std::isalnum(sc.ch)) {
                char buffer[32];
                sc.GetCurrentLowered(buffer, sizeof(buffer));
                if (m_keywords.InList(buffer)) {
                    sc.ChangeState(Style::Keyword);
                }
                sc.SetState(Style::Default);
            }
        }
        
        // deice what to do
        if (sc.state == Style::Default) {
            // is this a digit ?
            if (std::isdigit(sc.ch)) {
                sc.SetState(Style::Number);
            } else if (std::isalpha(sc.ch)) {
                sc.SetState(Style::Identifier);
            }
        }
        
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























