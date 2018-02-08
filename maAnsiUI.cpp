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

#include <cmath>
#include <cstdio>
#include <set>
#include <map>
#include <unordered_map>

#include "maAnsiUI.h"
#include "maTranslateTable.h"

using namespace std;

#if defined(USE_SERIAL)
size_t SerialWrite(const char * text);
int SerialGetChar();
#endif

AnsiUI::AnsiUI()
{
#if defined(USE_SERIAL)
    // I/O is unbuffered, I think.
#else

    // Without this we may or may not get timely output.
//    int res = setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    // Thanks to http://shtrom.ssji.net/skb/getc.html for this bit:

	// Store the terminal settings for stdin and make a copy.
	tcgetattr(0, &m_old_tio);
	m_new_tio = m_old_tio;

	// Disable canonical mode (buffered i/o), local echo and signals (INTR, QUIT and SUSP).
	m_new_tio.c_lflag &=(~ICANON & ~ECHO & ~ISIG);
	tcsetattr(0, TCSANOW, &m_new_tio);

    Write("\033[2J");   // Clear screen
    Write("\033[12h");  // Echo off, maybe.
//    Write("\033[?25l"); // Cursor off, maybe.

    WriteYX(6, 1, "=> ");

#endif

}


AnsiUI::~AnsiUI()
{
    Write("\033[?25h");  // Cursor on.
    // restore the former settings
#if !defined(USE_SERIAL)
	tcsetattr(0, TCSANOW, &m_old_tio);
#endif
}

size_t AnsiUI::Write(const char * s)
{
#if defined(USE_SERIAL)
    return SerialWrite(s);
#else
    return fputs(s, stdout);
#endif
}

void AnsiUI::PlaceCursor(int row, int col)
{
    char ansi[20];
    snprintf(ansi, 20, "\033[%i;%iH", row + 1, col + 1);
    Write(ansi);
}

size_t AnsiUI::WriteYX(int row, int col, const char * s)
{
    PlaceCursor(row, col);
    return Write(s);
}

//int AnsiUI::CursesAttribute(text_attribute_t attribute)
//{
//    switch (attribute)
//    {
//        case attr_bold:
//            return A_BOLD;
//        case attr_underline:
//            return A_UNDERLINE;
//        default:
//            return A_NORMAL;
//    };
//}

void AnsiUI::ClearArea(window_area_t area)
{
//    WINDOW * window = AreaToWindow(area);
//    wmove(window, 0, 0);
//    wclrtobot(window);
//    wrefresh(window);
}


void AnsiUI::Highlight(window_area_t area, int row, int col, int len, int colour, text_attribute_t attr)
{
//    WINDOW * window = AreaToWindow(area);
//    int scr_x, scr_y;
//    getyx(stdscr, scr_y, scr_x);
//    mvwchgat(window, row, col, len, CursesAttribute(attr), colour, NULL);
//    wrefresh(window);
//    wmove(stdscr, scr_y, scr_x);
}

void AnsiUI::Text(window_area_t area, int row, int col, const char * text, text_attribute_t attribute)
{
    MapToFullScreen(area, row, col);
    WriteYX(row, col, text);
    if ( area == whole_screen )
        Write("\033[0K"); // Clear to end of line.
//    WINDOW * window = AreaToWindow(area);
//    int scr_x, scr_y;
//    attron(attribute);
//    getyx(stdscr, scr_y, scr_x);
//    mvwprintw(window, row, col, text);
//    clrtoeol();
//    wrefresh(window);
//    attroff(attribute);
//    move(scr_y, scr_x);
//    refresh();
}

void AnsiUI::Progress(double progress, double stepWidth, double beat, int pattern_progress,
                        double rtBeat, unsigned int queueSecs, unsigned int queueNano)
{
//    int row = 2;
//    int col = 4;
//    double width = 64;
//
//    int n = lround(width * progress);
//    int len = lround(width * stepWidth);
//
//    if ( len < 1 )
//        len = 1;
//
//    int mode = 0;
//    if ( len < 3 )
//        mode = 1;
//    else if ( len < 9 )
//        mode = 2;
//
//    if ( n + len > 64 )
//        len = width - n;
//
//    char lineFill, markerFill;
//
//    switch ( mode )
//    {
//    case 1:
//        lineFill = '-';
//        markerFill = '>';
//        break;
//    case 2:
//        lineFill = ' ';
//        markerFill = '>';
//        break;
//    default:
//        lineFill = ' ';
//        markerFill = '-';
//        break;
//
//    }
//
//    string marker(len, markerFill);
//    string barline(width, lineFill);
//    char buff[10];
//    snprintf(buff, 10, "%7.2f ", beat /* g_State.Phase() + 1 */);
//    barline += buff;
//
//    int scr_x, scr_y;
//    getyx(stdscr, scr_y, scr_x);
//
//    switch ( mode )
//    {
//    case 1:
//        attron(COLOR_PAIR(CP_PROGRESS_BAR_BKGND));
//        mvprintw(row, col, barline.c_str());
//        attron(COLOR_PAIR(CP_PROGRESS_BAR_HIGHLIGHT));
//        mvprintw(row, col + n, marker.c_str());
//        break;
//    case 2:
//        attron(COLOR_PAIR(CP_PROGRESS_BAR_BKGND));
//        mvprintw(row, col, barline.c_str());
//        attron(COLOR_PAIR(CP_PROGRESS_BAR_HIGHLIGHT));
//        mvprintw(row, col + n, marker.c_str());
//        break;
//    default:
//        attron(COLOR_PAIR(CP_PROGRESS_BAR_BKGND));
//        mvprintw(row, col, barline.c_str());
//        attron(COLOR_PAIR(CP_PROGRESS_BAR_HIGHLIGHT));
//        mvprintw(row, col + n, marker.c_str());
//        break;
//    }
//
//    attroff(COLOR_PAIR(CP_PROGRESS_BAR_HIGHLIGHT));
//
//    // Extra kludge to show overall pattern chain progress.
//
//    mvchgat(2, 4, pattern_progress /* g_PatternStore.CurrentPosPatternChain() + 1 */,
//            A_UNDERLINE, CP_PROGRESS_BAR_BKGND, NULL);
//
//
//    move(scr_y, scr_x);
//    refresh();
//
//    char text[80];
//    snprintf(text, 80, " Beat%9.2f\n (Sec%6u:%u)", rtBeat, queueSecs, queueNano);
//    Text(progress_panel, 0, 0, text);
//
}


void AnsiUI::ShowNoteTransforms(vector<InOutPair> & pairs)
{
//    const int kbdStart = 29;
//    WINDOW *panel = m_BigPanelExtra;
//    wmove(panel, 0, 0);
//    wclrtobot(panel);
//
//    map<int, set<int>> inOutMap;
//
//    if ( !pairs.empty() )
//    {
//        typedef map<int, set<int>> inOutMap_t;
//        inOutMap_t inOutMap;
//
//        for ( auto it = pairs.begin(); it != pairs.end(); it++ )
//        {
////            if ( it-> m_NoteIn >= 0 )
////            {
////                mvwaddch(panel, 3, it->m_NoteIn - kbdStart, '^');
////            }
//
//            if ( it->m_NoteOut >= 0 )
//            {
//                mvwaddch(panel, 1, it->m_NoteOut - kbdStart, 'o');
//
//                // Create or update a set of output notes for this input note.
//
//                // This fails silently if the set already exists.
//
//                /* pair<inOutMap_t::iterator,bool> p = */ inOutMap.emplace(it->m_NoteIn, initializer_list<int>{});
//
//                // (The previous call is supposed to return the set that was either found or
//                // created but I couldn't work out how to access it using the pair above, so
//                // I just do a standard lookup instead.)
//
//                inOutMap.at(it->m_NoteIn).insert(it->m_NoteOut);
//
//            }
//        }
//
//        // For each input note, draw a tree from its output set.
//        // (Will overwrite each other at the moment.)
//
//        for ( inOutMap_t::iterator it = inOutMap.begin(); it != inOutMap.end(); it++ )
//        {
//            // Input note is in it->first, output set is in it->second. There's
//            // no entry created unless there's at least one output note.
//
//            int lower = min(it->first, *(it->second.begin()));
//            int upper = max(it->first, *(it->second.rbegin()));
//
//            chtype boxChar = '*';
//
//            if ( lower == upper )
//            {
//                // Single vertical bar.
//
//                boxChar = ACS_SBSB;
//                mvwaddch(panel, 2, lower - kbdStart, boxChar);
//            }
//            else
//            {
//                // Left hand end character.
//
//                if ( lower == it->first && it->second.find(lower) != it->second.end() )
//                    boxChar = ACS_SSSB;
//                else if ( lower == it->first )
//                    boxChar = ACS_BSSB;
//                else
//                    boxChar = ACS_SSBB;
//
//                mvwaddch(panel, 2, lower - kbdStart, boxChar);
//
//                // Right hand end character.
//
//                if ( upper == it->first && it->second.find(upper) != it->second.end() )
//                    boxChar = ACS_SBSS;
//                else if ( upper == it->first )
//                    boxChar = ACS_BBSS;
//                else
//                    boxChar = ACS_SBBS;
//
//                mvwaddch(panel, 2, upper - kbdStart, boxChar);
//            }
//
//            // Draw the bits in between.
//
//            for ( int i = lower + 1; i < upper; i++ )
//            {
//                if ( i == it->first && it->second.find(i) != it->second.end() )
//                    boxChar = ACS_SSSS;
//                else if ( i == it->first )
//                    boxChar = ACS_BSSS;
//                else if ( it->second.find(i) != it->second.end() )
//                    boxChar = ACS_SSBS;
//                else
//                    boxChar = ACS_BSBS;
//
//                mvwaddch(panel, 2, i - kbdStart, boxChar);
//            }
//        }
//    }
//
//    // Layout the keyboard pattern.
//
//    // It seems we can't set special (box) characters without mucking
//    // up the attributes - attrset() didn't have any effect - and if we
//    // use mvchgat() to set an attribute over a box character, we meess
//    // up the character as well.
//
//    for ( int row = 0; row < 2; row++ )
//        for ( int col = kbdStart; col < kbdStart + 72; col++ )
//            switch ( col % 12 )
//            {
//            case 1:
//            case 3:
//            case 6:
//            case 8:
//            case 10:
//                mvwchgat(panel, row, col - kbdStart, 1, 0, CP_PIANO_BLACK_KEY, NULL);
//                break;
//            default:
//                break;
//            }
//
//    // Make the fourth row black.
//
//    mvwchgat(panel, 3, 0, 72, A_BOLD, CP_PIANO_BLACK_KEY, NULL);
//
////    pairs.clear();
//
//    wrefresh(panel);
}


void AnsiUI::SetTopLine(int midiChannel, double stepValue, double quantum, int runState, int midiInputMode)
{
    char text[100];
    snprintf(text, 100, "Multi Arp - Midi:%02i, Step:%5.2f, Link Quantum:%5.2f     %s",
               midiChannel,
               stepValue,
               quantum,
               runState != 0 ? "<<   RUN   >>" : "<<   ---   >>");

    Text(BaseUI::whole_screen, 0, 0, text, BaseUI::attr_normal);

    vector<int> midiInputColour = {CP_MAIN, CP_RECORD, CP_RECORD, CP_REALTIME};
    Highlight(BaseUI::whole_screen, 0, 0, 80,
        midiInputColour.at(midiInputMode),  // Hmm ...
        BaseUI::attr_bold);

    Highlight(BaseUI::whole_screen, 0, 60, 5,
        runState != 0 ? CP_RUNNING : CP_MAIN,
        BaseUI::attr_bold);

}


#if 0
            key_ctrl_c,     /// !! This currently terminates app. !!
#endif

unordered_map<string, BaseUI::key_command_t> g_EscSeqToKeyCommand =
{
    {"[A", BaseUI::key_up},
    {"[B", BaseUI::key_down},
    {"[C", BaseUI::key_right},
    {"[D", BaseUI::key_left},
    {"[H", BaseUI::key_home},
    {"[F", BaseUI::key_end},
    {"[Z", BaseUI::key_shift_tab},

    {"[1;2A", BaseUI::key_shift_up},
    {"[1;2B", BaseUI::key_shift_down},
    {"[1;2C", BaseUI::key_shift_right},
    {"[1;2D", BaseUI::key_shift_left},

    {"[1;3A", BaseUI::key_alt_up},
    {"[1;3B", BaseUI::key_alt_down},
    {"[1;3C", BaseUI::key_alt_right},
    {"[1;3D", BaseUI::key_alt_left},

    {"[1;4A", BaseUI::key_alt_shift_up},
    {"[1;4B", BaseUI::key_alt_shift_down},
    {"[1;4C", BaseUI::key_alt_shift_right},
    {"[1;4D", BaseUI::key_alt_shift_left},

    {"[1;5A", BaseUI::key_ctrl_up},
    {"[1;5B", BaseUI::key_ctrl_down},
    {"[1;5C", BaseUI::key_ctrl_right},
    {"[1;5D", BaseUI::key_ctrl_left},

    {"[1;6A", BaseUI::key_ctrl_shift_up},        // Not received with Ubuntu/xfce4-terminal
    {"[1;6B", BaseUI::key_ctrl_shift_down},      // Not received with Ubuntu/xfce4-terminal
    {"[1;6C", BaseUI::key_ctrl_shift_right},
    {"[1;6D", BaseUI::key_ctrl_shift_left},

    {"[2~", BaseUI::key_insert},
    {"[3~", BaseUI::key_delete},
    {"[5~", BaseUI::key_page_up},
    {"[6~", BaseUI::key_page_down},
    {"[3;2~", BaseUI::key_shift_delete},
    {"[3;5~", BaseUI::key_ctrl_delete},

    {"[5;5~", BaseUI::key_ctrl_page_up},
    {"[6;5~", BaseUI::key_ctrl_page_down},
    {"[5;3~", BaseUI::key_alt_page_up},
    {"[6;3~", BaseUI::key_alt_page_down},


};

BaseUI::key_command_t GetCSISequence(int firstChar)
{
    /*
        This little loop for capturing control sequences, including ones
        we don't know about or aren't interested in, came from here:

        https://www.real-world-systems.com/docs/ANSIcode.html

        We're only specifically handling Command Sequences starting
        with '[', the 'introducer'. Other sequences may still
        interfere with subsequent input. If so, we should add further
        rules to capture and discard them.
    */

    string sequence;
    sequence += firstChar;
    int c;
    do
    {
        c = getchar();
        sequence += c;
    } while ( c < 0x40 || c > 0x7e );    // CSI sequence ends with 'alphabetic' character.

#if 0
    const char * s = sequence.c_str();
    auto n = g_EscSeqToKeyCommand.count(sequence);
#endif

    if ( g_EscSeqToKeyCommand.count(sequence) == 1 )
        return g_EscSeqToKeyCommand[sequence];
    else
        return BaseUI::key_none;
}

BaseUI::key_command_t AnsiUI::KeyInput()
{
    int c = getchar();

    if ( c == 27 )
    {
        c = getchar();
        switch (c )
        {
            case '[':
                return GetCSISequence(c);
            default:
                break;
        }
    }

    key_command_t key;

    switch (c)
    {
    case 1: // Ctrl-A
        key = key_ctrl_a;
        break;
    case 3:  // Ctrl-C, Copy
        key = key_ctrl_c;
        break;

    case 22: // Ctrl-V, Paste
        key = key_ctrl_v;
        break;

    case 0x0D:
    case 0x0A:
        key = key_return;
        break;

    case 32: // Space bar.
        key = key_space;
        break;

    case 9:
        key = key_tab;
        break;

    case 126:
        return key_delete;

    case 127:
        return key_backspace;

    default:
#if 0
        if ( true )
        {
            set_status(STAT_POS_2, "Key: %i", c);
        }
        else if ( commandString.size() == 0 )
        {
            set_status(STAT_POS_2, "");
        }
        if ( c > 31 && c < 127 )
            commandString += c;
#endif
        key = static_cast<BaseUI::key_command_t>(c);
        break;
    }

    return key;
}
