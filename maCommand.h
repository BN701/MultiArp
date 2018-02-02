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

#include <string>
#include <xcb/xcb.h>

// Todo: move X key code dependency and set up our own codes.

#define XK_MISCELLANY
#define XK_XKB_KEYS
#define XK_LATIN1
#include <X11/keysymdef.h>  // XK_ Unicode key name defines


#include "maCursorKeys.h"


void do_command_line(int argc, char *argv[]);
bool do_command(std::string commandList);

bool handle_key_input(CursorKeys::key_type_t curKey, xcb_keysym_t sym);

//#define RESET_NONE 0
//#define RESET_ALL 1
//#define RESET_CURRENT 2


#endif // MACOMMAND_H_INCLUDED
