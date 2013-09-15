//
//  LexFreeBasic.cpp
//  fbide
//
//  Created by Albert on 13/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//
#include "SciFBLexer.h"
#include "SourceContext.h"
#include "scintilla/WordList.h"
#include "scintilla/LexAccessor.h"
#include "scintilla/StyleContext.h"
#include "scintilla/CharacterSet.h"
#include "scintilla/LexerModule.h"
#include "scintilla/OptionSet.h"
#include "scintilla/Catalogue.h"


/**
 * is this an operator character ?
 */
static inline bool is_operator(int ch)
{
    return ch == '='
        || ch == '('
        || ch == ')'
        || ch == ','
        || ch == ':';
}


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
void SciFBLexer::registerLexer()
{
    static LexerModule module(5000, SciFBLexer::Create, "fbide-freebasic", freebasicWordListDesc);
    Catalogue::AddLexerModule(&module);
}


/**
 * create new lexer instance
 */
ILexer * SciFBLexer::Create()
{
    return new SciFBLexer();
}


/**
 * Release the instance
 */
void SciFBLexer::Release()
{
    delete this;
}


/**
 * Create new instance of the lexer
 */
SciFBLexer::SciFBLexer() : m_srcCtx(std::make_shared<SourceContext>())
{
}


/**
 * communicate with the lexer from outside
 */
void * SciFBLexer::PrivateCall(int, void *)
{
    return nullptr;
}


/**
 * Set keywords
 */
int SciFBLexer::WordListSet(int n, const char *wl)
{
    m_keywords.Set(wl);
    return 0;
}


/**
 * Lex the source
 */
void SciFBLexer::Lex(unsigned int startPos, int length, int initStyle, IDocument *pAccess)
{
    // set the buffer pointer.
    // !NB this might change at runtime (reallocation ...) so
    // always update it.
    m_srcCtx->setBuffer(pAccess->BufferPointer());
    
    // analyze the source dode
    auto ast = m_srcCtx->analyze(pAccess->LineFromPosition(startPos), startPos, length);
    if (ast != nullptr) {
        std::cout << "Ast is not null\n";
    }
    
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
        } else if (sc.state == Style::String) {
            if (sc.Match('"')) {
                sc.ForwardSetState(Style::Default);
            } else if (sc.atLineEnd) {
                sc.SetState(Style::Default);
            }
        } else if (sc.state == Style::Operator) {
            if (!is_operator(sc.ch)) sc.SetState(Style::Default);
        }
        
        
        // deice what to do
        if (sc.state == Style::Default) {
            // is this a digit ?
            if (std::isdigit(sc.ch)) {
                sc.SetState(Style::Number);
            } else if (std::isalpha(sc.ch)) {
                sc.SetState(Style::Identifier);
            } else if (sc.Match('"')) {
                sc.SetState(Style::String);
            } else if (is_operator(sc.ch)) {
                sc.SetState(Style::Operator);
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
void SciFBLexer::Fold(unsigned int startPos, int length, int /* initStyle */, IDocument *pAccess)
{
    return;
}























