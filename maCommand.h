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

#ifndef MACOMMAND_H_INCLUDED
#define MACOMMAND_H_INCLUDED

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

void do_command_line(int argc, char *argv[]);
#endif // MA_BLUE

#include "maItemMenu.h"
#include "maBaseUI.h"

enum command_t
{
    C_NONE,
    C_MENU,
    C_EXIT,
    C_HELP,
    C_RESET_SCREEN,
    C_CRASH_TEST,

    C_RUN,
    C_STOP,
    C_HALT,
    C_RESET,
    C_SET_RESETONPATTERNCHANGE,
    C_RESET_BEAT,
    C_REC_TOGGLE,

    C_SET_LABEL,

    C_TRIGS, // Open the trig menu on current pattern or enter a list.
    C_TRIGS_ARPEGGIO,

    C_STEPVAL,
    C_QUANTUM,
    C_GATELENGTH,
    C_GATE_HOLD,
    C_GATE_NORMAL,
    C_VELOCITY,
    C_TRANSPOSE,
    C_TEMPO,

    C_SCALE,
    C_SCALE_FROM_LIST,
    C_SCALE_SHOW,
    C_SCALE_HELP,
    C_SCALE_CONTROLS,
    C_SETROOT,

    C_PAGE_ONE,
    C_PAGE_TWO,
    C_PAGE_THREE,
    C_PAGE_HOLD,

    C_FEEL,
    C_FEEL_HELP,
    C_FEEL_NEW,
    C_FEEL_ADD,
    C_FEEL_REMOVE,
    C_FEEL_RESPACE,
    C_FEEL_ON,
    C_FEEL_OFF,

    C_MIDI,
    C_MIDI_REAL_TIME,
    C_MIDI_STEP,
    C_MIDI_QUICK,
//    C_MIDI_OFF,

    C_CUE,              // Set the next pattern to play
    C_EDIT,             // Set focus for copy/paste
    C_EDIT_ITEM,
    C_EDIT_CURSOR_LOCK,
    C_EDIT_CURSOR_UNLOCK,
    C_EDIT_CURSOR_LOCK_STATUS,
    C_NEW_PATTERN,              // Create new (empty) pattern.
    C_NEW_STEP_GROUP,
    C_NEW_RT_GROUP,
    C_COPY,             // Copy current pattern into a new entry at the end of the pattern list.
    C_DELETE,           // Delete pattern and reduce size of pattern list.
    C_CLEAR,            // Clear pattern but leave empty entry in pattern list.
    C_UNDO,             // Restore deleted or cleared pattern to end of the pattern list.
    C_STATUS,           // Display current stats.

    C_PATTERN_CHAIN,    // Set pattern chain mode.
    C_PATTERN_CHAIN_OFF,
    C_PATTERN_CHAIN_NATURAL,
    C_PATTERN_CHAIN_QUANTUM,
    C_PATTERN_CHAIN_CLEAR,
    C_PATTERN_CHAIN_NEW,
    C_PATTERN_CHAIN_DELETE,
    C_PATTERN_CHAIN_JUMP,
    C_PATTERN_CHAIN_HELP,

    C_STORE,
    C_STORE_STEP,
    C_STORE_GATE,
    C_STORE_VELOCITY,
    C_STORE_SCALE,
    C_STORE_ALL,
    C_STORE_HELP,
    C_LOAD,
    C_LOAD_STEP,
    C_LOAD_GATE,
    C_LOAD_VELOCITY,
    C_LOAD_SCALE,
    C_LOAD_ALL,

    C_USE,
    C_USE_GLOBAL_PLAYDATA,
    C_USE_PATTERN_PLAYDATA,
    C_USE_HELP,

    C_LIST,             // Note list commands.
    C_LIST_NEW,
    C_LIST_EDIT,
    C_LIST_DELETE,
    C_LIST_IMPORT,      // Import from midi file.

    C_LIST_RT,          // Real time list commands.
    C_LIST_RT_DELETE,
    C_LIST_RT_RATE,     // Set playback multipliers for real time lists
    C_LIST_RT_QUANTUM,  // Set quantum for real time lists.
    C_LIST_RT_START_PHASE,
    C_LIST_RT_ECHO,     // Create drifting echo of notes in current list.

    C_HELP_1,
    C_HELP_2,
    C_HELP_3,
    C_HELP_4,

    C_NUM_COMMANDS
};





bool do_command(std::string commandList, int directCommand = C_NONE);
//void do_command_menu(command_menu_id_t menu = C_MENU_ID_TOP);

//bool handle_key_input(ItemMenu::key_type_t curKey, xcb_keysym_t sym);
bool handle_key_input(BaseUI::key_command_t key);

//#define RESET_NONE 0
//#define RESET_ALL 1
//#define RESET_CURRENT 2


#endif // MACOMMAND_H_INCLUDED
