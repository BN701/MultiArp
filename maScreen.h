//
//    MultiArp - Another step in the Great Midi Adventure!
//    Copyright (C) 2017  Barry Neilsen
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

#ifndef MASCREEN_H_INCLUDED
#define MASCREEN_H_INCLUDED

#include <ncurses.h>

// Extra keys that aren't in ncurses.h

#define KEY_CLEFT   545
#define KEY_CRIGHT  560
#define KEY_CUP     525
#define KEY_CDOWN   566

#define KEY_CSLEFT   546
#define KEY_CSRIGHT  561
#define KEY_CSUP     567
#define KEY_CSDOWN   526

#define KEY_ARIGHT  558
#define KEY_ALEFT   543
#define KEY_AUP     564
#define KEY_ADOWN   523

#define KEY_ASRIGHT  559
#define KEY_ASLEFT   544
#define KEY_ASUP     565
#define KEY_ASDOWN   524

#define KEY_SUP     337
#define KEY_SDOWN   336

#define KEY_PGUP    339
#define KEY_PGDOWN  338

#define KEY_SPGUP   398
#define KEY_SPGDOWN 396

// These are no different to plain PGUP/PGDOWN

#define KEY_CPGUP
#define KEY_CPGDOWN

#define KEY_APGUP   553
#define KEY_APGDOWN 548

// Screen locations

#define COMMAND_HOME 6,4
#define STAT_POS_TOPLINE 0,0
#define STAT_POS_STEP gDisplay.SmallPanel(), 0,0
#define STAT_POS_PATTERN gDisplay.SmallPanel(), 1,0
#define STAT_POS_2 5,4



class Display
{
public:
    Display();
    ~Display();

    WINDOW *BigPanel()
    {
        return m_BigPanel;
    }

    WINDOW *SmallPanel()
    {
        return m_SmallPanel;
    }
private:
    WINDOW * m_BigPanel = NULL;
    WINDOW * m_SmallPanel = NULL;

};


/*
    This whole new variable args list thing is
    apparently C++11. I've no idea how the template
    statement works, it may as well be magic.

    I found it here in one of the less popular answers, here:
    https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf

    This page explicitly states that variadic templates are *not* magic (it's recursion):
    https://eli.thegreenplace.net/2014/variadic-templates-in-c/

    I get linker errors unless I define the body of the function here
    in the header.

    https://stackoverflow.com/questions/10632251/undefined-reference-to-template-function
 */

template<typename ... Args>
void set_status(int y, int x, const char *format, Args ... args)
{
    int scr_x, scr_y;
    attron(A_BOLD);			/* bold on */
    getyx(stdscr, scr_y, scr_x);
    mvprintw(y, x, format, args ...);
    clrtoeol();
    move(scr_y, scr_x);
    refresh();
    attroff(A_BOLD);			/* bold on */
}

template<typename ... Args>
void set_status_w(WINDOW * w, int y, int x, const char *format, Args ... args)
{
    int scr_x, scr_y;
    getyx(stdscr, scr_y, scr_x);
    mvwprintw(w,y, x, format, args ...);
    wclrtoeol(w);
    wmove(stdscr, scr_y, scr_x);
    wrefresh(w);
    refresh();
}


void set_top_line();
void show_status();
void show_translation_status();
void show_translation_map_status();
void show_listbuilder_status();
void show_status_after_navigation();
void update_progress_bar();

void update_pattern_panel();
void highlight_pattern_panel();

void highlight(int base_row, int base_col, int ofs, int len, int attr, int colour = 0);


#endif // MASCREEN_H_INCLUDED
