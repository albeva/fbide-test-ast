//
//  FbEditor.h
//  fbide
//
//  Created by Albert on 21/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//
#pragma once
#include "Token.h"

class SourceContext;
class Token;

/**
 * Token styles that are used for colorizing
 */
enum class TokenStyle
{
    Default,
    Identifier,
    Keyword,
    Operator,
    Number,
    String,
    Comment
};


/**
 * FreeBASIC editor
 */
class FbEditor : public wxStyledTextCtrl
{
public:
    /**
     * create
     */
    FbEditor(wxWindow *parent, wxWindowID);
    
private:
    
    
    /**
     * set simple style
     */
    void setStyle(TokenStyle style, std::string color, bool bold = false, bool italic = false, std::string bgcolor = "");
    
    /**
     * Style text based on the token for the length given
     */
    inline void style(int length, TokenPtr token);
    
    /**
     * Style text with given style ID
     */
    inline void style(int length, TokenStyle style);
    
    /**
     * source is modified
     */
    void onModified(wxStyledTextEvent & event);
    
    /**
     * need to style
     */
    void onStyleNeeded(wxStyledTextEvent & event);
    
    
    /**
     * CHaracter added
     */
    void onCharAdded(wxStyledTextEvent & event);
    
    std::shared_ptr<SourceContext> m_srcCtx;
    DECLARE_EVENT_TABLE();
};
