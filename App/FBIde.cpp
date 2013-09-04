/**
 * FBIde main application
 */
#include "app_pch.h"
#include "Sdk/Manager.h"
#include "Sdk/UiManager.h"


using namespace fbi;

/**
 * Main application class
 */
class FBIdeApp : public wxApp
{
    
    // Entry point
    virtual bool OnInit () {
        // base init
        if (!wxApp::OnInit()) return false;

		GET_UIMGR()->GetFrame()->Show();

		// done
		return true;
    }

};
IMPLEMENT_APP(FBIdeApp);
