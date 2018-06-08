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

//#include "maItemMenu.h"
#include "maBaseUI.h"

struct screen_pos_t
{
    int offset;
    int length;
    screen_pos_t(int ofs = 0, int len = 0) { offset = ofs; length = len; }
};


enum command_menu_id_t
{
    C_MENU_ID_NONE,
    C_MENU_ID_PATTERN_STORE,
    C_MENU_ID_PATTERN,
    C_MENU_ID_PATTERN_CHAIN,
    C_MENU_ID_SET,
    C_MENU_ID_SET_FULL,
    C_MENU_ID_SETTINGS,
    C_MENU_ID_SEQUENCE,
    C_MENU_ID_LOOP,
    C_MENU_ID_MIDI_MODE,
    C_MENU_ID_TRIGS,
    C_MENU_ID_STEPLIST,
    C_MENU_ID_STEPLIST_INSERT,
    C_MENU_ID_STEPLIST_COPY,
    C_MENU_ID_STEPLIST_MORE,
    C_MENU_ID_CLUSTER,
    C_MENU_ID_RTLIST
};

struct CommandMenuItem
{
    CommandMenuItem(bool subMenu, int command, const char * label, const char * params):
        m_SubMenu(subMenu),
        m_Command(command),
        m_Label(label),
        m_ParameterString(params)
    {}

    bool m_SubMenu;
    int m_Command;
    const char * m_Label;
    const char * m_ParameterString;
};

struct CommandMenuChoice
{
    int m_ID = -1;
    int m_Pos = -1; // Not used now that we cache all menu choices. Keep the struct, though, in case we
                    // need to put other things on the menu stack.
};

class CommandMenu
{
    public:
        bool Active() { return m_Active; }
        void Open(int menu = C_MENU_ID_PATTERN_STORE);
        void Show();
        void Choose(int i);
        bool HandleKey(BaseUI::key_command_t key);
        void ClearAll();

    private:
        static std::map<int, const char *> m_MenuTitles;
        static std::multimap<int, CommandMenuItem> m_MenuItems;
        static std::map<int, int> m_LastChoices;

        bool m_Active = false;
        int m_MenuChoice = 0;
        int m_Choices = 0;
        int m_CurrentMenuID = 0;

//        std::vector<CommandMenuItem *> m_MenuStack;
        std::vector<CommandMenuChoice> m_MenuStack;
        std::pair <std::multimap<int, CommandMenuItem>::iterator, std::multimap<int, CommandMenuItem>::iterator> m_CurrentMenu;
        std::vector<screen_pos_t> m_FieldPositions; // Offset/length.
        std::string m_MenuString;

        int InitMenuPos(int menu);
};

extern CommandMenu g_CommandMenu;

#endif // MACOMMANDMENU_H_INCLUDED
