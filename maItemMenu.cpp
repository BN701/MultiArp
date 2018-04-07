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


#include "maItemMenu.h"

ItemMenu * ItemMenu::m_Focus = NULL;
bool ItemMenu::m_RedrawMenuList = false;
int ItemMenu::m_ObjectCount = 0;

ItemMenu::ItemMenu()
{
#if defined(MA_BLUE) && !defined(MA_BLUE_PC)
    if ( SP_CURSORKEYS )
        EepromStore(SP_CURSORKEYS);
#endif
    //ctor
}

ItemMenu::ItemMenu(const ItemMenu & val)
{
    // Explicitly avoid copying any pointers to other menus,
    // nothing else to be done. (Members appear to be initialized
    // according to their declarations. i.e. set to NULL, etc.)
}

ItemMenu::~ItemMenu()
{
    //dtor

    if ( m_Focus == this )
        m_Focus = NULL;
}

bool ItemMenu::MenuActive()
{
    return m_Focus != NULL;
}

bool ItemMenu::RouteKey(BaseUI::key_command_t k)
{
    if ( m_Focus != NULL )
        return m_Focus->HandleKey(k);
    else
        return false;
}

std::string g_EmptyStatus;

std::string & ItemMenu::Status(bool setStatus)
{
    if ( m_Focus != NULL )
    {
        if ( setStatus )
            m_Focus->SetStatus();
        return m_Focus->m_Status;
    }
    else
        return g_EmptyStatus;
}

std::string & ItemMenu::Help()
{
    if ( m_Focus != NULL )
        return m_Focus->m_Help;
    else
        return g_EmptyStatus;
}

bool ItemMenu::FirstField()
{
    if ( m_Focus != NULL )
        return m_Focus->m_FirstField;
    else
        return false;
}

// Default empty containers.

std::vector<screen_pos_t> g_FieldPositions;
std::vector<screen_pos_t> g_Highlights;

std::vector<screen_pos_t> & ItemMenu::GetHighlights()
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

std::vector<screen_pos_t> & ItemMenu::GetFieldPositions()
{
    if ( m_Focus != NULL )
        return m_Focus->m_FieldPositions;
    else
        return g_FieldPositions;
}

bool ItemMenu::DisplayPos(int & row, int & col)
{
    if ( m_Focus != NULL )
        return m_Focus->GetDisplayPos(row, col);
    else
        return false;
}
