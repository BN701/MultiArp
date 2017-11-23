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
    CP_PATTERN_LIST_PANEL_HIGHLIGHT,
    CP_PATTERN_LIST_PANEL_HIGHLIGHT_TRIG,
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
    wbkgd(m_BigPanelExtra, COLOR_PAIR(CP_SUMMARY_PANEL_BKGND));
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

    // Extra kludge to show overall pattern chain progress.

    mvchgat(2, 4, g_PatternStore.CurrentPosPatternChain() + 1,
            A_UNDERLINE, CP_PROGRESS_BAR_BKGND, NULL);

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
    switch ( g_Display.BigPanelPage() )
    {
    case Display::one:
    case Display::two:
        break;
    default:
        return;
    }

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

void layout_pattern_extra_panel()
{
    wmove(g_Display.BigPanelExtra(), 0, 0);
    wclrtobot(g_Display.BigPanelExtra());

    for ( int row = 0; row < 2; row++ )
        for ( int col = 0; col < 72; col++ )
            switch ( col % 12 )
            {
            case 1:
            case 3:
            case 6:
            case 8:
            case 10:
                mvwchgat(g_Display.BigPanelExtra(), row, col, 1, 0, CP_PATTERN_LIST_PANEL_HIGHLIGHT, NULL);
                break;
            default:
                break;
            }

    wrefresh(g_Display.BigPanelExtra());
}

void update_pattern_panel()
{
    int scr_x, scr_y;

    if ( g_Display.BigPanelHold() )
        return;

    wmove(g_Display.BigPanel(), 0, 0);
//    wclrtobot(g_Display.BigPanel());

    try
    {
        bool showTrigProgress = false;
        static vector<PosInfo2> highlights; // Reset every for every update for pages 1 & 2, persist for page 3.

        switch ( g_Display.BigPanelPage() )
        {
        case Display::one:
            wclrtobot(g_Display.BigPanel());
            wprintw(g_Display.BigPanel(), g_PatternStore.CurrentPlayPattern().Display(2, highlights, 25, 79).c_str());
            showTrigProgress = true;
            break;
        case Display::two:
            wclrtobot(g_Display.BigPanel());
            wprintw(g_Display.BigPanel(), g_PatternStore.CurrentPlayPattern().Display(1, highlights, 25, 79).c_str());
            showTrigProgress = true;
            break;
        case Display::three:
            layout_pattern_extra_panel();
            wprintw(g_Display.BigPanel(), g_PatternStore.TranslateTableForPlay().Diags().Log(highlights).c_str());
            break;
        default:
            break;
        }

        switch ( g_Display.BigPanelPage() )
        {
        case Display::one:
        case Display::two:
            for ( auto it = highlights.begin(); it < highlights.end(); it++ )
            {
                mvwchgat(g_Display.BigPanel(), it->row, it->offset, it->length, 0,
                    it->row < 2 ? CP_PATTERN_LIST_PANEL_HIGHLIGHT_TRIG : CP_PATTERN_LIST_PANEL_HIGHLIGHT,
                    NULL);
            }
            highlights.clear(); // Clear highlights here in case page switches before next update.
            break;
        case Display::three:
            // Row highlighting here depends on the current row position inserted at the front of the list.
            mvwchgat(g_Display.BigPanel(), highlights.back().row, highlights.back().offset, highlights.back().length, 0, CP_PATTERN_LIST_PANEL, NULL);
            mvwchgat(g_Display.BigPanel(), highlights.front().row, highlights.front().offset, highlights.front().length, 0, CP_PATTERN_LIST_PANEL_HIGHLIGHT_TRIG, NULL);
            if ( highlights.size() > 1 )
                highlights.pop_back();
            break;
        default:
            break;
        }

        // Kludge to show overall trig position.

        if ( showTrigProgress )
            mvwchgat(g_Display.BigPanel(), 0, 4, g_PatternStore.CurrentPlayPattern().TrigPlayPosition() + 1,
                A_UNDERLINE, CP_PATTERN_LIST_PANEL, NULL);
    }
    catch (... /*string s*/)
    {
        // wprintw(g_Display.BigPanel(), s.c_str());
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
