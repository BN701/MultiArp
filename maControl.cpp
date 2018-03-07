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

#include "maCursorKeys.h"
#include "maScreen.h"

#include <unordered_map>

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
    CM_MIDI_MODE
};

typedef unordered_map<unsigned int, control_message_t> message_map_t;

message_map_t g_MessageMap_OP1 =
{
    {0x26, CM_MIDI_MODE},   // Rec
    {0x01, CM_MIDI_LR},     // Blue enc
    {0x02, CM_MIDI_UD},     // Green enc
    {0x40, CM_MIDI_BACK},   // Blue enc press
    {0x41, CM_MIDI_ENTER}   // Green enc press
};

bool g_shift_rec_mode = false;

void control(control_message_t message, int value)
{
    BaseUI::key_command_t key = BaseUI::key_none;

    switch (message)
    {
        case CM_MIDI_MODE:
            break;

        case CM_MIDI_LR:
        case CM_MIDI_UD:
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
            }

            CursorKeys::RouteKey(key);
            show_status_after_navigation();
            update_edit_panels();
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
