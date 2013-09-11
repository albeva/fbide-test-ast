//
//  main.cpp
//  fbide
//
//  Created by Albert on 11/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//


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
        // create the window
        m_frame = new wxFrame(nullptr, wxID_ANY, "FBIde");
        m_frame->Show();
        
        // all ok
        return true;
    }
    
    // main window
    wxFrame * m_frame;
};

IMPLEMENT_APP(FBIdeApp)
