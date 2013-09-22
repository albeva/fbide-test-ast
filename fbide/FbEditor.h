//
//  FbEditor.h
//  fbide
//
//  Created by Albert on 21/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//
#pragma once
#include <wx/stc/stc.h>

class SourceContext;


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
    String
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
    
    /**
     * set simple style
     */
    void setStyle(TokenStyle style, std::string color, bool bold = false, bool italic = false);
    
    /**
     * source is modified
     */
    void onModified(wxStyledTextEvent & event);
    
    /**
     * need to style
     */
    void onStyleNeeded(wxStyledTextEvent & event);
    
private:
    
    std::shared_ptr<SourceContext> m_srcCtx;
    DECLARE_EVENT_TABLE();
};
