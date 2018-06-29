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

#ifndef MAUTILITY_H_INCLUDED
#define MAUTILITY_H_INCLUDED

#include <vector>
#include <string>

#if defined(MA_BLUE) && !defined(MA_BLUE_PC)
#include "alsa_types.h"

#if defined(__DEBUG_FILE_ID__)
#define DEBUG_POS_ACTIVE
extern int g_debug_file;
extern int g_debug_lineno;
extern char g_debug_message[80];
#define DEBUG_POS_AUTO g_debug_file = __DEBUG_FILE_ID__; g_debug_lineno = __LINE__
#else
#define DEBUG_POS_AUTO
#define __DEBUG_FILE_ID__ -2
#endif

#else

#include <alsa/asoundlib.h>

#define DEBUG_POS_AUTO
#define __DEBUG_FILE_ID__ -3

#endif


std::vector<std::string> split(const char *str, char c = ' ', bool wantEmptyTokens = false);
std::string find_token(std::string s, const char * name);

#if !defined(MA_BLUE) || defined(MA_BLUE_PC)
std::string get_clipboard();
void copy_clipboard(std::string s);
#endif

int64_t lcm(int64_t a, int64_t b);
int64_t gcd(int64_t a, int64_t b);

double tidy(double val, int dp = 3);
double tidy_3(double val);

bool equals(double val1, double val2, int dp = 3);
bool equals_3(double val1, double val2);

uint64_t EventTimeToMicros(snd_seq_event_t & ev);
uint64_t AlsaTimeStampToMicros(snd_seq_timestamp & t);

uint32_t FreeMem();
void MemStats(uint32_t & heap_top, uint32_t & stack_bottom, uint32_t & free_mem);


#endif // MAUTILITY_H_INCLUDED
