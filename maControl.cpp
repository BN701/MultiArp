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

#include "maControl.h"

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
    CM_MIDI_CONFIRM,
    CM_MIDI_REC,
    CM_MIDI_RUN,
    CM_MIDI_STOP,
    CM_MIDI_MOD_INSERT,
    CM_MIDI_MOD_COPY,
    CM_MIDI_MOD_MOVE,
    CM_MIDI_MOD_ROTATE
};

typedef unordered_map<unsigned int, control_message_t> message_map_t;

message_map_t g_MessageMap_OP1 =
{
    {1, CM_MIDI_ENC_1},
    {2, CM_MIDI_ENC_2},
    {3, CM_MIDI_ENC_3},
    {4, CM_MIDI_ENC_4},
    {5, CM_MIDI_BACK},       // Blue enc press
    {6, CM_MIDI_ENTER},      // Green enc press
    {7, CM_MIDI_MOD_INSERT},
    {8, CM_MIDI_MOD_COPY},
    {9, CM_MIDI_MOD_MOVE},
    {10, CM_MIDI_MOD_ROTATE},
    {11, CM_MIDI_LEFT},
    {12, CM_MIDI_RIGHT},
    {13, CM_MIDI_UP},
    {14, CM_MIDI_DOWN},
    {0x26, CM_MIDI_REC},        // Rec
    {0x27, CM_MIDI_RUN},        // Play
    {0x28, CM_MIDI_STOP},       // Stop
    {0x30, CM_MIDI_CONFIRM},    // Mic
    {0x31, CM_MIDI_DELETE},     // Disc/Com
    {0x1a, CM_MIDI_MENU}        // Seq

};

bool g_shift_rec_mode = false;

void control(control_message_t message, int value)
{
    static bool modInsert = false;
    static bool modCopy = false;
    static bool modMove = false;
    static bool modRotate = false;

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
        case CM_MIDI_MOD_ROTATE:
            modRotate = value != 0;
            break;
        default:
            newModState = false;
            break;
    }

    if ( newModState )
        return;

    // Check for inc/dec mapping ...

    BaseUI::key_command_t key = BaseUI::key_none;

    switch (message)
    {
        case CM_MIDI_ENC_1:
            if ( value == 1 )
                key = BaseUI::key_cmd_inc;
            else
                key = BaseUI::key_cmd_dec;
            break;
        case CM_MIDI_ENC_2:
            if ( value == 1 )
                key = BaseUI::key_cmd_inc_2;
            else
                key = BaseUI::key_cmd_dec_2;
            break;
        default:
            break;
    }

    if ( key != BaseUI::key_none )
    {
        ItemMenu::RouteKey(key);
        return;
    }

    // Discard any other 'key up' messages.

    if ( value == 0 )
        return;

    // Handle message.

    switch (message)
    {
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
            if ( g_State.RecState() && g_ListBuilder.HandleKeybInput(BaseUI::key_space) )
                show_listbuilder_status();
            break;

        case CM_MIDI_DELETE:
            if ( g_State.RecState() && g_ListBuilder.HandleKeybInput(BaseUI::key_backspace) )
                show_listbuilder_status();
            break;

        case CM_MIDI_CONFIRM:
            if ( g_State.RecState() && g_ListBuilder.HandleKeybInput(BaseUI::key_return) )
            {
                if ( g_ListBuilder.RealTimeRecord() )
                    g_PatternStore.UpdatePattern(g_ListBuilder.RealTimeList(), g_State.Quantum());
                else
                    g_PatternStore.UpdatePattern(g_ListBuilder.CurrentList());
                g_ListBuilder.Clear();
                set_status(STAT_POS_2, "");
            }
            break;

        case CM_MIDI_MENU:
            do_command("", C_MENU);
            break;

        case CM_MIDI_LEFT:
            if ( modInsert )
                key = BaseUI::key_cmd_insert_left;
            else if ( modCopy )
                key = BaseUI::key_cmd_copy_left;
            else if ( modMove)
                key = BaseUI::key_cmd_move_left;
            else if ( modRotate )
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
            else if ( modRotate )
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

        case CM_MIDI_BACK:
            key = BaseUI::key_cmd_back;
            break;

        case CM_MIDI_ENTER:
            key = BaseUI::key_cmd_enter;
            break;

        default:
            break;
    }

    ItemMenu::RouteKey(key);
}

void handle_midi_control_event(unsigned int param, int value)
{
    set_status(STAT_POS_2, "Midi: CC=%i, Val=%i", param, value);

    message_map_t & message_map = g_MessageMap_OP1;     // Todo: replace with proper lookup at some point.

    if ( message_map.count(param) != 0 )
        control(message_map[param], value);
}
