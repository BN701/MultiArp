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

#include <string>
#include <vector>

#include "maBaseUI.h"
struct screen_pos_t
{
    int offset;
    int length;
    screen_pos_t(int ofs = 0, int len = 0) { offset = ofs; length = len; }
};

class CursorKeys
{
    public:
        CursorKeys();
        CursorKeys(const CursorKeys & val);
        virtual ~CursorKeys();

        enum follow_up_action_t
        {
            none,
            update_pattern_browser,
            num_follow_up_actions
        };

        int ItemID() { return m_ItemID; }
        void SetItemID( int val ) { m_ItemID = val; }

        virtual void SetReturnFocus( CursorKeys * val ) { m_ReturnFocus = val; }
        void ReturnFocus()
        {
            if ( m_ReturnFocus != NULL )
            {
                m_ReturnFocus->SetFocus();
                m_ReturnFocus->SetStatus();
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

        static bool MenuActive();
//        static bool RouteKey(key_type_t k);
        static bool RouteKey(BaseUI::key_command_t k);
        static std::string & Status();
        static std::string & Help();
        static bool FirstField();
        static std::vector<screen_pos_t> & GetHighlights();
        static std::vector<screen_pos_t> & GetFieldPositions();

        follow_up_action_t FollowUp()
        {
            follow_up_action_t t = m_FollowUp;
            m_FollowUp = none;
            return t;
        }

    protected:
        static CursorKeys * m_Focus;

//        virtual bool HandleKey(key_type_t k) { return false; };

        std::string m_Status;
        std::string m_Help;
        std::vector<screen_pos_t> m_FieldPositions; // Offset/length.
        std::vector<screen_pos_t> m_Highlights; // Offset/length.

        bool m_FirstField = true;
        follow_up_action_t m_FollowUp = none;

        CursorKeys * m_ReturnFocus = NULL;

        int m_ItemID = -1;

    private:
};

#endif // CURSORKEYS_H
