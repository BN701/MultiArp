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

#include "maMidiControl.h"

#include <unordered_map>

#include "maCommand.h"
#include "maItemMenu.h"
#include "maListBuilder.h"
#include "maPatternStore.h"
#include "maScreen.h"
#include "maState.h"

using namespace std;

enum control_message_t
{
    CM_MIDI_NOP,
    CM_MIDI_ENTER,
    CM_MIDI_BACK,
    CM_MIDI_MENU,
    CM_MIDI_ENC_1, // Biderectional encoder, needs further clarification
    CM_MIDI_ENC_2, // Biderectional encoder, needs further clarification
    CM_MIDI_ENC_3, // Biderectional encoder, needs further clarification
    CM_MIDI_ENC_4, // Biderectional encoder, needs further clarification
    CM_MIDI_LEFT,
    CM_MIDI_RIGHT,
    CM_MIDI_UP,
    CM_MIDI_DOWN,
    CM_MIDI_REST,
    CM_MIDI_DELETE,
    CM_MIDI_COMMIT,
    CM_MIDI_CLEAR_LAST,
    CM_MIDI_REC,
    CM_MIDI_RUN,
    CM_MIDI_STOP,
    CM_MIDI_MOD_INSERT,
    CM_MIDI_MOD_COPY,
    CM_MIDI_MOD_MOVE,
    CM_MIDI_MOD_SHIFT,
    CM_MIDI_ROTATE_LEFT,
    CM_MIDI_ROTATE_RIGHT,
    CM_MIDI_CHOICE_1,
    CM_MIDI_CHOICE_2,
    CM_MIDI_CHOICE_3,
    CM_MIDI_CHOICE_4,
    CM_MIDI_CHOICE_5,
    CM_MIDI_CHOICE_6,
    CM_MIDI_CHOICE_7,
    CM_MIDI_CHOICE_8,
    CM_MIDI_CHOICE_9,
    CM_MIDI_CHOICE_10
};

typedef unordered_map<unsigned int, control_message_t> message_map_t;

message_map_t g_MessageMap_OP1 =
{
    {1, CM_MIDI_ENC_1},
    {2, CM_MIDI_ENC_2},
    {3, CM_MIDI_ENC_3},
    {4, CM_MIDI_ENC_4},
    {5, CM_MIDI_BACK},          // Blue enc press
    {6, CM_MIDI_ENTER},         // Green enc press
    {7, CM_MIDI_MOD_INSERT},    // Synth
    {8, CM_MIDI_MOD_COPY},      // Drum
    {9, CM_MIDI_MOD_MOVE},      // Tape
    {10, CM_MIDI_MENU},         // Mix
    {11, CM_MIDI_LEFT},         // Track 1
    {12, CM_MIDI_RIGHT},        // Track 2
    {13, CM_MIDI_UP},           // Track 3
    {14, CM_MIDI_DOWN},         // Track 4
    {15, CM_MIDI_MOD_SHIFT},    // Lift
    {16, CM_MIDI_NOP},          // Drop
    {17, CM_MIDI_NOP},          // Scissors
    {26, CM_MIDI_NOP},          // Seq
    {38, CM_MIDI_REC},          // Rec
    {39, CM_MIDI_RUN},          // Play
    {40, CM_MIDI_STOP},         // Stop
    {41, CM_MIDI_NOP},          // '<'
    {42, CM_MIDI_NOP},          // '>'
    {48, CM_MIDI_COMMIT},       // Mic
    {49, CM_MIDI_REST},         // Disc/Com
    {50, CM_MIDI_CHOICE_1},     // '1'
    {51, CM_MIDI_CHOICE_2},     // '2'
    {52, CM_MIDI_CHOICE_3},     // '3'
    {21, CM_MIDI_CHOICE_4},     // '4'
    {22, CM_MIDI_CHOICE_5},     // '5'
    {23, CM_MIDI_CHOICE_6},     // '6'
    {24, CM_MIDI_CHOICE_7},     // '7'
    {25, CM_MIDI_CHOICE_8},     // '8'
    {64, CM_MIDI_NOP},          // Encoder press, blue
    {65, CM_MIDI_NOP},          // Encoder press, green
    {66, CM_MIDI_NOP},          // Encoder press, white
    {67, CM_MIDI_NOP},          // Encoder press, orange
};

//unordered_map<unsigned int, BaseUI::key_command_t> g_SimpleCommandMap =
//{
//    {CM_MIDI_BACK, BaseUI::key_cmd_back},
//    {CM_MIDI_ENTER, BaseUI::key_cmd_enter},
//    {CM_MIDI_CHOICE_1, BaseUI::key_choice_1},
//    {CM_MIDI_CHOICE_2, BaseUI::key_choice_2},
//    {CM_MIDI_CHOICE_3, BaseUI::key_choice_3},
//    {CM_MIDI_CHOICE_4, BaseUI::key_choice_4},
//    {CM_MIDI_CHOICE_5, BaseUI::key_choice_5},
//    {CM_MIDI_CHOICE_6, BaseUI::key_choice_6},
//    {CM_MIDI_CHOICE_7, BaseUI::key_choice_7},
//    {CM_MIDI_CHOICE_8, BaseUI::key_choice_8},
//    {CM_MIDI_CHOICE_9, BaseUI::key_choice_9}
//};

void control(control_message_t message, int value)
{
    static bool modInsert = false;
    static bool modCopy = false;
    static bool modMove = false;
    static bool modShift = false;

    // Do we have a modifier key?

    bool newModState = true;

    switch ( message )
    {
        case CM_MIDI_MOD_INSERT:
            modInsert = value != 0;
            break;
        case CM_MIDI_MOD_COPY:
            modCopy = value != 0;
            break;
        case CM_MIDI_MOD_MOVE:
            modMove = value != 0;
            break;
        case CM_MIDI_MOD_SHIFT:
            modShift = value != 0;
            break;
        default:
            newModState = false;
            break;
    }

    if ( newModState )
        return;

    // Check for encoder mapping ...

    BaseUI::key_command_t key = BaseUI::key_none;

    switch (message)
    {
        case CM_MIDI_ENC_1:
            if ( value == 1 )   // Todo: This value may be OP1 specific.
                key = BaseUI::key_cmd_inc;
            else
                key = BaseUI::key_cmd_dec;
            break;

        case CM_MIDI_ENC_2:
            if ( value == 1 )   // Todo: This value may be OP1 specific.
                key = BaseUI::key_cmd_inc_2;
            else
                key = BaseUI::key_cmd_dec_2;
            break;

        case CM_MIDI_ENC_3:
            if ( value == 1 )   // Todo: This value may be OP1 specific.
                message = CM_MIDI_RIGHT;
            else
                message = CM_MIDI_LEFT;
            break;

        case CM_MIDI_ENC_4:
            if ( value == 1 )   // Todo: This value may be OP1 specific.
                message = CM_MIDI_UP;
            else
                message = CM_MIDI_DOWN;
            break;

        default:
            break;
    }

    if ( key != BaseUI::key_none )
    {
        ItemMenu::RouteKey(key);
        return;
    }

    // Not interested in any other 'key up' messages.

    if ( value == 0 )
        return;

    // Check for further key mappings or handle message directly.

//    if ( g_SimpleCommandMap.count(message) == 1 )
//    {
//        key = g_SimpleCommandMap[message];
//    }
//    else
    {
        switch (message)
        {
            case CM_MIDI_ENTER:
                if ( /* g_State.RecState() && */ g_ListBuilder.HandleKeybInput(BaseUI::key_cmd_midi_commit) )
                {
//                    if ( g_ListBuilder.RealTimeRecord() )
////                        g_PatternStore.UpdatePattern(g_ListBuilder.RealTimeList(), g_State.Quantum());
//                        ItemMenu::UpdateFocusItem(g_ListBuilder.RealTimeList(), g_State.Quantum());
//                    else
//                        ItemMenu::UpdateFocusItem(g_ListBuilder.CurrentList());
//                    g_ListBuilder.Clear();
                    set_status(STAT_POS_2, "");
                }
                else
                    key = BaseUI::key_cmd_enter;
                break;

            case CM_MIDI_BACK:
                if ( /* g_State.RecState() && */ g_ListBuilder.HandleKeybInput(BaseUI::key_cmd_midi_clear_last) )
                    show_listbuilder_status();
                else
                    key = BaseUI::key_cmd_back;
                break;

            case CM_MIDI_MENU:
                do_command("", C_MENU);
                break;

            case CM_MIDI_RUN:
                do_command("", C_RUN);
                break;

            case CM_MIDI_STOP:
                do_command("", C_STOP);
                break;

            case CM_MIDI_REC:
                do_command("", C_REC_TOGGLE);
                break;

            case CM_MIDI_REST:
                if ( /* g_State.RecState() && */ g_ListBuilder.HandleKeybInput(BaseUI::key_cmd_midi_rest) )
                    show_listbuilder_status();
                break;

            case CM_MIDI_CLEAR_LAST:
                break;

            case CM_MIDI_COMMIT:
                break;

            case CM_MIDI_CHOICE_1:
                key = BaseUI::key_choice_1;
                break;
            case CM_MIDI_CHOICE_2:
                key = BaseUI::key_choice_2;
                break;
            case CM_MIDI_CHOICE_3:
                key = BaseUI::key_choice_3;
                break;
            case CM_MIDI_CHOICE_4:
                key = BaseUI::key_choice_4;
                break;
            case CM_MIDI_CHOICE_5:
                key = BaseUI::key_choice_5;
                break;
            case CM_MIDI_CHOICE_6:
                key = BaseUI::key_choice_6;
                break;
            case CM_MIDI_CHOICE_7:
                key = BaseUI::key_choice_7;
                break;
            case CM_MIDI_CHOICE_8:
                if ( modShift )
                    key = BaseUI::key_cmd_delete;
                else
                    key = BaseUI::key_choice_8;
                break;

            case CM_MIDI_LEFT:
                if ( modInsert )
                    key = BaseUI::key_cmd_insert_left;
                else if ( modCopy )
                    key = BaseUI::key_cmd_copy_left;
                else if ( modMove)
                    key = BaseUI::key_cmd_move_left;
                else if ( modShift )
                    key = BaseUI::key_cmd_shift_left;
                else
                    key = BaseUI::key_cmd_left;
                break;
            case CM_MIDI_RIGHT:
                if ( modInsert )
                    key = BaseUI::key_cmd_insert_right;
                else if ( modCopy )
                    key = BaseUI::key_cmd_copy_right;
                else if ( modMove)
                    key = BaseUI::key_cmd_move_right;
                else if ( modShift )
                    key = BaseUI::key_cmd_shift_right;
                else
                    key = BaseUI::key_cmd_right;
                break;
            case CM_MIDI_UP:
                if ( modMove )
                    key = BaseUI::key_cmd_move_up;
                else
                    key = BaseUI::key_cmd_up;
                break;
            case CM_MIDI_DOWN:
                if ( modMove )
                    key = BaseUI::key_cmd_move_down;
                else
                    key = BaseUI::key_cmd_down;
                break;

            default:
                break;
        }
    }

    // If command menu is open it eats all keys.

    if ( g_CommandMenu.HandleKey(key) )
        return;
    else
        ItemMenu::RouteKey(key);
}

void handle_midi_control_event(unsigned int param, int value)
{
//    set_status(STAT_POS_2, "Midi: CC=%i, Val=%i", param, value);

    // Look up incoming control message and obtain internal midi handling message.

    message_map_t & message_map = g_MessageMap_OP1;     // Todo: replace with proper lookup at some point.

    if ( message_map.count(param) != 0 )
        control(message_map[param], value);
}
