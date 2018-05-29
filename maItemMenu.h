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

#ifndef CURSORKEYS_H
#define CURSORKEYS_H

#include <list>
#include <string>
#include <vector>

#include "maBaseUI.h"
#include "maCommandMenu.h"

//struct screen_pos_t
//{
//    int offset;
//    int length;
//    screen_pos_t(int ofs = 0, int len = 0) { offset = ofs; length = len; }
//};

class ItemMenu;

typedef std::list<ItemMenu *> menu_list_t;
typedef std::list<ItemMenu *>::iterator menu_list_cursor_t;

class MenuList
{
    public:
        menu_list_t m_Items;
        menu_list_cursor_t m_Cursor = m_Items.end();
        BaseUI::display_object_type_t m_DisplayObjectType = BaseUI::dot_base;

        menu_list_cursor_t Add(ItemMenu * item, bool select = false);    // Wraps push_back().
        menu_list_cursor_t Insert(menu_list_cursor_t pos, ItemMenu * item, bool select = false);
        menu_list_cursor_t InsertAfter(menu_list_cursor_t pos, ItemMenu * item, bool select = false);
        menu_list_cursor_t Remove(menu_list_cursor_t pos, bool setCursorToEnd = false);
        menu_list_cursor_t Select(menu_list_cursor_t pos/*, bool setReturnFocus = true*/);
        menu_list_cursor_t Select(ItemMenu * item);
        ItemMenu* ClearCursor();

//        void OpenCurrentItem();
//        void OpenCurrentItem(ItemMenu * returnFocus);
        bool GetDisplayInfo(BaseUI & display, MenuListDisplayInfo * & displayInfo);

        ItemMenu * CurrentItem();
        menu_list_cursor_t FindFirstNonMatching(int type);
        menu_list_cursor_t ReverseFind(int type);

        MenuList(ItemMenu * container, bool * vis):
            m_Visible(vis),
            m_Container(container)
        {}

        ~MenuList()
        {}

        bool DownCursorPos();
        bool UpCursorPos();

        bool m_SelectionChanged = false;
        bool * m_Visible = NULL;
        ItemMenu * m_Container = NULL;

        int m_LastCursorPos = -1;

};

class ItemMenu
{
    friend class MenuList;
    public:
        ItemMenu();
        ItemMenu(const ItemMenu & val);
        ItemMenu & operator=(const ItemMenu &); // Needed because we have a move constructor.
        ItemMenu(ItemMenu && val) noexcept;
        virtual ~ItemMenu();

//        enum follow_up_action_t
//        {
//            none,
//            update_pattern_browser,
//            num_follow_up_actions
//        };

        int ItemID() { return m_ItemID; }
        void SetItemID( int val ) { m_ItemID = val; }

        virtual void SetReturnFocus( ItemMenu * val ) { m_ReturnFocus = val; }
        void ReturnFocus()
        {
            if ( m_ReturnFocus != NULL )
            {
                m_ReturnFocus->SetFocus();
                m_ReturnFocus = NULL;
            }
            else if ( m_DefaultFocus != NULL )
                m_DefaultFocus->SetFocus();

        }

        // These two should be overriden, but don't make them pure as we
        // need to declare an instance of *this* class to route keystrokes
        // to the active menu instance.

        virtual void SetStatus() {};
        void ClearStatus() { m_Status.clear(); }
        virtual bool HandleKey(BaseUI::key_command_t k) { return false; };

        virtual void SetFocus() noexcept
        {
            if ( m_Focus != NULL )
            {
                m_Focus->SetRedraw();
                m_Focus->m_GotFocus = false;
            }
            m_Focus = & (*this);
            SetRedraw();
            m_GotFocus = true;
        }

        bool HasFocus() { return m_GotFocus; }

        static void SetDefaultFocus() { m_DefaultFocus = m_Focus; }

        std::string & StatusString() { return m_Status; }

        static bool MenuActive();
        static bool RouteKey(BaseUI::key_command_t k);
        static std::string & Status(bool setStatus = true);
        static std::string & Help();
        static bool FirstField();
        static std::vector<screen_pos_t> & GetHighlights();
        static std::vector<screen_pos_t> & GetFieldPositions();
        static bool DisplayPos(int & row, int & col);

        std::vector<screen_pos_t> & ItemHighlights() { return m_Highlights; }

//        follow_up_action_t FollowUp()
//        {
//            follow_up_action_t t = m_FollowUp;
//            m_FollowUp = none;
//            return t;
//        }

        command_menu_id_t PopUpMenuID()
        {
            return m_PopUpMenuID;
        }

        bool CheckType(int type) { return m_DisplayObjectType == type; }
        void SetType(BaseUI::display_object_type_t type) { m_DisplayObjectType = type; }
        BaseUI::display_object_type_t Type() { return m_DisplayObjectType; }

        void SetDisplayIndent(int val) { m_MenuListIndent = val; }
        int DisplayIndent() { return m_MenuListIndent; }

        void SetMenuListRow(int row)
        {
            m_MenuListRow = row;
            SetRedraw();
        }
        int MenuListRow() { return m_MenuListRow; }

        void SetMenuList(MenuList * list, menu_list_cursor_t pos)
        {
            m_MenuListPtr = list;
            m_PosInMenuList = pos;
        }

        void ClearMenuList() { m_MenuListPtr = NULL; }

        bool GetDisplayInfo(BaseUI & display, int & row, int & col, int & width, Rectangle & clearArea);

        static void ClearRedrawList() { m_RedrawList.clear(); }
        static std::list<ItemMenu *> & RedrawList() { return m_RedrawList; }

        virtual void SetRedraw() noexcept
        {
            if ( m_Visible )
            {
                // There's no 'find' function to check that we're not
                // adding a duplicate, so may as well attempt a 'remove'
                // to make sure.
                m_RedrawList.remove(this);
                m_RedrawList.push_back(this);
            }
        }

        virtual void SetVisible(bool val) { m_Visible = val; }
        void SetMenuPos(menu_list_cursor_t pos) { m_PosInMenuList = pos; }
        menu_list_cursor_t MenuPos() { return m_PosInMenuList; }
//        std::string m_TestString = "Set from ItemMenu class definition.";

    protected:
        static ItemMenu * m_Focus;
        static ItemMenu * m_DefaultFocus;   // Use this for return focus if nothing else has been set.
        static std::list<ItemMenu*> m_RedrawList;

        bool m_GotFocus = false;
        bool m_Visible = false;
        int m_MenuListRow = 0;
        int m_MenuListIndent = 0;
        command_menu_id_t m_PopUpMenuID = C_MENU_ID_NONE;
        bool m_FirstField = true;
//        follow_up_action_t m_FollowUp = none;
        ItemMenu * m_ReturnFocus = NULL;

        BaseUI::display_object_type_t m_DisplayObjectType = BaseUI::dot_base;

        std::string m_Status = "Not set";
        std::string m_Help;

        int m_ItemID = -1;

        MenuList * m_MenuListPtr = NULL;
        menu_list_cursor_t m_PosInMenuList; // This is an iterator to a list, so I think I read it's safe to store.

        std::vector<screen_pos_t> m_FieldPositions; // Offset/length.
        std::vector<screen_pos_t> m_Highlights; // Offset/length.

//        void MenuInsert(menu_list_cursor_t pos, ItemMenu * item)
//        {
//            if ( m_MenuList != NULL )
//            {
//                m_MenuList->InsertAfter(pos, item, true);
//            }
//        }

        void InitStatus();

    private:

//        int m_ObjectID = m_ObjectCount++;

//        static bool m_RedrawMenuList;
//        static int m_ObjectCount;
};

#endif // CURSORKEYS_H
