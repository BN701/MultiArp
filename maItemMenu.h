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

class ItemMenu
{
    public:
        ItemMenu();
        ItemMenu(const ItemMenu & val);
        virtual ~ItemMenu();

        enum follow_up_action_t
        {
            none,
            update_pattern_browser,
            num_follow_up_actions
        };

        enum display_object_type_t  // Hopefully just use these for sanity checks.
        {
            dot_base,
            dot_pattern,
            dot_step_list_group,
            dot_rt_list_group,
            dot_step_list,
            dot_rt_list
        };

        int ItemID() { return m_ItemID; }
        void SetItemID( int val ) { m_ItemID = val; }

        virtual void SetReturnFocus( ItemMenu * val ) { m_ReturnFocus = val; }
        void ReturnFocus()
        {
            if ( m_ReturnFocus != NULL )
            {
                m_ReturnFocus->SetFocus();
//                m_ReturnFocus->SetStatus();
            }
            m_ReturnFocus = NULL;
        }

        // These two should be overriden, but don't make them pure as we
        // need to declare an instance of *this* class to route keystrokes
        // to the active menu instance.

        virtual void SetStatus() {}
//        virtual void SetStatus() = 0;
        virtual bool HandleKey(BaseUI::key_command_t k) { return false; };
//        virtual bool HandleKey(BaseUI::key_command_t k) = 0;

        virtual void SetFocus() { m_Focus = & (*this); }
        void InitFocus() { m_Focus = NULL; }

        std::string & StatusString() { return m_Status; }

        static bool MenuActive();
        static bool RouteKey(BaseUI::key_command_t k);
        static std::string & Status(bool setStatus = true);
        static std::string & Help();
        static bool FirstField();
        static std::vector<screen_pos_t> & GetHighlights();
        static std::vector<screen_pos_t> & GetFieldPositions();
        static bool DisplayPos(int & row, int & col);

        follow_up_action_t FollowUp()
        {
            follow_up_action_t t = m_FollowUp;
            m_FollowUp = none;
            return t;
        }

        command_menu_id_t PopUpMenuID()
        {
            return m_PopUpMenuID;
        }

        bool CheckType(display_object_type_t type) { return m_DisplayObjectType == type; }

        void SetDisplayPos(int row, int col)
        {
            m_DisplayRow = row;
            m_DisplayCol = col;
        }

        void SetDisplayRow(int row) { m_DisplayRow = row; }

        int DisplayCol() { return m_DisplayCol; }

        static void SetRedrawMenuList() { m_RedrawMenuList = true; }
        static bool RedrawMenuList()
        {
            if ( m_RedrawMenuList )
            {
                m_RedrawMenuList = false;
                return true;
            }
            else
                return false;
        }

        void SetMenuListInfo(menu_list_t * menu, menu_list_cursor_t cursor)
        {
            m_MenuList = menu;
            m_MenuPos = cursor;
        }

        menu_list_cursor_t MenuInsert(menu_list_t * menu, menu_list_cursor_t pos)
        {
            // This assumes that any one instance of ItemMenu can only
            // be a member of one menu list. (There are many menu lists, however.
            // At the moment there's one for each Pattern.)

            m_MenuList = menu;
            m_MenuPos = m_MenuList->insert(++pos, this);
            m_RedrawMenuList = true;
            return m_MenuPos;
        }

    protected:
        static ItemMenu * m_Focus;

//        virtual bool HandleKey(key_type_t k) { return false; };

        bool GetDisplayPos(int & row, int & col)    // Non-static
        {
            if ( m_DisplayRow > -1 && m_DisplayCol > -1 )
            {
                row = m_DisplayRow;
                col = m_DisplayCol;
                return true;
            }
            else
                return false;
        }

        int m_DisplayRow = -1;
        int m_DisplayCol = -1;

        std::string m_Status = "Not set";
        std::string m_Help;
        std::vector<screen_pos_t> m_FieldPositions; // Offset/length.
        std::vector<screen_pos_t> m_Highlights; // Offset/length.

        bool m_FirstField = true;
        follow_up_action_t m_FollowUp = none;

        ItemMenu * m_ReturnFocus = NULL;

        int m_ItemID = -1;

        command_menu_id_t m_PopUpMenuID = C_MENU_ID_NONE;
        display_object_type_t m_DisplayObjectType = dot_base;

        menu_list_t * m_MenuList;
        menu_list_cursor_t m_MenuPos;

    private:

        int m_ObjectID = m_ObjectCount++;

        static bool m_RedrawMenuList;
        static int m_ObjectCount;
};

#endif // CURSORKEYS_H
