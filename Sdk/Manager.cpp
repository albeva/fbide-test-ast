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


/**
 * Manager class implementation
 */
struct TheManager : Manager
{
    // create
    TheManager()
    {
    };


    // destroy
    ~TheManager()
    {
        ReleaseManagers();
    }


    // get version information
    virtual const Version & GetVersion()
    {
        // the static version object
        static Version v =
        {
            SDK_VERSION_MAJOR,
            SDK_VERSION_MINOR,
            SDK_VERSION_RELEASE,
            SDK_VERSION_STRING,
            __TIMESTAMP__
        };
        // return const ref
        return v;
    }
};


// Get Ui manager instance
UiManager * Manager::GetUiManager()
{
    return UiManager::GetInstance();
}



/**
 * Release all managers. The order is important
 * And probably needs some adjustment later on
 *
 * Logically thinking plugin and scripting should go down
 * first, followed by various content managers and lastly
 * the ui
 */
void Manager::ReleaseManagers()
{
    UiManager::Release();
}


// implement manager
IMPLEMENT_MANAGER( Manager, TheManager )
