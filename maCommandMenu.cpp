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

#include "maCommand.h"
#include "maCommandMenu.h"

#include <algorithm>
#include <cstring>
#include <cmath>
#include <unordered_map>
#include <array>

#ifdef MA_BLUE
#include <cstdio>
#include <cstdlib>
#include "maSequencer.h"
#else
#include "platform_Linux/maAlsaSequencer.h"
#include "platform_Linux/maAlsaSequencerQueue.h"
#endif // MA_BLUE

//#include "maItemMenu.h"
#include "maListBuilder.h"
#include "maPatternStore.h"
#include "maScreen.h"
#include "maState.h"
#include "maTranslateTable.h"
#include "maUtility.h"

using namespace std;

// Global objects.

#ifndef MA_BLUE
#endif

extern AnsiUI g_TextUI;

CommandMenu g_CommandMenu;

std::map<int, int> CommandMenu::m_LastChoices;

map<int, const char *> CommandMenu::m_MenuTitles =
{
    {C_MENU_ID_NONE, ""},
    {C_MENU_ID_TOP, "Pattern"},
    {C_MENU_ID_PATTERN, "Current Pattern"},
    {C_MENU_ID_SET, "Layer"},
    {C_MENU_ID_SET_FULL, "Layer"},
    {C_MENU_ID_SETTINGS, "Settings"},
    {C_MENU_ID_SEQUENCE, "Sequence"},
    {C_MENU_ID_LOOP, "Loop"},
    {C_MENU_ID_MIDI_MODE, "Capture"},
    {C_MENU_ID_TRIGS, "Trigs"},
    {C_MENU_ID_STEPLIST, "Step"},
    {C_MENU_ID_STEPLIST_INSERT, "Insert"},
    {C_MENU_ID_STEPLIST_COPY, "Copy"},
    {C_MENU_ID_STEPLIST_MORE, "List"},
    {C_MENU_ID_CLUSTER, "Step Detail"},
    {C_MENU_ID_RTLIST, "Looper"}
};

multimap<int, CommandMenuItem> CommandMenu::m_MenuItems =
{
    // Pattern Store
//    {C_MENU_ID_TOP, {true, C_MENU_ID_PATTERN, "Pattern", ""}},
//    {C_MENU_ID_TOP, {true, C_MENU_ID_SET, "Layer", ""}},
    {C_MENU_ID_TOP, {false, C_NEW_PATTERN, "New", ""}},
    {C_MENU_ID_TOP, {false, C_CUE_CURRENT, "Cue", ""}},
    {C_MENU_ID_TOP, {false, C_COPY, "Copy", ""}},
    {C_MENU_ID_TOP, {false, C_DELETE, "Delete", ""}},
    {C_MENU_ID_TOP, {false, C_UNDO_PATTERN_DELETE, "Undo", ""}},
    {C_MENU_ID_TOP, {true, C_MENU_ID_SETTINGS, "Settings", ""}},

    // Pattern Store -> Settings
    {C_MENU_ID_SETTINGS, {true, C_MENU_ID_MIDI_MODE, "Capture", ""}},

    // Pattern Store -> Pattern
    {C_MENU_ID_PATTERN, {false, C_NEW_STEP_GROUP, "New Sequence Layer", ""}},
    {C_MENU_ID_PATTERN, {false, C_NEW_RT_GROUP, "New Loop Layer", ""}},
    {C_MENU_ID_PATTERN, {false, C_PATTERN_RUN_ALL, "Run", ""}},
    {C_MENU_ID_PATTERN, {false, C_PATTERN_STOP_ALL, "Stop", ""}},

    // Pattern Store -> Settings -> Capture
    {C_MENU_ID_MIDI_MODE, {false, C_MIDI_QUICK, "Quick", ""}},
    {C_MENU_ID_MIDI_MODE, {false, C_MIDI_STEP, "Step", ""}},
    {C_MENU_ID_MIDI_MODE, {false, C_MIDI_REAL_TIME, "Real Time", ""}},

    // Sequencer
    {C_MENU_ID_SEQUENCE, {false, C_LIST_NEW, "New List", ""}},
    {C_MENU_ID_SEQUENCE, {true, C_MENU_ID_SET_FULL, "Layer", ""}},
    {C_MENU_ID_SEQUENCE, {true, C_NONE, "Trigs", ""}},

    // Sequencer -> More
    {C_MENU_ID_SET_FULL, {false, C_NEW_STEP_GROUP, "New Sequence Layer", ""}},
    {C_MENU_ID_SET_FULL, {false, C_NEW_RT_GROUP, "New Loop Layer", ""}},
    {C_MENU_ID_SET_FULL, {false, C_COPY_GROUP, "Copy Layer", ""}},
    {C_MENU_ID_SET_FULL, {false, C_DELETE_GROUP, "Delete Layer", ""}},

    // Step
    {C_MENU_ID_STEPLIST, {true, C_MENU_ID_STEPLIST_INSERT, "Insert Step", ""}},
    {C_MENU_ID_STEPLIST, {false, C_STEP_COPY_RIGHT, "Copy Step", ""}},
    {C_MENU_ID_STEPLIST, {false, C_STEP_DELETE, "Delete Step", ""}},
    {C_MENU_ID_STEPLIST, {true, C_MENU_ID_STEPLIST_MORE, "More ...", ""}},

    // Step -> Insert
    {C_MENU_ID_STEPLIST_INSERT, {false, C_STEP_INSERT_LEFT, "Left of cursor", ""}},
    {C_MENU_ID_STEPLIST_INSERT, {false, C_STEP_INSERT_RIGHT, "Right of cursor", ""}},

    // Step -> Copy (Not used)
    {C_MENU_ID_STEPLIST_COPY, {false, C_STEP_COPY_LEFT, "Left of cursor", ""}},
    {C_MENU_ID_STEPLIST_COPY, {false, C_STEP_COPY_RIGHT, "Right of cursor", ""}},

    // List -> More
    {C_MENU_ID_STEPLIST_MORE, {false, C_LIST_NEW, "New", ""}},
    {C_MENU_ID_STEPLIST_MORE, {false, C_LIST_COPY, "Copy", ""}},
    {C_MENU_ID_STEPLIST_MORE, {false, C_LIST_DELETE, "Delete", ""}},
    {C_MENU_ID_STEPLIST_MORE, {false, C_LIST_MOVE_UP, "Move Up", ""}},
    {C_MENU_ID_STEPLIST_MORE, {false, C_LIST_MOVE_DOWN, "Move Down", ""}},

    // Step Detail (Cluster)
    {C_MENU_ID_CLUSTER, {false, C_CLUSTER_INSERT_LEFT, "Insert Left", ""}},
    {C_MENU_ID_CLUSTER, {false, C_CLUSTER_INSERT_RIGHT, "Insert Right", ""}},
    {C_MENU_ID_CLUSTER, {false, C_CLUSTER_COPY_RIGHT, "Copy", ""}},
    {C_MENU_ID_CLUSTER, {false, C_CLUSTER_DELETE, "Delete", ""}},

    // Not used (yet)
    {C_MENU_ID_LOOP, {false, C_NONE, "New List", ""}},
    {C_MENU_ID_LOOP, {false, C_NONE, "Delete", ""}},
    {C_MENU_ID_RTLIST, {false, C_NONE, "Edit", ""}},
    {C_MENU_ID_RTLIST, {false, C_NONE, "Delete", ""}},
};

int CommandMenu::InitMenuPos(int menu)
{
    switch (menu)
    {
        case C_MENU_ID_MIDI_MODE:
            m_MenuChoice = g_ListBuilder.MidiInputMode();
            break;

        default:
        {
            auto it = m_LastChoices.find(menu);
            if ( it != m_LastChoices.end() )
                m_MenuChoice = it->second;
            else
                m_MenuChoice = 0;
            break;
        }
    }

    return m_MenuChoice;
}

void CommandMenu::Open(int menu)
{
    m_Active = true;
    m_MenuString = "";
    m_CurrentMenu = m_MenuItems.equal_range(menu);
    m_FieldPositions.clear();

//    if ( choice == -1 )
//        InitMenuPos(menu);
//    else
//        m_MenuChoice = choice;
    InitMenuPos(menu);

    m_CurrentMenuID = menu;

    int i = 1;
    m_Choices = 0;

    for (auto it = m_CurrentMenu.first; it != m_CurrentMenu.second; it++, i++)
    {
        char prefix[20];
        snprintf(prefix, 20, "%s%i) ", i == 1 ? "" : " ", i);
        m_MenuString += prefix;
        int pos = m_MenuString.size();
        m_MenuString += it->second.m_Label;
        m_FieldPositions.emplace_back(pos, static_cast<int>(m_MenuString.size() - pos));
        m_Choices += 1;
    }

    // Only do this once.

    string title;
    for ( auto it = m_MenuStack.begin(); it != m_MenuStack.end(); it++ )
    {
        title += m_MenuTitles[it->m_ID];
        title += "/";
    }
    title += m_MenuTitles[m_CurrentMenuID];
    set_status(STAT_POS_2, title.c_str());

    Show();

}

void CommandMenu::Show()
{
    set_status(COMMAND_HOME, m_MenuString.c_str());
    g_TextUI.HighlightLastWrite(m_FieldPositions[m_MenuChoice].offset, m_FieldPositions[m_MenuChoice].length, CP_MENU_HIGHLIGHT, BaseUI::attr_bold);
}

void CommandMenu::ClearAll()
{
    m_MenuStack.clear();
    m_Active = false;
}

void CommandMenu::Choose(int choice)
{
    int i = 0;
    auto it = m_CurrentMenu.first;
    while ( it != m_CurrentMenu.second && i < choice )
    {
        it++;
        i++;
    }
    if ( it != m_CurrentMenu.second )
    {
        CommandMenuItem & item = it->second;
        m_LastChoices[m_CurrentMenuID] = choice;
        if ( item.m_SubMenu )
        {
            m_MenuStack.emplace_back();
            m_MenuStack.back().m_ID = m_CurrentMenuID;
            m_MenuStack.back().m_Pos = choice;  // Not used now that we cache all menu choices.
            Open(item.m_Command);
        }
        else
        {
            set_status(COMMAND_HOME, "");
            do_command(item.m_ParameterString, item.m_Command);
            ClearAll();
        }
    }
}

bool CommandMenu::HandleKey(BaseUI::key_command_t key)
{
    if ( ! m_Active )
        return false;

    switch ((int) key)
    {
        case '/':
            set_status(STAT_POS_2, "");
            set_status(COMMAND_HOME, "");
            ClearAll();
            break;

        case BaseUI::key_return:
            Choose(m_MenuChoice);
            break;

        case BaseUI::key_backspace:
//            if ( m_MenuStack.empty() )
//                break;
//            m_MenuStack.pop_back();
            if ( ! m_MenuStack.empty() )
            {
                CommandMenuChoice m = m_MenuStack.back();
                m_MenuStack.pop_back();
                Open(m.m_ID);
            }
            else
//                Open();
            {
                set_status(STAT_POS_2, "");
                set_status(COMMAND_HOME, "");
                ClearAll();
            }
            break;

        case BaseUI::key_left:
            if ( m_MenuChoice  > 0 )
            {
                m_MenuChoice -= 1;
                Show();
            }
            break;

        case BaseUI::key_right:
            if ( m_MenuChoice < m_Choices - 1 )
            {
                m_MenuChoice += 1;
                Show();
            }
            break;

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            Choose(key - '1');
            break;
        default:
            break;
    }

    return true;
}
