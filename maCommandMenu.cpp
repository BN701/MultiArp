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
extern Sequencer g_Sequencer;
#else
#include "platform_Linux/maAlsaSequencer.h"
#include "platform_Linux/maAlsaSequencerQueue.h"
extern AlsaSequencer g_Sequencer;
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

extern State g_State;
extern PatternStore g_PatternStore;
extern AnsiUI g_TextUI;
extern ListBuilder g_ListBuilder;

CommandMenu g_CommandMenu;

multimap<int, CommandMenuItem> CommandMenu::m_MenuItems =
{
    // Pattern Store
    {C_MENU_ID_TOP, {true, C_MENU_ID_PATTERN, "Pattern", ""}},
    {C_MENU_ID_TOP, {true, C_MENU_ID_SET, "Set", ""}},
    {C_MENU_ID_TOP, {true, C_MENU_ID_SETTINGS, "Settings", ""}},

    // Pattern Store -> Set
    {C_MENU_ID_SET, {false, C_NEW_STEP_GROUP, "New Sequence Set", ""}},
    {C_MENU_ID_SET, {false, C_NEW_RT_GROUP, "New Loop Set", ""}},

    // Pattern Store -> Settings
    {C_MENU_ID_SETTINGS, {true, C_MENU_ID_MIDI_MODE, "Capture", ""}},

    // Pattern Store -> Pattern
    {C_MENU_ID_PATTERN, {false, C_NEW_PATTERN, "New", ""}},
    {C_MENU_ID_PATTERN, {false, C_COPY, "Copy", ""}},
    {C_MENU_ID_PATTERN, {false, C_DELETE, "Delete", ""}},

    // Pattern Store -> Settings -> Capture
    {C_MENU_ID_MIDI_MODE, {false, C_MIDI_QUICK, "Quick", ""}},
    {C_MENU_ID_MIDI_MODE, {false, C_MIDI_STEP, "Step", ""}},
    {C_MENU_ID_MIDI_MODE, {false, C_MIDI_REAL_TIME, "Real Time", ""}},

    // Sequence Set
    {C_MENU_ID_SEQUENCE, {false, C_LIST_NEW, "New Layer", ""}},
    {C_MENU_ID_SEQUENCE, {true, C_MENU_ID_SET_FULL, "Set", ""}},
    {C_MENU_ID_SEQUENCE, {true, C_NONE, "Trigs", ""}},

    // Sequence Set -> Set, Loop Set -> Set
    {C_MENU_ID_SET_FULL, {false, C_NEW_STEP_GROUP, "New Sequence Set", ""}},
    {C_MENU_ID_SET_FULL, {false, C_NEW_RT_GROUP, "New Loop Set", ""}},
    {C_MENU_ID_SET_FULL, {false, C_COPY_GROUP, "Copy", ""}},
    {C_MENU_ID_SET_FULL, {false, C_DELETE_GROUP, "Delete", ""}},

    // Sequence Layer
    {C_MENU_ID_STEPLIST, {false, C_NONE, "New", ""}},
    {C_MENU_ID_STEPLIST, {false, C_NONE, "Copy", ""}},
    {C_MENU_ID_STEPLIST, {false, C_NONE, "Delete", ""}},
    {C_MENU_ID_STEPLIST, {true, C_MENU_ID_STEPLIST_LAYER, "Layer", ""}},

    // Sequence Layer -> Layer
    {C_MENU_ID_STEPLIST_LAYER, {false, C_LIST_NEW, "New", ""}},
    {C_MENU_ID_STEPLIST_LAYER, {false, C_LIST_COPY, "Copy", ""}},
    {C_MENU_ID_STEPLIST_LAYER, {false, C_LIST_DELETE, "Delete", ""}},

    // Layer -> Layer
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
            m_MenuPos = g_ListBuilder.MidiInputMode();
            break;

        default:
            m_MenuPos = 0;
            break;
    }

    return m_MenuPos;
}

void CommandMenu::Open(int menu)
{
    m_Active = true;
    m_MenuString = "";
    m_CurrentMenu = m_MenuItems.equal_range(menu);
    m_FieldPositions.clear();
    InitMenuPos(menu);

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

    string title = "Menu";
    for ( auto it = m_MenuStack.begin(); it != m_MenuStack.end(); it++ )
    {
        title += " >";
        title += (*it)->m_Label;
    }
    set_status(STAT_POS_2, title.c_str());

    Show();

}

void CommandMenu::Show()
{
    set_status(COMMAND_HOME, m_MenuString.c_str());
    g_TextUI.HighlightLastWrite(m_FieldPositions[m_MenuPos].offset, m_FieldPositions[m_MenuPos].length, CP_MENU_HIGHLIGHT, BaseUI::attr_bold);
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
        if ( item.m_SubMenu )
        {
            m_MenuStack.push_back(&item);
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
            Choose(m_MenuPos);
            break;

        case BaseUI::key_backspace:
            if ( m_MenuStack.empty() )
                break;
            m_MenuStack.pop_back();
            if ( ! m_MenuStack.empty() )
                Open(m_MenuStack.back()->m_Command);
            else
                Open();
            break;

        case BaseUI::key_left:
            if ( m_MenuPos  > 0 )
            {
                m_MenuPos -= 1;
                Show();
            }
            break;

        case BaseUI::key_right:
            if ( m_MenuPos < m_Choices - 1 )
            {
                m_MenuPos += 1;
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
