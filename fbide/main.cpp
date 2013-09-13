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
        auto stc = new wxStyledTextCtrl(m_frame, wxID_ANY);
        stc->SetLexer(5000);
        
        // set some basic styles
        stc->StyleSetForeground(wxSTC_B_DEFAULT, wxColour("black"));
        stc->StyleSetFontAttr(wxSTC_B_DEFAULT, 16, "Monaco", false, false, false);
        stc->PrivateLexerCall(0, nullptr);
        
        // show
        m_frame->Show();
        
        // all ok
        return true;
    }
    
    // main window
    wxFrame * m_frame;
};

IMPLEMENT_APP(FBIdeApp)
