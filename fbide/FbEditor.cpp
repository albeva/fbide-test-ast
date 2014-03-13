//
//  FbEditor.cpp
//  fbide
//
//  Created by Albert on 21/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//

#include "FbEditor.h"
#include "Token.h"
#include "SourceContext.h"
    
/**
 * map token types to the style used in the editor
 */
static const TokenStyle _tokenStyles[] = {
    #define _tkn_styles(_id, _str, _style, ...) TokenStyle::_style,
    TOKEN_ALL(_tkn_styles)
    #undef _tkn_styles
};


/**
 * Event table for the editor
 */
BEGIN_EVENT_TABLE (FbEditor, wxStyledTextCtrl)
    EVT_STC_MODIFIED    ( wxID_ANY, FbEditor::onModified    )
    EVT_STC_STYLENEEDED ( wxID_ANY, FbEditor::onStyleNeeded )
    EVT_STC_CHARADDED   ( wxID_ANY, FbEditor::onCharAdded   )
END_EVENT_TABLE()


// indicators
enum {
    ErrorIndicator = wxSTC_INDIC_CONTAINER
};


/**
 * create
 */
FbEditor::FbEditor(wxWindow *parent, wxWindowID id) : wxStyledTextCtrl(parent, id)
{
    // the source context
    m_srcCtx = std::make_shared<SourceContext>();
    
    // use container styling
    SetLexer(wxSTC_LEX_CONTAINER);
    
    // ingore case
    AutoCompSetIgnoreCase(true);
    
    // default
    setStyle(TokenStyle::Default,      "black");
    setStyle(TokenStyle::Identifier,   "black");
    setStyle(TokenStyle::Number,       "blue");
    setStyle(TokenStyle::Keyword,      "purple", true);
    setStyle(TokenStyle::String,       "red");
    setStyle(TokenStyle::Operator,     "brown", true);
    setStyle(TokenStyle::Comment,      "gray", false, true);

    // error
    IndicatorSetStyle(ErrorIndicator, wxSTC_INDIC_SQUIGGLEPIXMAP);
    IndicatorSetForeground(ErrorIndicator, wxColour("red"));
}



/**
 * Set style
 */
void FbEditor::setStyle(TokenStyle style, std::string color, bool bold, bool italic, std::string bgcolor)
{
    StyleSetForeground((int)style, wxColour(color));
    StyleSetFontAttr((int)style, 16, "Consolas", bold, italic, false);
    
    if (bgcolor != "") {
        StyleSetBackground((int)style, wxColour(bgcolor));
        StyleSetEOLFilled((int)style, true);
    }
}


/**
 * source is modified
 */
void FbEditor::onModified(wxStyledTextEvent & event)
{
    // not directly modifying text?
    auto flags = event.GetModificationType();
    if ((flags & (wxSTC_MOD_INSERTTEXT | wxSTC_MOD_DELETETEXT)) == 0) return;

    // set input buffer
    m_srcCtx->setBuffer(GetCharacterPointer());

    // get modified line & line length
    int line     = LineFromPosition(event.GetPosition());
    int pos      = PositionFromLine(line);
    int length   = this->GetLineLength(line);

    // analyze the source
    m_srcCtx->analyze(line, pos, length);
}


/**
 * CHaracter added
 */ 
void FbEditor::onCharAdded(wxStyledTextEvent & event)
{
    int p = GetCurrentPos();
    auto w = GetTextRange(WordStartPosition(p, true), p);
    if (w.length() < 2) return;

    
    w.UpperCase();
    wxString words = "";
    int count = 0;
    for (auto id : m_srcCtx->getIdentifiers(LineFromPosition(p), p - LineFromPosition(p))) {
        wxString ID(id);
        ID.UpperCase();
        if (ID.compare(0, w.length(), w) == 0) {
            if (ID.length() == w.length()) continue;
            words += id + " ";
            count++;
        }
    }
    
    if (count > 0) {
        AutoCompShow((int)w.Length(), words);
    }
}


/**
 * Style needed
 */
void FbEditor::onStyleNeeded(wxStyledTextEvent & event)
{
    // startint position
    auto startPos  = GetEndStyled();
    auto startLine = LineFromPosition(startPos);
    startPos       = PositionFromLine(startLine);
    // end position
    int lastPos    = event.GetPosition();
    int lastLine   = std::max(LineFromPosition(lastPos), std::min(GetLineCount(), GetFirstVisibleLine() + LinesOnScreen()));
    lastPos        = GetLineEndPosition(lastLine);
        
    // get token
    auto token = m_srcCtx->getLine(startLine, lastLine);
    
    // set stylling position
    StartStyling(startPos, INT_MAX);
    
    // clear indicatirs
    SetIndicatorCurrent(ErrorIndicator);
    IndicatorClearRange(startPos, lastPos - startPos);
    
    // no token? just colour to default
    if (!token) {
        style(lastPos - startPos, TokenStyle::Default);
        return;
    }
    
    
    // style the tokens
    int line = startLine;
    int col  = 0;
    while (token && line <= lastLine) {
        // end of the line?
        if (token->getKind() == TokenKind::EndOfLine) {
            token = token->getNext();
            continue;
        }
        
        // token line
        int tline = token->getLine();
        
        // token started before current line
        if (line > tline) {
            int start = PositionFromLine(line);
            int end   = PositionFromLine(token->getEndLine()) + token->getEndCol();
            style(end - start, token);
            
            // end on line and column
            col  = token->getEndCol();
            line = token->getEndLine();
            
            // get next token and continue
            token = token->getNext();
            continue;
        }
        
        // empty lines before next token?
        if (line < tline) {
            int start = PositionFromLine(line) + col;
            int end   = PositionFromLine(tline) + token->getCol();
            style(end - start, TokenStyle::Default);
            
            // end on line and column
            line = token->getLine();
            col = token->getCol();
            continue;
        }
        
        // started on the current line
        if (line == tline) {
            // empty space ?
            if (token->getCol() > col) {
                style(token->getCol() - col, TokenStyle::Default);
            }
            
            // style the token
            style(token->getLength(), token);
            col = token->getEndCol();
            line = token->getEndLine();
            
            // advance to the next one
            token = token->getNext();
            continue;
        }
        
        // some empty space till end of the line
        int length = GetLineLength(line);
        if (col < length) {
            style(length - col, TokenStyle::Default);
        }
        
        // incement line
        line++;
        col = 0;
    }
}



/**
 * Style text based on the token for the length given
 */
void FbEditor::style(int length, TokenPtr token)
{
    if (token->getKind() == TokenKind::Invalid || !token->isValid()) {
        SetIndicatorCurrent(ErrorIndicator);
        IndicatorFillRange(GetEndStyled(), length);
    }
    style(length, _tokenStyles[(int)token->getKind()]);
}



/**
 * Style text with given style ID
 */
void FbEditor::style(int length, TokenStyle style)
{
    SetStyling(length, (int)style);
}


