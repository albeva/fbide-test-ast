/*
 * This file is part of FBIde project
 *
 * FBIde is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FBIde is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FBIde.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Albert Varaksin <albeva@me.com>
 * Copyright (C) The FBIde development team
 */
#include "sdk_pch.h"
#include "Manager.h"
#include "UiManager.h"


using namespace fbi;

// decouple the tab
const int ID_DecoupleTab = ::wxNewId();

/**
 * Manager class implementation
 */
struct TheUiManager : UiManager, wxEvtHandler
{
    // create
    TheUiManager () : m_frame(nullptr)
    {
    }


    // destroy
    ~TheUiManager ()
    {}
    

    // Get application title
    // - maybe use later a title template
    //   to allow customing?
    virtual wxString GetTitle ()
    {
        return wxString("FBIde ") + GET_MGR()->GetVersion().string;
    }


    // get the main frame
    virtual wxFrame * GetFrame ()
	{
		if (m_frame == nullptr) {
			m_frame = new wxFrame(nullptr, wxID_ANY, GetTitle());
		}
		return m_frame;
	}


    // Handle close event
    void OnClose (wxCloseEvent & event)
    {
		if (m_frame) {
			m_frame->RemoveEventHandler(this);
			m_frame->Destroy();
		}
    }


    // handle command events
    void OnCommandEvent (wxCommandEvent & event)
    {
        // sllow others to catch
        event.Skip();
    }



    // vars
    wxFrame *           m_frame;                    // the main application frame
 
    // route events
    DECLARE_EVENT_TABLE();
};


// event dispatching
BEGIN_EVENT_TABLE(TheUiManager, wxEvtHandler)
    EVT_CLOSE   (                           TheUiManager::OnClose)
    EVT_MENU    (wxID_ANY,                  TheUiManager::OnCommandEvent)
END_EVENT_TABLE()


// Implement Manager
IMPLEMENT_MANAGER(UiManager, TheUiManager)
