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
#include <set>
#include <map>
#include <unordered_map>

#include <ncurses.h>

#define XK_MISCELLANY
#define XK_XKB_KEYS
#define XK_LATIN1
#include <X11/keysymdef.h>  // XK_ Unicode key name defines

#include "maTextUI.h"
#include "maTranslateTable.h"

using namespace std;

#define COLOUR_GREEN        8
#define COLOUR_RED          9
#define COLOUR_BLUE         10
#define COLOUR_GREY         11
#define COLOUR_BRIGHT_RED   12
#define COLOUR_REDDISH      13
#define COLOUR_BRIGHT_GREEN 14
#define COLOUR_YELLOW       16
#define COLOUR_DARK_GREY    17
#define COLOUR_DARKER_GREY  18
#define COLOUR_WHITE        19
#define COLOUR_BLACK        20


TextUI::TextUI()
{
    // Set up NCurses.

    initscr();
    raw();
    keypad(stdscr, true);
    noecho();
    curs_set(1); // 0, 1, or 2 (the latter supposed to be 'highly visible', but same as 1 on my terminal.

    start_color();

    init_color(COLOUR_GREEN, 0, 400, 0);
    init_color(COLOUR_BRIGHT_GREEN, 400, 750, 400);
    init_color(COLOUR_YELLOW, 750, 500, 0);
    init_color(COLOUR_RED, 750, 0, 0);
    init_color(COLOUR_REDDISH, 750, 300, 200);
    init_color(COLOUR_BLUE, 250, 750, 900);
    init_color(COLOUR_BRIGHT_RED, 1000, 0, 0);
    init_color(COLOUR_GREY, 750, 750, 750);
    init_color(COLOUR_DARK_GREY, 500, 500, 500);
    init_color(COLOUR_DARKER_GREY, 250, 250, 250);

    init_color(COLOUR_WHITE, 1000, 1000, 1000);
    init_color(COLOUR_BLACK, 0, 0, 0);

	init_pair(CP_MAIN, COLOUR_WHITE, COLOUR_BLACK);
	init_pair(CP_RUNNING, COLOUR_WHITE, COLOUR_BRIGHT_GREEN);
	init_pair(CP_REALTIME, COLOUR_WHITE, COLOUR_RED);
	init_pair(CP_RECORD, COLOUR_WHITE, COLOUR_YELLOW);
	init_pair(CP_MENU_HIGHLIGHT, COLOUR_WHITE, COLOUR_REDDISH);

	init_pair(CP_PATTERN_LIST_PANEL, COLOUR_BRIGHT_GREEN, COLOUR_BLACK);
	init_pair(CP_PATTERN_LIST_PANEL_HIGHLIGHT, COLOUR_WHITE, COLOUR_BLACK);
	init_pair(CP_PATTERN_LIST_PANEL_HIGHLIGHT_TRIG, COLOUR_BRIGHT_GREEN, COLOUR_DARKER_GREY);

	init_pair(CP_PIANO_WHITE_KEY, COLOUR_GREY, COLOUR_DARKER_GREY);
	init_pair(CP_PIANO_BLACK_KEY, COLOUR_GREY, COLOUR_BLACK);

    init_pair(CP_LIST_PANEL_BKGND, COLOUR_GREY, COLOUR_BLACK);
    init_pair(CP_SUMMARY_PANEL_BKGND, COLOUR_GREY, COLOR_BLACK);
    init_pair(CP_SMALL_PANEL_BKGND, COLOR_YELLOW, COLOUR_BLACK);
    init_pair(CP_SMALL_PANEL_2_BKGND, COLOUR_GREY, COLOUR_BLACK);
    init_pair(CP_PATTERN_CHAIN_HIGHLIGHT, COLOR_YELLOW, COLOUR_DARKER_GREY);

    init_pair(CP_PROGRESS_BAR_BKGND, COLOUR_GREY, COLOUR_BLACK);
    init_pair(CP_PROGRESS_BAR_HIGHLIGHT, COLOUR_WHITE, COLOUR_BLACK);

    mvprintw(6, 1, "=> ");

    m_SmallPanel = newwin(4, 56, 2, 4);
    m_ProgressPanel = newwin(2, 15, 3, 61);
    m_EditListPanel = newwin(4, 20, 8, 4);
    m_EditSummaryPanel = newwin(4, 52, 8, 24);
    m_BigPanel = newwin(11, 80, 12, 0);
    m_BigPanelExtra = newwin(4, 72, 8, 4);

    bkgd(COLOR_PAIR(CP_MAIN));
    wbkgd(m_SmallPanel, COLOR_PAIR(CP_SMALL_PANEL_BKGND));
    wbkgd(m_ProgressPanel, COLOR_PAIR(CP_SMALL_PANEL_2_BKGND));
    wbkgd(m_EditListPanel, COLOR_PAIR(CP_LIST_PANEL_BKGND));
    wbkgd(m_EditSummaryPanel, COLOR_PAIR(CP_SUMMARY_PANEL_BKGND));
    wbkgd(m_BigPanel, COLOR_PAIR(CP_PATTERN_LIST_PANEL));
    wbkgd(m_BigPanelExtra, COLOR_PAIR(CP_PIANO_WHITE_KEY));
}


TextUI::~TextUI()
{
    delwin(m_SmallPanel);
    delwin(m_BigPanel);
    endwin();			/* End curses mode		  */
}

int TextUI::CursesAttribute(text_attribute_t attribute)
{
    switch (attribute)
    {
        case attr_bold:
            return A_BOLD;
        case attr_underline:
            return A_UNDERLINE;
        default:
            return A_NORMAL;
    };
}

WINDOW * TextUI::AreaToWindow(window_area_t area)
{
    switch (area)
    {
        case big_panel:
            return m_BigPanel;
        case small_panel:
            return m_SmallPanel;
        case progress_panel:
            return m_ProgressPanel;
        case edit_list_panel:
            return m_EditListPanel;
        case edit_summary_panel:
            return m_EditSummaryPanel;
        case big_panel_extra:
            return m_BigPanelExtra;
        default:
            return stdscr;
    }
}

void TextUI::ClearArea(window_area_t area)
{
    WINDOW * window = AreaToWindow(area);
    wmove(window, 0, 0);
    wclrtobot(window);
    wrefresh(window);
}

void TextUI::PlaceCursor(int row, int col)
{
    move(row, col);
}

void TextUI::Highlight(window_area_t area, int row, int col, int len, int colour, text_attribute_t attr)
{
    WINDOW * window = AreaToWindow(area);
    int scr_x, scr_y;
    getyx(stdscr, scr_y, scr_x);
    mvwchgat(window, row, col, len, CursesAttribute(attr), colour, NULL);
    wrefresh(window);
    wmove(stdscr, scr_y, scr_x);
}

void TextUI::Text(window_area_t area, int row, int col, const char * text, text_attribute_t attribute)
{
    WINDOW * window = AreaToWindow(area);
    int scr_x, scr_y;
    attron(attribute);
    getyx(stdscr, scr_y, scr_x);
    mvwprintw(window, row, col, text);
    clrtoeol();
    wrefresh(window);
    attroff(attribute);
    move(scr_y, scr_x);
    refresh();
}

void TextUI::Progress(double progress, double stepWidth, double beat, int pattern_progress,
                        double rtBeat, unsigned int queueSecs, unsigned int queueNano)
{
    int row = 2;
    int col = 4;
    double width = 64;

    int n = lround(width * progress);
    int len = lround(width * stepWidth);

    if ( len < 1 )
        len = 1;

    int mode = 0;
    if ( len < 3 )
        mode = 1;
    else if ( len < 9 )
        mode = 2;

    if ( n + len > 64 )
        len = width - n;

    char lineFill, markerFill;

    switch ( mode )
    {
    case 1:
        lineFill = '-';
        markerFill = '>';
        break;
    case 2:
        lineFill = ' ';
        markerFill = '>';
        break;
    default:
        lineFill = ' ';
        markerFill = '-';
        break;

    }

    string marker(len, markerFill);
    string barline(width, lineFill);
    char buff[10];
    snprintf(buff, 10, "%7.2f ", beat /* g_State.Phase() + 1 */);
    barline += buff;

    int scr_x, scr_y;
    getyx(stdscr, scr_y, scr_x);

    switch ( mode )
    {
    case 1:
        attron(COLOR_PAIR(CP_PROGRESS_BAR_BKGND));
        mvprintw(row, col, barline.c_str());
        attron(COLOR_PAIR(CP_PROGRESS_BAR_HIGHLIGHT));
        mvprintw(row, col + n, marker.c_str());
        break;
    case 2:
        attron(COLOR_PAIR(CP_PROGRESS_BAR_BKGND));
        mvprintw(row, col, barline.c_str());
        attron(COLOR_PAIR(CP_PROGRESS_BAR_HIGHLIGHT));
        mvprintw(row, col + n, marker.c_str());
        break;
    default:
        attron(COLOR_PAIR(CP_PROGRESS_BAR_BKGND));
        mvprintw(row, col, barline.c_str());
        attron(COLOR_PAIR(CP_PROGRESS_BAR_HIGHLIGHT));
        mvprintw(row, col + n, marker.c_str());
        break;
    }

    attroff(COLOR_PAIR(CP_PROGRESS_BAR_HIGHLIGHT));

    // Extra kludge to show overall pattern chain progress.

    mvchgat(2, 4, pattern_progress /* g_PatternStore.CurrentPosPatternChain() + 1 */,
            A_UNDERLINE, CP_PROGRESS_BAR_BKGND, NULL);


    move(scr_y, scr_x);
    refresh();

    char text[80];
    snprintf(text, 80, " Beat%9.2f\n (Sec%6u:%u)", rtBeat, queueSecs, queueNano);
    Text(progress_panel, 0, 0, text);

}


void TextUI::ShowNoteTransforms(vector<InOutPair> & pairs)
{
    const int kbdStart = 29;
    WINDOW *panel = m_BigPanelExtra;
    wmove(panel, 0, 0);
    wclrtobot(panel);

    map<int, set<int>> inOutMap;

    if ( !pairs.empty() )
    {
        typedef map<int, set<int>> inOutMap_t;
        inOutMap_t inOutMap;

        for ( auto it = pairs.begin(); it != pairs.end(); it++ )
        {
//            if ( it-> m_NoteIn >= 0 )
//            {
//                mvwaddch(panel, 3, it->m_NoteIn - kbdStart, '^');
//            }

            if ( it->m_NoteOut >= 0 )
            {
                mvwaddch(panel, 1, it->m_NoteOut - kbdStart, 'o');

                // Create or update a set of output notes for this input note.

                // This fails silently if the set already exists.

                /* pair<inOutMap_t::iterator,bool> p = */ inOutMap.emplace(it->m_NoteIn, initializer_list<int>{});

                // (The previous call is supposed to return the set that was either found or
                // created but I couldn't work out how to access it using the pair above, so
                // I just do a standard lookup instead.)

                inOutMap.at(it->m_NoteIn).insert(it->m_NoteOut);

            }
        }

        // For each input note, draw a tree from its output set.
        // (Will overwrite each other at the moment.)

        for ( inOutMap_t::iterator it = inOutMap.begin(); it != inOutMap.end(); it++ )
        {
            // Input note is in it->first, output set is in it->second. There's
            // no entry created unless there's at least one output note.

            int lower = min(it->first, *(it->second.begin()));
            int upper = max(it->first, *(it->second.rbegin()));

            chtype boxChar = '*';

            if ( lower == upper )
            {
                // Single vertical bar.

                boxChar = ACS_SBSB;
                mvwaddch(panel, 2, lower - kbdStart, boxChar);
            }
            else
            {
                // Left hand end character.

                if ( lower == it->first && it->second.find(lower) != it->second.end() )
                    boxChar = ACS_SSSB;
                else if ( lower == it->first )
                    boxChar = ACS_BSSB;
                else
                    boxChar = ACS_SSBB;

                mvwaddch(panel, 2, lower - kbdStart, boxChar);

                // Right hand end character.

                if ( upper == it->first && it->second.find(upper) != it->second.end() )
                    boxChar = ACS_SBSS;
                else if ( upper == it->first )
                    boxChar = ACS_BBSS;
                else
                    boxChar = ACS_SBBS;

                mvwaddch(panel, 2, upper - kbdStart, boxChar);
            }

            // Draw the bits in between.

            for ( int i = lower + 1; i < upper; i++ )
            {
                if ( i == it->first && it->second.find(i) != it->second.end() )
                    boxChar = ACS_SSSS;
                else if ( i == it->first )
                    boxChar = ACS_BSSS;
                else if ( it->second.find(i) != it->second.end() )
                    boxChar = ACS_SSBS;
                else
                    boxChar = ACS_BSBS;

                mvwaddch(panel, 2, i - kbdStart, boxChar);
            }
        }
    }

    // Layout the keyboard pattern.

    // It seems we can't set special (box) characters without mucking
    // up the attributes - attrset() didn't have any effect - and if we
    // use mvchgat() to set an attribute over a box character, we meess
    // up the character as well.

    for ( int row = 0; row < 2; row++ )
        for ( int col = kbdStart; col < kbdStart + 72; col++ )
            switch ( col % 12 )
            {
            case 1:
            case 3:
            case 6:
            case 8:
            case 10:
                mvwchgat(panel, row, col - kbdStart, 1, 0, CP_PIANO_BLACK_KEY, NULL);
                break;
            default:
                break;
            }

    // Make the fourth row black.

    mvwchgat(panel, 3, 0, 72, A_BOLD, CP_PIANO_BLACK_KEY, NULL);

//    pairs.clear();

    wrefresh(panel);
}


void TextUI::SetTopLine(int midiChannel, double stepValue, double quantum, int runState, int midiInputMode)
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

unordered_map<int, CursorKeys::key_type_t> g_CursorKeyMap =
{
    {KEY_INSERT, CursorKeys::ins},
    {KEY_DELETE, CursorKeys::del},
    {KEY_SDELETE, CursorKeys::shift_delete},
    {KEY_CDELETE, CursorKeys::ctrl_delete},
    {KEY_TAB, CursorKeys::tab},
    {KEY_SHTAB, CursorKeys::shift_tab},
    {KEY_DOWN, CursorKeys::down},
    {KEY_UP, CursorKeys::up},
    {KEY_LEFT, CursorKeys::left},
    {KEY_RIGHT, CursorKeys::right},
    {KEY_SPGUP, CursorKeys::shift_page_up},
    {KEY_SPGDOWN, CursorKeys::shift_page_down},
    {KEY_APGUP, CursorKeys::alt_page_up},
    {KEY_APGDOWN, CursorKeys::alt_page_down},
    {KEY_CDOWN, CursorKeys::ctrl_down},
    {KEY_CUP, CursorKeys::ctrl_up},
    {KEY_CLEFT, CursorKeys::ctrl_left},
    {KEY_CRIGHT, CursorKeys::ctrl_right},
    {KEY_SDOWN, CursorKeys::shift_down},
    {KEY_SUP, CursorKeys::shift_up},
    {KEY_SLEFT, CursorKeys::shift_left},
    {KEY_SRIGHT, CursorKeys::shift_right},
    {KEY_CSLEFT, CursorKeys::ctrl_shift_left},
    {KEY_CSRIGHT, CursorKeys::ctrl_shift_right},
    {KEY_CSUP, CursorKeys::ctrl_shift_up},
    {KEY_CSDOWN, CursorKeys::ctrl_shift_down}
};

void TextUI::KeyInput(CursorKeys::key_type_t & curKey, xcb_keysym_t & sym)
{
    int c = getch();

    switch (c)
    {
    case 1: // Ctrl-A
        sym = 0xE6;
        break;
    case 3:  // Ctrl-C, Copy
        sym = 0xA2;
        break;

        // After using Alt-Enter, we seem to get additional Enter - char(10) - messages
        // on the next keypress, regardless of what that key press actually is. Disabling
        // this bit of code (probably) doesn't prevent that peculiar behaviour, but will
        // at least discourage using Alt-Enter or Esc all round.

//    case 27: // Alt-Enter sends this without a delay, otherwise it takes about a second to arrive.
//        if ( g_CursorKeys.RouteKey(CursorKeys::escape) )
//        {
//            show_status_after_navigation();
//        }
//        break;

    case 22: // Ctrl-V, Paste
        sym = 0xAD2;
        break;

    case 10: // Enter (Ctl-J *and* Ctl-M will also fire this one.)
        sym = XK_Return;
        break;

    case 32: // Space bar.
        sym = XK_space;
        break;

    case KEY_TAB:
        sym = XK_Tab;
        break;

    case KEY_SHTAB:
        sym = XK_ISO_Left_Tab;
        break;

    case KEY_SPGUP:
    case KEY_SPGDOWN:
    case KEY_APGUP:
    case KEY_APGDOWN:
    case KEY_CDOWN:
    case KEY_CUP:
    case KEY_CLEFT:
    case KEY_CRIGHT:
    case KEY_SDOWN:
    case KEY_SUP:
    case KEY_SLEFT:
    case KEY_SRIGHT:
    case KEY_CSLEFT:
    case KEY_CSRIGHT:
    case KEY_CSUP:
    case KEY_CSDOWN:
    case KEY_DOWN:
    case KEY_UP:
    case KEY_LEFT:
    case KEY_RIGHT:
    case KEY_INSERT:
    case KEY_DELETE:
    case KEY_SDELETE:
    case KEY_CDELETE:
        curKey = g_CursorKeyMap.at(c);
        break;

    case KEY_BACKSPACE: // 263
        sym = XK_BackSpace;
        break;

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
        sym = c;
        break;
    }

}
