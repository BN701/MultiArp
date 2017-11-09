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

#include "maScreen.h"

#include "maState.h"
#include "maUtility.h"
#include "maAlsaSequencer.h"
#include "maAlsaSequencerQueue.h"
#include "maListBuilder.h"
#include "maPatternStore.h"
#include "maTranslateTable.h"

using namespace std;

// Global ALSA Sequencer instance.

extern AlsaSequencer g_Sequencer;
extern CursorKeys g_CursorKeys;
extern ListBuilder g_ListBuilder;
extern PatternStore g_PatternStore;
extern State g_State;
//extern TranslateTable * pTranslateTable;

// Additional colours beyond the basic eight that are defined
// in ncurses.h. (Note the UK spelling to distinguish them from
// the defaults, which aren't changed.)

// (I haven't found documentation that says it's safe to use colour
// definition numbers beyond the first 0..7. It just works, so I'm
// using them for now.)

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

// Color Pair uses

enum colour_pairs
{
    CP_MAIN = 1,
    CP_PATTERN_LIST_PANEL,
    CP_PATTERN_LIST_PANEL_2,
    CP_PATTERN_LIST_PANEL_3,
    CP_PATTERN_LIST_PANEL_BKGND,
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


Display::Display()
{
    // Set up NCurses.

    initscr();
    raw();
    keypad(stdscr, true);

    start_color();

    init_color(COLOUR_GREEN, 0, 400, 0);
    init_color(COLOUR_BRIGHT_GREEN, 0, 750, 0);
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
	init_pair(CP_PATTERN_LIST_PANEL_2, COLOUR_BLUE, COLOUR_BLACK);
	init_pair(CP_PATTERN_LIST_PANEL_3, COLOUR_REDDISH, COLOUR_BLACK);
	init_pair(CP_PATTERN_LIST_PANEL_BKGND, COLOR_YELLOW, COLOUR_GREY);

    init_pair(CP_LIST_PANEL_BKGND, COLOUR_GREY, COLOUR_BLACK);
    init_pair(CP_SUMMARY_PANEL_BKGND, COLOUR_GREY, COLOR_BLACK);
    init_pair(CP_SMALL_PANEL_BKGND, COLOR_YELLOW, COLOUR_BLACK);
    init_pair(CP_SMALL_PANEL_2_BKGND, COLOUR_GREY, COLOR_BLACK);
    init_pair(CP_PATTERN_CHAIN_HIGHLIGHT, COLOR_YELLOW, COLOUR_GREEN);

    init_pair(CP_PROGRESS_BAR_BKGND, COLOUR_GREY, COLOR_BLACK);
    init_pair(CP_PROGRESS_BAR_HIGHLIGHT, COLOUR_WHITE, COLOR_BLACK);

    mvprintw(6, 1, "=> ");

    m_SmallPanel = newwin(4, 56, 2, 4);
    m_ProgressPanel = newwin(2, 15, 3, 61);
    m_EditListPanel = newwin(4, 20, 8, 4);
    m_EditSummaryPanel = newwin(4, 52, 8, 24);
    m_BigPanel = newwin(10, 80, 15, 0);

    bkgd(COLOR_PAIR(CP_MAIN));
    wbkgd(m_SmallPanel, COLOR_PAIR(CP_SMALL_PANEL_BKGND));
    wbkgd(m_ProgressPanel, COLOR_PAIR(CP_SMALL_PANEL_2_BKGND));
    wbkgd(m_EditListPanel, COLOR_PAIR(CP_LIST_PANEL_BKGND));
    wbkgd(m_EditSummaryPanel, COLOR_PAIR(CP_SUMMARY_PANEL_BKGND));
    wbkgd(m_BigPanel, COLOR_PAIR(CP_MAIN));
}


Display::~Display()
{
    delwin(m_SmallPanel);
    delwin(m_BigPanel);
    endwin();			/* End curses mode		  */
}

Display g_Display;

void highlight(int base_row, int base_col, int ofs, int len, int attr, int colour)
{
    int scr_x, scr_y;
    getyx(stdscr, scr_y, scr_x);
    mvchgat(base_row, base_col + ofs, len, attr, colour, NULL);
    move(scr_y, scr_x);
    refresh();
}

void set_top_line()
{
    set_status(STAT_POS_TOPLINE, "Multi Arp - Midi:%02i, Step:%5.2f, Link Quantum:%5.2f     %s",
               g_Sequencer.MidiChannel() + 1,
               g_State.CurrentStepValue(),
               g_State.Quantum(),
               g_State.RunState() ? "<<   RUN   >>" : "<<   ---   >>");

    highlight(0, 0, 0, 80, A_BOLD, g_ListBuilder.MidiInputModeAsColour(vector<int> {CP_MAIN, CP_RECORD, CP_RECORD, CP_REALTIME})); // Hmm ...
    highlight(0, 0, 60, 5, A_BOLD, g_State.RunState() ? CP_RUNNING : CP_MAIN);
}

void update_progress_bar()
{
    int row = 2;
    int col = 4;
    double width = 64;


    double progress, stepWidth;
    g_State.Progress(progress, stepWidth);

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
        lineFill = '-';
        markerFill = '=';
        break;
    default:
        lineFill = ' ';
        markerFill = '-';
        break;

    }

    string marker(len, markerFill);
    string barline(width, lineFill);
    char buff[10];
    sprintf(buff, "%7.2f ", g_State.Phase() + 1);
    barline += buff;

    int scr_x, scr_y;
    getyx(stdscr, scr_y, scr_x);

    switch ( mode )
    {
    case 1:
        attron(COLOR_PAIR(CP_PROGRESS_BAR_BKGND));
        mvprintw(row, col, barline.c_str());
        attron(COLOR_PAIR(CP_PROGRESS_BAR_HIGHLIGHT));
//        mvaddch(row, col + n, ACS_RARROW);
//        mvaddch(row, col + n, '>');
        mvprintw(row, col + n, marker.c_str());
        break;
    case 2:
        attron(COLOR_PAIR(CP_PROGRESS_BAR_BKGND));
        mvprintw(row, col, barline.c_str());
        attron(COLOR_PAIR(CP_PROGRESS_BAR_HIGHLIGHT));
        mvprintw(row, col + n, marker.c_str());
        break;
    default:
//        move(row, col);
//        clrtoeol();
        attron(COLOR_PAIR(CP_PROGRESS_BAR_BKGND));
        mvprintw(row, col, barline.c_str());
        attron(COLOR_PAIR(CP_PROGRESS_BAR_HIGHLIGHT));
        mvprintw(row, col + n, marker.c_str());
        break;
    }

    attroff(COLOR_PAIR(CP_PROGRESS_BAR_HIGHLIGHT));

    move(scr_y, scr_x);
    refresh();

}

void update_pattern_status_panel()
{
    wmove(g_Display.SmallPanel(), 1, 0);
    wclrtoeol(g_Display.SmallPanel());
    wmove(g_Display.SmallPanel(), 2, 0);
    wclrtoeol(g_Display.SmallPanel());

    if ( g_PatternStore.PatternChainMode() == PC_MODE_NONE ||
         g_PatternStore.Empty() ||
         g_PatternStore.PatternChainEmpty() )
    {
        set_status_w(STAT_POS_PATTERN, g_PatternStore.PatternStatus().c_str());
        return;
    }

    static int firstRow = 0;
    const int rows = 2;

    int selection = g_PatternStore.CurrentPosPatternChain();

    while ( selection >= 4 * (firstRow + rows) )
        firstRow += 1;

    while ( selection < 4 * firstRow )
        firstRow -= 1;

    int rowSelect = selection/4 - firstRow;

    int scr_x, scr_y;
    getyx(stdscr, scr_y, scr_x);

    mvwprintw(g_Display.SmallPanel(), 1, 0, g_PatternStore.PatternChainToStringForDisplay(firstRow, rows).c_str());
    mvwchgat(g_Display.SmallPanel(), 1 + rowSelect, 5 + 12 * (selection % 4), 12, 0, CP_PATTERN_CHAIN_HIGHLIGHT, NULL);

    wrefresh(g_Display.SmallPanel());
    move(scr_y, scr_x);
    refresh();
}

void update_edit_panels(bool refreshList)
{
    if ( g_PatternStore.Empty() )
    {
        wmove(g_Display.EditListPanel(), 0, 0);
        wclrtobot(g_Display.EditListPanel());
        wrefresh(g_Display.EditListPanel());
        wmove(g_Display.EditSummaryPanel(), 0, 0);
        wclrtobot(g_Display.EditSummaryPanel());
        wrefresh(g_Display.EditSummaryPanel());
        return;
    }

    // Selection List

    static int listStart = 0;
    const int rows = 4;

    int selection = g_PatternStore.CurrentPosEdit();

    while ( selection >= listStart + rows )
        listStart += 1;

    while ( selection < listStart )
        listStart -= 1;

    wmove(g_Display.EditListPanel(), 0, 0);
    wclrtobot(g_Display.EditListPanel());

    wprintw(g_Display.EditListPanel(), g_PatternStore.PatternSelectionList(listStart, rows).c_str());
    mvwchgat(g_Display.EditListPanel(), selection - listStart, 0, 20, 0, CP_SUMMARY_PANEL_BKGND, NULL);

    wrefresh(g_Display.EditListPanel());

    // Summary Panel

    wmove(g_Display.EditSummaryPanel(), 0, 0);
    wclrtobot(g_Display.EditSummaryPanel());

    Pattern & p = g_PatternStore.CurrentEditPattern();

    wmove(g_Display.EditSummaryPanel(), 0, 1);
    wprintw(g_Display.EditSummaryPanel(), "List(s) %i, Real Time %i", p.StepListCount(), p.RealTimeListCount());

    wmove(g_Display.EditSummaryPanel(), 1, 1);
    wprintw(g_Display.EditSummaryPanel(), "Step value %.2f, Vel %i, Gate %.0f%% (Hold %s)", p.StepValue(),
        p.Velocity(), p.Gate() * 100, p.GateHold() ? "on" : "off");

    TranslateTable & table = p.PatternTranslateTable();

    wmove(g_Display.EditSummaryPanel(), 2, 1);
    wprintw(g_Display.EditSummaryPanel(), "Chromatic %i, Tonal %i (%s), %s-%s",
            table.Transpose(),
            table.DegreeShift(),
            table.ShiftName(),
            table.RootName().c_str(),
            table.ScaleName());

    wmove(g_Display.EditSummaryPanel(), 3, 1);
    wprintw(g_Display.EditSummaryPanel(), "Premap %s, Accidentals %s",
            table.PremapModeName(),
            table.AccidentalsModeName());

    wrefresh(g_Display.EditSummaryPanel());
}

std::vector<int> g_ListDisplayRows;

void update_pattern_panel()
{
    int scr_x, scr_y;

    wmove(g_Display.BigPanel(), 0, 0);
    wclrtobot(g_Display.BigPanel());

	wattron(g_Display.BigPanel(), COLOR_PAIR(CP_PATTERN_LIST_PANEL_2));

    g_ListDisplayRows.clear();

    for ( int i = 0; i < g_PatternStore.RealTimeListCount(); i++ )
    {
        wprintw(g_Display.BigPanel(), "%s\n", g_PatternStore.RealTimeListToStringForDisplay(i).c_str());
    }

    wprintw(g_Display.BigPanel(), "\n");
	wattron(g_Display.BigPanel(), COLOR_PAIR(CP_PATTERN_LIST_PANEL));

    for ( int i = 0; i < g_PatternStore.PlayPatternListCount(); i++ )
    {
        getyx(g_Display.BigPanel(), scr_y, scr_x);
        g_ListDisplayRows.push_back(scr_y);
        wprintw(g_Display.BigPanel(), "%s\n", g_PatternStore.PlayPatternListToString(i).c_str());
    }

    wattroff(g_Display.BigPanel(), COLOR_PAIR(CP_PATTERN_LIST_PANEL));

    wrefresh(g_Display.BigPanel());

}

void highlight_pattern_panel()
{
/*
    Clear previous highlist.

    Get list number and highlight info from g_PatternStore.

    Translate to screen location for highlight.

    How to allow for lines that wrap?
        - Probably OK for first List = 1.
        - But need to account for any wraps on previous lists when updating List > 1.

    Update attributes.
*/
    static std::vector<int> clearPositions;

    while ( clearPositions.size() >= 3 )
    {
        int lastLength = clearPositions.back();
        clearPositions.pop_back();
        int lastOffset = clearPositions.back();
        clearPositions.pop_back();
        int lastRow = clearPositions.back();
        clearPositions.pop_back();

        mvwchgat(g_Display.BigPanel(), lastRow, lastOffset, lastLength, 0, 1, NULL);
    }

    int listIndex, offset, length;

    if ( g_PatternStore.PlayPositionInfo(listIndex, offset, length) )
    {
        int row = g_ListDisplayRows.at(listIndex);

        // TODO: (Sort of.) This is a hack to cope with the fact that the pattern now inserts a simple
        //       arrow at the start of each row to show selection. The 'big panel' now uses the full width
        //       of the screen, but the play list ToString() routine still store's highlight positions
        //       relative to the beginning of the string it creates, which doesn't include the selection
        //       arrow. If we keep this flickering highlight mechanism, we need to make this better.

        offset += 4;

        while ( offset >= 76 )
        {
            row += 1;
            offset -= 76;
        }

        if ( (offset + length) >= 76 )
        {
            int length0 = 76 - offset;
            mvwchgat(g_Display.BigPanel(), row, offset, length0, A_BOLD, 1, NULL);

            clearPositions.push_back(row);
            clearPositions.push_back(offset);
            clearPositions.push_back(length0);

            row += 1;
            offset = 0;
            length -= length0;
        }

        mvwchgat(g_Display.BigPanel(), row, offset, length, A_REVERSE, 1, NULL);

        clearPositions.push_back(row);
        clearPositions.push_back(offset);
        clearPositions.push_back(length);
    }

    wrefresh(g_Display.BigPanel());
}



void show_status()
{
    set_status(STAT_POS_2, g_PatternStore.PatternOverview().c_str());
}

void show_status_after_navigation()
{
    const int width = 72;

    // Call this after any change to focus or navigation involving
    // objects derived from CursorKeys. All objects share the same
    // static pointer to the object in focus, and calling Status()
    // on any of them will retrieve the status string for the object
    // that currently has focus.

    static int adjustOffset = 0;
    string status = g_CursorKeys.Status();

    // Although I started off with a mechanism which allows for multiple
    // highlights, so far I've only ever set one at a time. Now that I
    // need to handle long strings and need to make sure the highlight
    // is visible I'm relying on there only being one highlight and using
    // that to position the whole string within the available width. Any
    // highlights set after the first may end up off screen.

    if ( status.size() < width )
        adjustOffset = 0;

    vector<screen_pos_t> & highlights = g_CursorKeys.GetHighlights();

    if ( !highlights.empty() )
    {
        screen_pos_t cursor = highlights.front();

        if ( g_CursorKeys.FirstField() )
        {
            // Make sure whole string scrolls into view on the left.
            cursor.length += cursor.offset;
            cursor.offset = 0;
        }

        if ( cursor.offset < adjustOffset )
            adjustOffset = cursor.offset;
        else if ( cursor.offset + cursor.length - adjustOffset >= width )
            adjustOffset = cursor.offset + cursor.length - width;
    }

    set_status(STAT_POS_MENU, status.substr(adjustOffset, width).c_str());

    for ( size_t i = 0; i < highlights.size(); i++ )
        highlight(STAT_POS_MENU, highlights.at(i).offset - adjustOffset, highlights.at(i).length, A_BOLD, CP_MENU_HIGHLIGHT);

}

void show_translation_map_status()
{
    set_status(STAT_POS_2, "I: %s     Map: %s", g_PatternStore.TranslateTableForEdit(false).ShowScale().c_str(), g_PatternStore.TranslateTableForEdit(false).ShowNoteMap().c_str());
}

void show_translation_status()
{
    set_status(STAT_POS_2, g_PatternStore.TranslateTableForEdit().Status().c_str());
}


void show_listbuilder_status()
{
    std::string temp = g_ListBuilder.ToString();
    int len = temp.size();
    if ( len > 60 )
        set_status(STAT_POS_2, "%c: ...%s", g_ListBuilder.MidiInputModeAsChar(), temp.c_str() + len - 60 );
    else
        set_status(STAT_POS_2, "%c:%s", g_ListBuilder.MidiInputModeAsChar(), temp.c_str());
}
