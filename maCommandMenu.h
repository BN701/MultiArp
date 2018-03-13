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

#ifndef MACOMMANDMENU_H_INCLUDED
#define MACOMMANDMENU_H_INCLUDED

#include <map>
#include <stack>
#include <string>

#ifdef MA_BLUE

//typedef uint32_t xcb_keysym_t;

#else

//#include <xcb/xcb.h>
//
//// Todo: move X key code dependency and set up our own codes.
//
//#define XK_MISCELLANY
//#define XK_XKB_KEYS
//#define XK_LATIN1
//#include <X11/keysymdef.h>  // XK_ Unicode key name defines
//

#endif // MA_BLUE

#include "maCursorKeys.h"
#include "maBaseUI.h"

enum command_menu_id_t
{
    C_MENU_ID_NONE,
    C_MENU_ID_LIST,
    C_MENU_ID_GLOBAL,
    C_MENU_ID_MIDI_MODE,
    C_MENU_ID_NEW,
    C_MENU_ID_PATTERN,
    C_MENU_ID_TOP,
    C_MENU_ID_TRIGS
};

struct CommandMenuItem
{
//    command_menu_id_t m_Parent;
    bool m_SubMenu;
    int m_Command;
    const char * m_Label;
    const char * m_ParameterString;
};


class CommandMenu
{
    public:
        bool Active() { return m_Active; }
        void Open(int menu = C_MENU_ID_TOP);
        void Show();
        void Choose(int i);
        bool HandleKey(BaseUI::key_command_t key);
        void ClearAll();

    private:
        static std::multimap<int, CommandMenuItem> m_MenuItems;

        bool m_Active = false;
        int m_MenuPos = 0;
        int m_Choices = 0;

        std::vector<CommandMenuItem *> m_MenuStack;
        std::pair <std::multimap<int, CommandMenuItem>::iterator, std::multimap<int, CommandMenuItem>::iterator> m_CurrentMenu;
        std::vector<screen_pos_t> m_FieldPositions; // Offset/length.
        std::string m_MenuString;

        int InitMenuPos(int menu);
};


#endif // MACOMMANDMENU_H_INCLUDED
