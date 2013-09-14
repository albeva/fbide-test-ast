//
//  main.cpp
//  fbide
//
//  Created by Albert on 11/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//

#include <iostream>
#include <wx/stc/stc.h>
#include "sdk/LexFreeBasic.h"


/**
 * The FBIde App class. This is where application
 * starts and ends its lifetime
 */
class FBIdeApp: public wxApp
{
    /**
     * App is about to start
     */
    virtual bool OnInit()
    {
        // register the lexer
        LexerFreeBasic::registerLexer();
        
        // create the window
        m_frame = new wxFrame(nullptr, wxID_ANY, "FBIde");
        
        // wxStyledTextCtrl
        m_stc = new wxStyledTextCtrl(m_frame, wxID_ANY);
        m_stc->SetLexer(5000);
        
        // default
        setStyle(LexerFreeBasic::Style::Default,    "black");
        setStyle(LexerFreeBasic::Style::Identifier, "black");
        
        // number
        setStyle(LexerFreeBasic::Style::Number, "blue");
        
        // keywords
        setStyle(LexerFreeBasic::Style::Keyword, "black", true);
        m_stc->SetKeyWords(0, "dim as string integer declare function end"); // end+function

        // show
        m_frame->Show();
        
        // all ok
        return true;
    }
    
    
    /**
     * Set style
     */
    void setStyle(int style, std::string color, bool bold = false, bool italic = false)
    {
        m_stc->StyleSetForeground(style, wxColour(color));
        m_stc->StyleSetFontAttr(style, 16, "Consolas", bold, italic, false);
    }
    
    
    // main window
    wxFrame * m_frame;
    
    // the editor
    wxStyledTextCtrl * m_stc;
};

IMPLEMENT_APP(FBIdeApp)
