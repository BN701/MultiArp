//
//    MultiArp - Another step in the Great Midi Adventure!
//    Copyright (C) 2017, 2018  Barry Neilsen
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////////


#include "maCursorKeys.h"

CursorKeys * CursorKeys::m_Focus = NULL;
bool CursorKeys::m_RedrawDisplay = false;
int CursorKeys::m_ObjectCount = 0;

CursorKeys::CursorKeys()
{
#if defined(MA_BLUE) && !defined(MA_BLUE_PC)
    if ( SP_CURSORKEYS )
        EepromStore(SP_CURSORKEYS);
#endif
    //ctor
}

CursorKeys::CursorKeys(const CursorKeys & val)
{
    // Explicitly avoid copying any pointers to other menus,
    // nothing else to be done. (Members appear to be initialized
    // according to their declarations. i.e. set to NULL, etc.)
}

CursorKeys::~CursorKeys()
{
    //dtor

    if ( m_Focus == this )
        m_Focus = NULL;
}

bool CursorKeys::MenuActive()
{
    return m_Focus != NULL;
}

//bool CursorKeys::RouteKey(key_type_t k)
bool CursorKeys::RouteKey(BaseUI::key_command_t k)
{
    if ( m_Focus != NULL )
        return m_Focus->HandleKey(k);
    else
        return false;
}

std::string g_EmptyStatus;

std::string & CursorKeys::Status()
{
    if ( m_Focus != NULL )
        return m_Focus->m_Status;
    else
        return g_EmptyStatus;
}

std::string & CursorKeys::Help()
{
    if ( m_Focus != NULL )
        return m_Focus->m_Help;
    else
        return g_EmptyStatus;
}

bool CursorKeys::FirstField()
{
    if ( m_Focus != NULL )
        return m_Focus->m_FirstField;
    else
        return false;
}

// Default empty containers.

std::vector<screen_pos_t> g_FieldPositions;
std::vector<screen_pos_t> g_Highlights;

std::vector<screen_pos_t> & CursorKeys::GetHighlights()
{
    if ( m_Focus != NULL )
        return m_Focus->m_Highlights;
    else
        return g_Highlights;

    // Convoluted behaviour, I know, but assuming this method is (usually)
    // called on the default global object used for routing to actual
    // instances, the global object's array will always exist and always
    // be empty.
}

std::vector<screen_pos_t> & CursorKeys::GetFieldPositions()
{
    if ( m_Focus != NULL )
        return m_Focus->m_FieldPositions;
    else
        return g_FieldPositions;
}
