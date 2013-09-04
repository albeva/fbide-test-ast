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
#pragma once

namespace fbi
{

    /**
    * Declare non copyable class
    */
    struct SDK_DLL NonCopyable
    {
    protected:
        NonCopyable() {}
        ~NonCopyable() {}

    private:  // emphasize the following members are private
        NonCopyable(const NonCopyable&);
        const NonCopyable& operator=(const NonCopyable&) ;
    };


    /**
    * Template to declare singleton classes
    */
    template <class T> class Singleton : public NonCopyable
    {
    private:
        static T * m_instance;

    protected:
        // private constructor
        Singleton()
        {
            assert(Singleton<T>::m_instance == nullptr);
        }

        // private destructor
        virtual ~Singleton()
        {
            Singleton<T>::m_instance = nullptr;
        }


    public:

        /**
         * Return true if Singleton instance exists
         */
        static inline bool HasInstance() { return m_instance != nullptr; }


        /**
         * Get singleton class instance. Create if doesn't exist
         */
        static inline T * GetInstance()
        {
            if (m_instance == nullptr) m_instance = new T();
            return m_instance;
        }


        /**
         * Release existing singleton instance
         */
        static inline void Release()
        {
            if (m_instance == nullptr) return;
            delete m_instance;
            m_instance = nullptr;
        }
    };
    template<class T>T * Singleton<T>::m_instance = nullptr;

}
