//
//    MultiArp - Another step in the Great Midi Adventure!
//    Copyright (C) 2017  Barry Neilsen
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

// This is probably bad practice, but classes will derive from this
// to handle cursor keys and although the entry mode field will be
// common to all of them, its content won't be. I could embed the enum
// in the class, but that means the base class will need to know about
// what derives from it, which is even more wrong.

enum entry_modes_t
{
    em_normal,
    em_scale,
    em_feel,
    number_entry_modes
};

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
        virtual ~CursorKeys();

        enum key_type_t {
            enter,
            back_space,
            up,
            down,
            left,
            right,
            shift_page_up,
            shift_page_down,
            alt_page_up,
            alt_page_down,
            ctrl_down,
            ctrl_up,
            ctrl_left,
            ctrl_right,
            shift_down,
            shift_up,
            shift_left,
            shift_right,
            ctrl_shift_left,
            ctrl_shift_right,
            ctrl_shift_up,
            ctrl_shift_down
        };

        enum follow_up_action_t {
            none,
            update_pattern_browser,
            number_follow_up_actions
        };

        virtual void SetMode(entry_modes_t m) { m_EntryMode = m; }
        entry_modes_t Mode() { return m_EntryMode; }

        virtual void SetReturnFocus( CursorKeys * val ) { m_ReturnFocus = val; }
        virtual void SetStatus() {}
        virtual void SetFocus() { m_Focus = & (*this); }
        void InitFocus() { m_Focus = NULL; }
        bool RouteKey(key_type_t k) { return m_Focus->HandleKey(k); }
        std::string & Status() { return m_Focus->m_Status; }

        bool FirstField() { return m_Focus->m_FirstField; }
        std::vector<screen_pos_t> & GetHighlights() { return m_Focus->m_Highlights; }

        follow_up_action_t FollowUp()
        {
            follow_up_action_t t = m_FollowUp;
            m_FollowUp = none;
            return t;
        }

    protected:
        virtual bool HandleKey(key_type_t k) { return false; };

        entry_modes_t m_EntryMode;
        std::string m_Status;
        std::vector<screen_pos_t> m_FieldPositions; // Offset/length.
        std::vector<screen_pos_t> m_Highlights; // Offset/length.

        bool m_FirstField = true;
        static CursorKeys * m_Focus;

        CursorKeys * m_ReturnFocus = NULL;
        follow_up_action_t m_FollowUp = none;

    private:
};

#endif // CURSORKEYS_H
