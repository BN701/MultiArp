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
ItemMenu * ItemMenu::m_DefaultFocus = NULL;
std::list<ItemMenu*> ItemMenu::m_RedrawList;

//bool ItemMenu::m_RedrawMenuList = false;
//int ItemMenu::m_ObjectCount = 0;

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

menu_list_cursor_t MenuList::Remove(menu_list_cursor_t pos, bool setCursorToEnd)
{
    int row = (*pos)->MenuListRow();
    (*pos)->ClearMenuList();

//    // If we're taking out the current selection,
//    // we have to select something else.

//    bool reselect = m_Cursor == pos;

    // Prevent removal of current selection. This saves agonising
    // over what to reselect and shifts that responsibility upstream :)

    if ( m_Cursor == pos )
        if ( setCursorToEnd )
            m_Cursor = m_Items.end();
        else
            throw "MenuList::Remove(), should't try to remove current selection.";

    menu_list_cursor_t result = m_Items.erase(pos);

    // If we've removed the last item in the list, nothing more to do?

//    if ( m_Items.empty() )
    if ( result == m_Items.end() )
    {
//        m_Cursor = result;
        return result;
    }

    // Renumber items further along the list.

    for ( auto it = result; it != m_Items.end(); it++ )
        (*it)->SetMenuListRow(row++);

//    if ( result != m_Items.begin() )
//        result--;

//    if ( reselect )
//    {
//        m_Cursor = m_Items.end();   // Prevent Select() from doing anything to the item we've
//                                    // just removed.
//        Select(result/*, setReturnFocus*/);
//    }
//
    return result;
}

menu_list_cursor_t MenuList::Select(menu_list_cursor_t pos/*, bool setReturnFocus*/)
{
    if ( m_Cursor != m_Items.end() )
        (*m_Cursor)->SetRedraw();

    menu_list_cursor_t previous = m_Cursor;
    m_Cursor = pos;

    (*m_Cursor)->SetRedraw();
    m_SelectionChanged = true;

    (*m_Cursor)->SetFocus();

//    if ( setReturnFocus )
//        (*m_Cursor)->SetReturnFocus(m_Container->m_ReturnFocus);

    return previous;
}

bool MenuList::UpCursorPos()
{
    menu_list_cursor_t pos = m_Cursor;
    pos++;
    if ( pos != m_Items.end() )
        Select(pos);
    return m_SelectionChanged;
}

bool MenuList::DownCursorPos()
{
    if ( m_Cursor == m_Items.begin() )
        return false;
    menu_list_cursor_t pos = m_Cursor;
    Select(--pos);
    return m_SelectionChanged;
}


menu_list_cursor_t MenuList::FindFirstNonMatching(int type)
{
    menu_list_cursor_t result = m_Cursor;
    while (true)
    {
        result++;
        if ( result == m_Items.end() )
            break;
        if ( !(*result)->CheckType(type) )
            break;
    }
    return result;
}

menu_list_cursor_t MenuList::ReverseFind(int type)
{
    menu_list_cursor_t result = m_Cursor;
    while (true)
    {
        result--;
        if ( (*result)->CheckType(type) )
            break;
        if ( result == m_Items.begin() )
        {
            result = m_Items.end();
            break;
        }
    }
    return result;
}

ItemMenu * MenuList::CurrentItem()
{
    if ( m_Cursor == m_Items.end() )
        return NULL;
    else
        return *m_Cursor;
}

//void MenuList::OpenCurrentItem()
//{
//    if ( !m_Items.empty() )
//    {
//        ItemMenu & menu = **m_Cursor;
//        menu.SetFocus();
//    }
//}
//
//void MenuList::OpenCurrentItem(ItemMenu * returnFocus)
//{
//    if ( !m_Items.empty() )
//    {
//        ItemMenu & menu = **m_Cursor;
//        menu.SetFocus();
//        menu.SetReturnFocus(returnFocus);
//    }
//}
//
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

//    m_TestString = "Set from ItemMenu constructor body.";
}

ItemMenu::ItemMenu(const ItemMenu & m):
    m_GotFocus(m.m_GotFocus),
    m_Visible(m.m_Visible),
    m_MenuListRow(m.m_MenuListRow),
    m_MenuListIndent(m.m_MenuListIndent),
    m_PopUpMenuID(m.m_PopUpMenuID),
    m_FirstField(m.m_FirstField),
    m_FollowUp(m.m_FollowUp),
    m_ReturnFocus(m.m_ReturnFocus),
    m_DisplayObjectType(m.m_DisplayObjectType),
    m_Status(m.m_Status),
    m_Help(m.m_Help),
    m_ItemID(m.m_ItemID),
    m_MenuList(m.m_MenuList),
    m_PosInMenuList(m.m_PosInMenuList)
{
//    m_TestString = "Set from ItemMenu copy constructor body.";

    // Doing these is fine if we're being copied as part
    // of a reallocation, as the original will be destroyed.

    // If making a real copy, remove focus and remove from menu beforehand.

    if ( m_GotFocus )
    {
        SetFocus();
    }

    if ( m_MenuList != NULL )
    {
        *m_PosInMenuList = this;
    }
}

ItemMenu::~ItemMenu()
{
    //dtor

    // Calling ReturnFocus() from here is *not* the preferred way of
    // closing things down. Objects persist regardless of whether their
    // menus are visible or have input focus. It's probably still a
    // useful thing to do, however.

    if ( m_Focus == this )
    {
        ReturnFocus();
    }

    if ( m_MenuList != NULL && *m_PosInMenuList == this)
        m_MenuList->Remove(m_PosInMenuList, true); // Allow removal even if selected.

    m_RedrawList.remove(this);
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
        if ( /* m_MenuList->m_Container->m_GotFocus && */ m_MenuList->m_Cursor == m_PosInMenuList )
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

        if ( m_MenuList->m_SelectionChanged && m_MenuList->m_Cursor != m_MenuList->m_Items.end() )
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
    {
        display.GetDisplayInfo(m_DisplayObjectType, displayInfo);
        if ( displayInfo == NULL )
            return false;

        // If display area more than one line in height, set an area to be cleared.
        if ( displayInfo->m_iHeight > 1 )
        {
            clearArea = *displayInfo;
            clearArea.m_iY += 1;
            clearArea.m_iHeight -= 1;
        }
    }


    col += displayInfo->m_iX;
    row += displayInfo->m_iY;
    width = displayInfo->m_iWidth;

    return true;
}

