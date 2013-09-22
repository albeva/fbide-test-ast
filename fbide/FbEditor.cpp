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

#define LOG(_msg) std::cout << _msg << '\n';
#define LOG_V(_var) std::cout << #_var " = " << _var << '\n';
    
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
    EVT_STC_MODIFIED(wxID_ANY,      FbEditor::onModified)
    EVT_STC_STYLENEEDED(wxID_ANY,   FbEditor::onStyleNeeded)
END_EVENT_TABLE()


/**
 * create
 */
FbEditor::FbEditor(wxWindow *parent, wxWindowID id) : wxStyledTextCtrl(parent, id)
{
    // the source context
    m_srcCtx = std::make_shared<SourceContext>();
    
    // use container styling
    SetLexer(wxSTC_LEX_CONTAINER);
    
    // default
    setStyle(TokenStyle::Default,      "black");
    setStyle(TokenStyle::Identifier,   "black");
    setStyle(TokenStyle::Number,       "blue");
    setStyle(TokenStyle::Keyword,      "maroon", true);
    setStyle(TokenStyle::String,       "red");
    setStyle(TokenStyle::Operator,     "purple");
}



/**
 * Set style
 */
void FbEditor::setStyle(TokenStyle style, std::string color, bool bold, bool italic)
{
    StyleSetForeground((int)style, wxColour(color));
    StyleSetFontAttr((int)style, 16, "Consolas", bold, italic, false);
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
 * Style needed
 */
void FbEditor::onStyleNeeded(wxStyledTextEvent & event)
{
    // startint position
    auto startPos = GetEndStyled();
    auto startLine= LineFromPosition(startPos);
    startPos      = PositionFromLine(startLine);
    // end position
    int lastPos   = event.GetPosition();
    int lastLine  = LineFromPosition(lastPos);
    
    // get token
    auto token = m_srcCtx->getLine(startLine, lastLine);
    
    if (!token) {
        StartStyling(startPos, INT_MAX);
        SetStyling(lastPos - startPos, (int)TokenStyle::Default);
        return;
    }
    
    // go through the lines
    for (int line = startLine; line <= lastLine; line++) {
        StartStyling(PositionFromLine(line), INT_MAX);

        int col = 0;
        int length = GetLineLength(line);

        while (token != nullptr
               && token->getLine() == line
               && token->getKind() != TokenKind::EndOfFile) {
            
            // skip end of line token
            if (token->getKind() == TokenKind::EndOfLine) {
                token = token->getNext();
                break;
            }
            
            int p = token->getCol() - 1;
            if (p > col) {
                SetStyling(p - col, (int)TokenStyle::Default);
            }
            col = p;

            SetStyling(token->getLength(), (int)_tokenStyles[(int)token->getKind()]);
            col += token->getLength();

            // next token
            token = token->getNext();
        }
        
        // colorize rest of the line
        if (col < length) {
            SetStyling(length - col, (int)TokenStyle::Default);
        }
    }
}






