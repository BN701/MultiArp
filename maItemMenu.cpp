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
std::list<ItemMenu*> ItemMenu::m_RedrawList;

//bool ItemMenu::m_RedrawMenuList = false;
int ItemMenu::m_ObjectCount = 0;

menu_list_cursor_t MenuList::Add(ItemMenu * item, bool select)
{
    m_Items.push_back(item);
    menu_list_cursor_t newItemPos = --m_Items.end();
    item->SetMenuList(this, newItemPos);
    item->SetMenuListRow(m_Items.size() - 1);
    if ( select )
        Select(newItemPos);
    return newItemPos;
}

menu_list_cursor_t MenuList::Insert(menu_list_cursor_t pos, ItemMenu * item, bool select)
{
    int row;
    if ( pos != m_Items.end() )
        row = (*pos)->MenuListRow();
    else
        row = m_Items.size();
    menu_list_cursor_t newItemPos = m_Items.insert(pos, item);
    item->SetMenuList(this, newItemPos);
    if ( select )
        Select(newItemPos);
    for ( auto it = newItemPos; it != m_Items.end(); it++ )
        (*it)->SetMenuListRow(row++);
    return newItemPos;
}

menu_list_cursor_t MenuList::InsertAfter(menu_list_cursor_t pos, ItemMenu * item, bool select)
{
    int row = (*pos)->MenuListRow();
    menu_list_cursor_t newItemPos = m_Items.insert(++pos, item);
    item->SetMenuList(this, newItemPos);
    if ( select )
        Select(newItemPos);
    for ( auto it = newItemPos; it != m_Items.end(); it++ )
        (*it)->SetMenuListRow(++row);
    return newItemPos;
}

menu_list_cursor_t MenuList::Remove(menu_list_cursor_t pos)
{
    int row = (*pos)->MenuListRow();
    (*pos)->ClearMenuList();
    bool reselect = m_Cursor == pos;
    menu_list_cursor_t result = m_Items.erase(pos);
    for ( auto it = result; it != m_Items.end(); it++ )
        (*it)->SetMenuListRow(row++);
    result--;
    if ( reselect )
        Select(result);
    return result;
}

menu_list_cursor_t MenuList::Select(menu_list_cursor_t pos)
{
    if ( m_Cursor != m_Items.end() )
        (*m_Cursor)->SetRedraw();
    m_Cursor = pos;
    (*m_Cursor)->SetRedraw();
    m_SelectionChanged = true;
}

bool MenuList::UpCursorPos()
{
    if ( m_Cursor == --m_Items.end() )
        return false;

    (*m_Cursor)->SetRedraw();
    m_Cursor++;
    (*m_Cursor)->SetRedraw();

    m_SelectionChanged = true;
    return true;
}

bool MenuList::DownCursorPos()
{
    if ( m_Cursor == m_Items.begin() )
        return false;

    (*m_Cursor)->SetRedraw();
    m_Cursor--;
    (*m_Cursor)->SetRedraw();

    m_SelectionChanged = true;
    return true;
}


menu_list_cursor_t MenuList::FindFirstNonMatching(int type)
{
    menu_list_cursor_t result = m_Cursor;
    do {
        result++;
    } while ( result != m_Items.end() && (*result)->CheckType(type) );
    return result;
}

void MenuList::OpenCurrentItem()
{
    if ( !m_Items.empty() )
    {
        ItemMenu & menu = **m_Cursor;
        menu.SetFocus();
    }
}

void MenuList::OpenCurrentItem(ItemMenu * returnFocus)
{
    if ( !m_Items.empty() )
    {
        ItemMenu & menu = **m_Cursor;
        menu.SetFocus();
        menu.SetReturnFocus(returnFocus);
    }
}

bool MenuList::GetDisplayInfo(BaseUI & display, MenuListDisplayInfo * & displayInfo)    // Non-static
{
    return display.GetDisplayInfo(m_DisplayObjectType, displayInfo);
}

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

void ItemMenu::InitStatus()
{
    m_Status.clear();

    if ( m_MenuList != NULL )
    {
        if ( /* m_MenuList->m_Container->m_GotFocus && */ m_MenuList->m_Cursor == m_MenuPos )
            m_Status = " -> ";
        m_Status.resize(m_MenuListIndent + 4, ' ');
    }
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

bool ItemMenu::GetDisplayInfo(BaseUI & display, int & row, int & col, int & width, Rectangle & clearArea)    // Non-static
{
    // Return true if visible.

    MenuListDisplayInfo * displayInfo = NULL;    // This will be set by child class overrides if it needs clearing.
    row = 0;
    col = 0;
    width = 80; // Just a sensible looking default width.

    if ( m_MenuList != NULL )
    {
        if ( !m_MenuList->GetDisplayInfo(display, displayInfo) )
            return false;

        // Make sure selected item is within scrolling window.

        if ( m_MenuList->m_SelectionChanged )
        {
            int rowSelection = (*m_MenuList->m_Cursor)->m_MenuListRow;

            if ( rowSelection < displayInfo->m_ScrollStart )
            {
                displayInfo->m_ScrollStart = rowSelection;
                m_MenuList->m_Container->SetRedraw();
            }

            if ( rowSelection >= displayInfo->m_ScrollStart + displayInfo->m_ScrollHeight )
            {
                displayInfo->m_ScrollStart = rowSelection - displayInfo->m_ScrollHeight + 1;
                m_MenuList->m_Container->SetRedraw();
            }

            m_MenuList->m_SelectionChanged = false;
        }

        // Check the current item.

        row = m_MenuListRow - displayInfo->m_ScrollStart;

        if ( row < 0 )
            return false;

        if ( row >= displayInfo->m_ScrollHeight )
            return false;

        // Check if we need to clear space at the bottom of the display area.

        int listSize = m_MenuList->m_Items.size();

        if ( listSize < displayInfo->m_PreviousListSize )
        {
            int clearHeight = displayInfo->m_ScrollHeight - (listSize - displayInfo->m_ScrollStart);
            if ( clearHeight > 0 )
            {
                clearArea = *displayInfo;
                clearArea.m_iY += clearArea.m_iHeight - clearHeight;
                clearArea.m_iHeight = clearHeight;
            }
        }

        displayInfo->m_PreviousListSize = listSize;
    }
    else
        display.GetDisplayInfo(m_DisplayObjectType, displayInfo);

    if ( displayInfo != NULL )
    {
        col += displayInfo->m_iX;
        row += displayInfo->m_iY;
        width = displayInfo->m_iWidth;
        return true;
    }
    else
        return false;
}

