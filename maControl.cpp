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

extern ListBuilder g_ListBuilder;
extern PatternStore g_PatternStore;
extern State g_State;

using namespace std;

enum control_message_t
{
    CM_MIDI_NOP,
    CM_MIDI_ENTER,
    CM_MIDI_BACK,
    CM_MIDI_LR, // Biderectional encoder, needs further clarification
    CM_MIDI_UD, // Biderectional encoder, needs further clarification
    CM_MIDI_LEFT,
    CM_MIDI_RIGHT,
    CM_MIDI_UP,
    CM_MIDI_DOWN,
    CM_MIDI_REST,
    CM_MIDI_DELETE,
    CM_MIDI_CONFIRM,
    CM_MIDI_REC,
    CM_MIDI_RUN,
    CM_MIDI_STOP
};

typedef unordered_map<unsigned int, control_message_t> message_map_t;

message_map_t g_MessageMap_OP1 =
{
    {0x26, CM_MIDI_REC},        // Rec
    {0x27, CM_MIDI_RUN},        // Play
    {0x28, CM_MIDI_STOP},       // Stop
    {0x01, CM_MIDI_LR},         // Blue enc
    {0x02, CM_MIDI_UD},         // Green enc
    {0x40, CM_MIDI_BACK},       // Blue enc press
    {0x41, CM_MIDI_ENTER},      // Green enc press
    {0x30, CM_MIDI_CONFIRM},    // Mic
    {0x31, CM_MIDI_DELETE},     // Disc/Com
    {0x1a, CM_MIDI_REST}        // Seq

};

bool g_shift_rec_mode = false;

void control(control_message_t message, int value)
{
    // Discard 'key up' for these messages.

    if ( value == 0 )
    {
        switch (message)
        {
            case CM_MIDI_RUN:
            case CM_MIDI_STOP:
            case CM_MIDI_REC:
            case CM_MIDI_REST:
            case CM_MIDI_DELETE:
            case CM_MIDI_CONFIRM:
                return;
            default:
                break;
        }
    }

    // Handle message.
    switch (message)
    {
        case CM_MIDI_RUN:
            do_command("", C_RUN);
//            if ( g_State.RunState() )
//            {
//                g_State.SetPatternReset(RESET_ALL);
//                set_status(STAT_POS_2, "All patterns will be reset.");
//            }
//            else
//            {
//                g_State.SetNewRunStatePending(true);
//                set_status(STAT_POS_2, "Starting ...");
//            }
            break;

        case CM_MIDI_STOP:
            do_command("", C_STOP);
//            g_State.SetNewRunStatePending(false, 1);
//            set_status(STAT_POS_2, "Stopping ...");
            break;

        case CM_MIDI_REC:
            do_command("", C_REC_TOGGLE);
//            g_State.SetRecState(!g_State.RecState());
//            set_status(STAT_POS_2, "Record: %s", g_State.RecState() ? "ON" : "Off");
//            set_top_line();
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
                update_big_panel();
                set_status(STAT_POS_2, "");
            }
            break;

        case CM_MIDI_LR:
        case CM_MIDI_UD:
            {
                BaseUI::key_command_t key = BaseUI::key_none;
                switch (message)
                {
                    case CM_MIDI_LR:
                        if ( value == 1 )
                            key = BaseUI::key_right;
                        else
                            key = BaseUI::key_left;
                        break;

                    case CM_MIDI_UD:
                        if ( value == 1 )
                            key = BaseUI::key_up;
                        else
                            key = BaseUI::key_down;
                        break;
                    default:
                        return;
                }

                ItemMenu::RouteKey(key);
                show_status_after_navigation();
                update_pattern_list_panels();
            }
            break;

        default:
            break;
    }

}

void handle_midi_control_event(unsigned int param, int value)
{
    message_map_t & message_map = g_MessageMap_OP1;     // Todo: replace with proper lookup at some point.

    if ( message_map.count(param) != 0 )
        control(message_map[param], value);
}
