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

#ifndef MATEXTUI_H_INCLUDED
#define MATEXTUI_H_INCLUDED

#include <vector>

#include "maBaseUI.h"

//#include <ncurses.h>

// Extra keys that aren't in ncurses.h

#define KEY_INSERT  331
#define KEY_DELETE  330
#define KEY_SDELETE 383
#define KEY_CDELETE 519
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

#define KEY_TAB     9
#define KEY_SHTAB    353 // "KEY_STAB", my preferred name, is already defined in /usr/include/curses.h as "0524", /* set-tab key */"


// These are no different to plain PGUP/PGDOWN

#define KEY_CPGUP
#define KEY_CPGDOWN

#define KEY_APGUP   553
#define KEY_APGDOWN 548

// Color Pair uses

enum colour_pairs
{
    CP_MAIN = 1,
    CP_PATTERN_LIST_PANEL,
    CP_PATTERN_LIST_PANEL_HIGHLIGHT,
    CP_PATTERN_LIST_PANEL_HIGHLIGHT_TRIG,
    CP_PIANO_WHITE_KEY,
    CP_PIANO_BLACK_KEY,
    CP_PROGRESS_BAR_HIGHLIGHT,
    CP_PROGRESS_BAR_BKGND,
    CP_SMALL_PANEL_BKGND,
    CP_SMALL_PANEL_2_BKGND,
    CP_PATTERN_CHAIN_HIGHLIGHT,
    CP_LIST_PANEL_BKGND,
    CP_SUMMARY_PANEL_BKGND,
    CP_MENU_HIGHLIGHT,
    CP_RUNNING,
    CP_RECORD,
    CP_REALTIME
};

#if !defined(CURSES_H)
struct WINDOW;
#endif

class TextUI : public BaseUI
{
    public:

        TextUI();
        ~TextUI();

        WINDOW * AreaToWindow(window_area_t area);
        int CursesAttribute(text_attribute_t attribute);

        virtual void Text(window_area_t area, int row, int col, const char * text, text_attribute_t attribute = attr_normal);
        void Highlight(window_area_t area, int row, int col, int len, int colour, text_attribute_t attr = attr_normal);
        void ClearArea(window_area_t area);
        void PlaceCursor(int row, int col);

        void SetTopLine(int midiChannel, double stepValue, double quantum, int runState, int midiMode);

        virtual void Progress(double progress, double stepWidth, double beat,
                int pattern_progress, double rtBeat, unsigned int queueSecs, unsigned int queueNano);
        void ShowNoteTransforms(std::vector<InOutPair> & pairs);

    private:
        WINDOW * m_BigPanel = NULL;             // Botton half of screen, used for all sorts.
        WINDOW * m_SmallPanel = NULL;           // Pattern Status, or pattern chain. Under progress bar, to the left.
        WINDOW * m_ProgressPanel = NULL;        // Extra progress panel, to the right of Pattern Status
        WINDOW * m_EditListPanel = NULL;        // Pattern List, under command line.
        WINDOW * m_EditSummaryPanel = NULL;     // Pattern Summary, under command line to the right of Pattern List
        WINDOW * m_BigPanelExtra = NULL;        // Overwrites pattern list and summary when in Big Panel is in Page 3 mode.

};

#endif // MATEXTUI_H_INCLUDED
