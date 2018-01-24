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

#include <set>


#include "maScreen.h"

#include "maAlsaSequencer.h"
#include "maAlsaSequencerQueue.h"
#include "maListBuilder.h"
#include "maPatternStore.h"
#include "maState.h"
#include "maTranslateTable.h"
#include "maUtility.h"

using namespace std;

// Global ALSA Sequencer instance.

extern AlsaSequencer g_Sequencer;
extern CursorKeys g_CursorKeys;
extern ListBuilder g_ListBuilder;
extern PatternStore g_PatternStore;
extern State g_State;

TextUI g_TextUI;
CairoUI g_CairoUI;



//// Additional colours beyond the basic eight that are defined
//// in ncurses.h. (Note the UK spelling to distinguish them from
//// the defaults, which aren't changed.)
//
//// (I haven't found documentation that says it's safe to use colour
//// definition numbers beyond the first 0..7. It just works, so I'm
//// using them for now.)
//
//#define COLOUR_GREEN        8
//#define COLOUR_RED          9
//#define COLOUR_BLUE         10
//#define COLOUR_GREY         11
//#define COLOUR_BRIGHT_RED   12
//#define COLOUR_REDDISH      13
//#define COLOUR_BRIGHT_GREEN 14
//#define COLOUR_YELLOW       16
//#define COLOUR_DARK_GREY    17
//#define COLOUR_DARKER_GREY  18
//#define COLOUR_WHITE        19
//#define COLOUR_BLACK        20
//
//// Color Pair uses
//
//enum colour_pairs
//{
//    CP_MAIN = 1,
//    CP_PATTERN_LIST_PANEL,
//    CP_PATTERN_LIST_PANEL_HIGHLIGHT,
//    CP_PATTERN_LIST_PANEL_HIGHLIGHT_TRIG,
//    CP_PIANO_WHITE_KEY,
//    CP_PIANO_BLACK_KEY,
//    CP_PROGRESS_BAR_HIGHLIGHT,
//    CP_PROGRESS_BAR_BKGND,
//    CP_SMALL_PANEL_BKGND,
//    CP_SMALL_PANEL_2_BKGND,
//    CP_PATTERN_CHAIN_HIGHLIGHT,
//    CP_LIST_PANEL_BKGND,
//    CP_SUMMARY_PANEL_BKGND,
//    CP_MENU_HIGHLIGHT,
//    CP_RUNNING,
//    CP_RECORD,
//    CP_REALTIME
//};
//
//
//TextUI::TextUI()
//{
//    // Set up NCurses.
//
//    initscr();
//    raw();
//    keypad(stdscr, true);
//
//    start_color();
//
//    init_color(COLOUR_GREEN, 0, 400, 0);
//    init_color(COLOUR_BRIGHT_GREEN, 400, 750, 400);
//    init_color(COLOUR_YELLOW, 750, 500, 0);
//    init_color(COLOUR_RED, 750, 0, 0);
//    init_color(COLOUR_REDDISH, 750, 300, 200);
//    init_color(COLOUR_BLUE, 250, 750, 900);
//    init_color(COLOUR_BRIGHT_RED, 1000, 0, 0);
//    init_color(COLOUR_GREY, 750, 750, 750);
//    init_color(COLOUR_DARK_GREY, 500, 500, 500);
//    init_color(COLOUR_DARKER_GREY, 250, 250, 250);
//
//    init_color(COLOUR_WHITE, 1000, 1000, 1000);
//    init_color(COLOUR_BLACK, 0, 0, 0);
//
//	init_pair(CP_MAIN, COLOUR_WHITE, COLOUR_BLACK);
//	init_pair(CP_RUNNING, COLOUR_WHITE, COLOUR_BRIGHT_GREEN);
//	init_pair(CP_REALTIME, COLOUR_WHITE, COLOUR_RED);
//	init_pair(CP_RECORD, COLOUR_WHITE, COLOUR_YELLOW);
//	init_pair(CP_MENU_HIGHLIGHT, COLOUR_WHITE, COLOUR_REDDISH);
//
//	init_pair(CP_PATTERN_LIST_PANEL, COLOUR_BRIGHT_GREEN, COLOUR_BLACK);
//	init_pair(CP_PATTERN_LIST_PANEL_HIGHLIGHT, COLOUR_WHITE, COLOUR_BLACK);
//	init_pair(CP_PATTERN_LIST_PANEL_HIGHLIGHT_TRIG, COLOUR_BRIGHT_GREEN, COLOUR_DARKER_GREY);
//
//	init_pair(CP_PIANO_WHITE_KEY, COLOUR_GREY, COLOUR_DARKER_GREY);
//	init_pair(CP_PIANO_BLACK_KEY, COLOUR_GREY, COLOUR_BLACK);
//
//    init_pair(CP_LIST_PANEL_BKGND, COLOUR_GREY, COLOUR_BLACK);
//    init_pair(CP_SUMMARY_PANEL_BKGND, COLOUR_GREY, COLOR_BLACK);
//    init_pair(CP_SMALL_PANEL_BKGND, COLOR_YELLOW, COLOUR_BLACK);
//    init_pair(CP_SMALL_PANEL_2_BKGND, COLOUR_GREY, COLOUR_BLACK);
//    init_pair(CP_PATTERN_CHAIN_HIGHLIGHT, COLOR_YELLOW, COLOUR_DARKER_GREY);
//
//    init_pair(CP_PROGRESS_BAR_BKGND, COLOUR_GREY, COLOUR_BLACK);
//    init_pair(CP_PROGRESS_BAR_HIGHLIGHT, COLOUR_WHITE, COLOUR_BLACK);
//
//    mvprintw(6, 1, "=> ");
//
//    m_SmallPanel = newwin(4, 56, 2, 4);
//    m_ProgressPanel = newwin(2, 15, 3, 61);
//    m_EditListPanel = newwin(4, 20, 8, 4);
//    m_EditSummaryPanel = newwin(4, 52, 8, 24);
//    m_BigPanel = newwin(11, 80, 12, 0);
//    m_BigPanelExtra = newwin(4, 72, 8, 4);
//
//    bkgd(COLOR_PAIR(CP_MAIN));
//    wbkgd(m_SmallPanel, COLOR_PAIR(CP_SMALL_PANEL_BKGND));
//    wbkgd(m_ProgressPanel, COLOR_PAIR(CP_SMALL_PANEL_2_BKGND));
//    wbkgd(m_EditListPanel, COLOR_PAIR(CP_LIST_PANEL_BKGND));
//    wbkgd(m_EditSummaryPanel, COLOR_PAIR(CP_SUMMARY_PANEL_BKGND));
//    wbkgd(m_BigPanel, COLOR_PAIR(CP_PATTERN_LIST_PANEL));
//    wbkgd(m_BigPanelExtra, COLOR_PAIR(CP_PIANO_WHITE_KEY));
//}
//
//
//TextUI::~TextUI()
//{
//    delwin(m_SmallPanel);
//    delwin(m_BigPanel);
//    endwin();			/* End curses mode		  */
//}
//
//void TextUI::NextBigPanelPage( int direction )
//{
//    if ( direction > 0 )
//    {
//        m_BigPanelPage = static_cast<big_panel_page_t>(m_BigPanelPage + 1);
//        if ( m_BigPanelPage == num_big_panel_pages )
//            m_BigPanelPage = one;
//    }
//    else
//    {
//        m_BigPanelPage = static_cast<big_panel_page_t>(m_BigPanelPage - 1);
//        if ( m_BigPanelPage < one )
//            m_BigPanelPage = static_cast<big_panel_page_t>(num_big_panel_pages - 1);
//    }
//}
//
//void TextUI::Text(window_area_t area, int row, int col, const char * text, int attribute)
//{
//    WINDOW * window = stdscr;
//
//    switch (area)
//    {
//        case big_panel:
//            window = m_BigPanel;
//            break;
//        case small_panel:
//            window = m_SmallPanel;
//            break;
//        case progress_panel:
//            window = m_ProgressPanel;
//            break;
//        case edit_list_panel:
//            window = m_EditListPanel;
//            break;
//        case edit_summary_panel:
//            window = m_EditSummaryPanel;
//            break;
//        case big_panel_extra:
//            window = m_BigPanelExtra;
//            break;
//        default:
//            break;
//    }
//
//    int scr_x, scr_y;
//    attron(attribute);
//    getyx(stdscr, scr_y, scr_x);
//
//    int result = mvwprintw(window, row, col, text);
//
//    clrtoeol();
//    move(scr_y, scr_x);
//    refresh();
//    attroff(attribute);
//}

void place_cursor(int row, int col)
{
    g_TextUI.PlaceCursor(row, col);
}

void set_status(int y, int x, const char *format, ...)
{
    char text[100];
    va_list args;
    va_start(args, format);
    vsnprintf(text, 100, format, args);
    va_end(args);

    g_TextUI.Text(BaseUI::whole_screen, y, x, text, BaseUI::attr_normal);
    g_CairoUI.Text(BaseUI::whole_screen, y, x, text, BaseUI::attr_normal);

//    int scr_x, scr_y;
//    attron(A_BOLD);			/* bold on */
//    getyx(stdscr, scr_y, scr_x);
//
//    mvprintw(y, x, text);
//
//    clrtoeol();
//    move(scr_y, scr_x);
//    refresh();
//    attroff(A_BOLD);			/* bold on */
}

//void set_status_w(WINDOW * w, int y, int x, const char *format, ...)
void set_status_w(TextUI::window_area_t area, int y, int x, const char *format, ...)
{
    char text[100];
    va_list args;
    va_start(args, format);
    vsnprintf(text, 100, format, args);
    va_end(args);

    g_TextUI.Text(area, y, x, text);

//    int scr_x, scr_y;
//    getyx(stdscr, scr_y, scr_x);
//    mvwprintw(w,y, x, text);
//    wclrtoeol(w);
//    wmove(stdscr, scr_y, scr_x);
//    wrefresh(w);
//    refresh();
}


//void highlight(int base_row, int base_col, int ofs, int len, int attr, int colour)
//{
//    int scr_x, scr_y;
//    getyx(stdscr, scr_y, scr_x);
//    mvchgat(base_row, base_col + ofs, len, attr, colour, NULL);
//    move(scr_y, scr_x);
//    refresh();
//}

void set_top_line()
{
    set_status(STAT_POS_TOPLINE, "Multi Arp - Midi:%02i, Step:%5.2f, Link Quantum:%5.2f     %s",
               g_Sequencer.MidiChannel() + 1,
               g_State.CurrentStepValue(),
               g_State.Quantum(),
               g_State.RunState() ? "<<   RUN   >>" : "<<   ---   >>");

    g_TextUI.Highlight(BaseUI::whole_screen, 0, 0, 80,
        g_ListBuilder.MidiInputModeAsColour(vector<int> {CP_MAIN, CP_RECORD, CP_RECORD, CP_REALTIME}),  // Hmm ...
        BaseUI::attr_bold);
    g_TextUI.Highlight(BaseUI::whole_screen, 0, 60, 5,
        g_State.RunState() ? CP_RUNNING : CP_MAIN,
        BaseUI::attr_bold);
}

void update_progress_bar()
{
    double progress, stepWidth;
    g_State.Progress(progress, stepWidth);

    g_TextUI.Progress(progress, stepWidth, g_State.Phase() + 1, g_PatternStore.CurrentPosPatternChain() + 1);

//    int row = 2;
//    int col = 4;
//    double width = 64;
//
//
//    double progress, stepWidth;
//    g_State.Progress(progress, stepWidth);
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
//    sprintf(buff, "%7.2f ", g_State.Phase() + 1);
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
////        mvaddch(row, col + n, ACS_RARROW);
////        mvaddch(row, col + n, '>');
//        mvprintw(row, col + n, marker.c_str());
//        break;
//    case 2:
//        attron(COLOR_PAIR(CP_PROGRESS_BAR_BKGND));
//        mvprintw(row, col, barline.c_str());
//        attron(COLOR_PAIR(CP_PROGRESS_BAR_HIGHLIGHT));
//        mvprintw(row, col + n, marker.c_str());
//        break;
//    default:
////        move(row, col);
////        clrtoeol();
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
//    mvchgat(2, 4, g_PatternStore.CurrentPosPatternChain() + 1,
//            A_UNDERLINE, CP_PROGRESS_BAR_BKGND, NULL);
//
//    move(scr_y, scr_x);
//    refresh();
//
}

// If Pattern Chains active, show pattern chain status. Otherwise
// show pattern store status summary.
// Called from queue_next_step() when phase zero to catch pattern chain updates.
// Called from update_edit_panels() to catch any pattern store changes.

void update_pattern_status_panel()
{
//    wmove(g_TextUI.SmallPanel(), 1, 0);
//    wclrtoeol(g_TextUI.SmallPanel());
//    wmove(g_TextUI.SmallPanel(), 2, 0);
//    wclrtoeol(g_TextUI.SmallPanel());

    set_status_w(STAT_POS_PATTERN_EDIT, g_PatternStore.PatternStatusEdit().c_str());

    if ( g_PatternStore.PatternChainMode() == PatternChain::off ||
         g_PatternStore.Empty() ||
         g_PatternStore.PatternChainEmpty() )
    {
        set_status_w(STAT_POS_PATTERN, g_PatternStore.PatternStatusPlay().c_str());
        return;
    }

    // We're displaying a Pattern Chain and we have to pick a row to display.

    static int firstRow = 0;
    const int rows = 1;

    int selection = g_PatternStore.CurrentPosPatternChain();

    while ( selection >= 4 * (firstRow + rows) )
        firstRow += 1;

    while ( selection < 4 * firstRow )
        firstRow -= 1;

    int rowSelect = selection/4 - firstRow;

//    int scr_x, scr_y;
//    getyx(stdscr, scr_y, scr_x);

//    mvwprintw(g_TextUI.SmallPanel(), 1, 0, g_PatternStore.PatternChainToStringForDisplay(firstRow, rows).c_str());
    g_TextUI.Text(BaseUI::small_panel, 1, 0, g_PatternStore.PatternChainToStringForDisplay(firstRow, rows).c_str());
//    mvwchgat(g_TextUI.SmallPanel(), 1 + rowSelect, 5 + 12 * (selection % 4), 12, 0, CP_PATTERN_CHAIN_HIGHLIGHT, NULL);
    g_TextUI.Highlight(BaseUI::small_panel, 1 + rowSelect, 5 + 12 * (selection % 4), 12, CP_PATTERN_CHAIN_HIGHLIGHT);

//    wrefresh(g_TextUI.SmallPanel());
//    move(scr_y, scr_x);
//    refresh();
}

void update_edit_panels(bool refreshList)
{
    update_pattern_status_panel();

    switch ( g_TextUI.BigPanelPage() )
    {
    case TextUI::one:
    case TextUI::two:
        break;
    default:
        return;
    }

    if ( g_PatternStore.Empty() )
    {
        g_TextUI.ClearArea(BaseUI::edit_list_panel);
//        wmove(g_TextUI.EditListPanel(), 0, 0);
//        wclrtobot(g_TextUI.EditListPanel());
//        wrefresh(g_TextUI.EditListPanel());
        g_TextUI.ClearArea(BaseUI::edit_summary_panel);
//        wmove(g_TextUI.EditSummaryPanel(), 0, 0);
//        wclrtobot(g_TextUI.EditSummaryPanel());
//        wrefresh(g_TextUI.EditSummaryPanel());
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

//    wmove(g_TextUI.EditListPanel(), 0, 0);
//    wclrtobot(g_TextUI.EditListPanel());
    g_TextUI.ClearArea(BaseUI::edit_list_panel);

//    wprintw(g_TextUI.EditListPanel(), g_PatternStore.PatternSelectionList(listStart, rows).c_str());
    g_TextUI.Text(BaseUI::edit_list_panel, 0, 0, g_PatternStore.PatternSelectionList(listStart, rows).c_str());
//    mvwchgat(g_TextUI.EditListPanel(), selection - listStart, 0, 20, 0, CP_SUMMARY_PANEL_BKGND, NULL);
    g_TextUI.Highlight(BaseUI::edit_list_panel, selection - listStart, 0, 20, CP_SUMMARY_PANEL_BKGND);

//    wrefresh(g_TextUI.EditListPanel());

    // Summary Panel

//    wmove(g_TextUI.EditSummaryPanel(), 0, 0);
//    wclrtobot(g_TextUI.EditSummaryPanel());
    g_TextUI.ClearArea(BaseUI::edit_summary_panel);

    char text[100];
    Pattern & p = g_PatternStore.CurrentEditPattern();

//    wmove(g_TextUI.EditSummaryPanel(), 0, 1);
//    wprintw(g_TextUI.EditSummaryPanel(), "List(s) %i, Real Time %i, Trigs %i", p.StepListCount(), p.RealTimeListCount(), p.TrigListCount());
    snprintf(text, 100, "List(s) %i, Real Time %i, Trigs %i", p.StepListCount(), p.RealTimeListCount(), p.TrigListCount());
    g_TextUI.Text(BaseUI::edit_summary_panel, 0, 1, text);

//    wmove(g_TextUI.EditSummaryPanel(), 1, 1);
//    wprintw(g_TextUI.EditSummaryPanel(), "Step value %.2f, Vel %i, Gate %.0f%% (Hold %s)", p.StepValue(),
//        p.Velocity(), p.Gate() * 100, p.GateHold() ? "on" : "off");
    snprintf(text, 100, "Step value %.2f, Vel %i, Gate %.0f%% (Hold %s)", p.StepValue(),
        p.Velocity(), p.Gate() * 100, p.GateHold() ? "on" : "off");
    g_TextUI.Text(BaseUI::edit_summary_panel, 1, 1, text);

    TranslateTable & table = p.PatternTranslateTable();

//    wmove(g_TextUI.EditSummaryPanel(), 2, 1);
//    wprintw(g_TextUI.EditSummaryPanel(), "Chromatic %i, Tonal %i (%s), %s-%s",
//            table.Transpose(),
//            table.DegreeShift(),
//            table.ShiftName(),
//            table.RootName().c_str(),
//            table.ScaleName());
    snprintf(text, 100, "Chromatic %i, Tonal %i (%s), %s-%s",
            table.Transpose(),
            table.DegreeShift(),
            table.ShiftName(),
            table.RootName().c_str(),
            table.ScaleName());
    g_TextUI.Text(BaseUI::edit_summary_panel, 2, 1, text);

//    wmove(g_TextUI.EditSummaryPanel(), 3, 1);
//    wprintw(g_TextUI.EditSummaryPanel(), "Premap %s, Accidentals %s",
//            table.PremapModeName(),
//            table.AccidentalsModeName());
    snprintf(text, 100, "Premap %s, Accidentals %s",
            table.PremapModeName(),
            table.AccidentalsModeName());
    g_TextUI.Text(BaseUI::edit_summary_panel, 3, 1, text);

//    wrefresh(g_TextUI.EditSummaryPanel());
}

void layout_pattern_extra_panel(vector<InOutPair> & pairs)
{
    g_TextUI.ShowNoteTransforms(pairs);

//    const int kbdStart = 29;
//    WINDOW *panel = g_TextUI.BigPanelExtra();
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
//    wrefresh(panel);

    pairs.clear();
}

//void layout_pattern_extra_panel_test()
//{
//    WINDOW *panel = g_TextUI.BigPanelExtra();
//    wmove(panel, 0, 0);
//    wclrtobot(panel);
//
//    for ( int row = 0; row < 3; row++ )
//    {
//        mvwaddch(panel, row, 0,  ACS_BSSB);
//        mvwaddch(panel, row, 2,  ACS_SSBB);
//        mvwaddch(panel, row, 4,  ACS_BBSS);
//        mvwaddch(panel, row, 6,  ACS_SBBS);
//        mvwaddch(panel, row, 8,  ACS_SBSS);
//        mvwaddch(panel, row, 10, ACS_SSSB);
//        mvwaddch(panel, row, 12, ACS_SSBS);
//        mvwaddch(panel, row, 14, ACS_BSSS);
//        mvwaddch(panel, row, 16, ACS_BSBS);
//        mvwaddch(panel, row, 18, ACS_SBSB);
//        mvwaddch(panel, row, 20, ACS_SSSS);
//
//        mvwaddch(panel, row, 22,  ACS_S1		);
//        mvwaddch(panel, row, 24,  ACS_S9		);
//        mvwaddch(panel, row, 26,  ACS_DIAMOND	);
//        mvwaddch(panel, row, 28,  ACS_CKBOARD	);
//        mvwaddch(panel, row, 30,  ACS_DEGREE	);
//        mvwaddch(panel, row, 32,  ACS_PLMINUS	);
//        mvwaddch(panel, row, 34,  ACS_BULLET	);
//        mvwaddch(panel, row, 36,  ACS_LARROW	);
//        mvwaddch(panel, row, 38,  ACS_RARROW	);
//        mvwaddch(panel, row, 40,  ACS_DARROW	);
//        mvwaddch(panel, row, 42,  ACS_UARROW	);
//        mvwaddch(panel, row, 44,  ACS_BOARD	    );
//        mvwaddch(panel, row, 46,  ACS_LANTERN	);
//        mvwaddch(panel, row, 48,  ACS_BLOCK	    );
//    }
//
//    wrefresh(panel);
//}

void update_pattern_panel()
{
//    int scr_x, scr_y;

    if ( g_TextUI.BigPanelHold() )
        return;

//    wmove(g_TextUI.BigPanel(), 0, 0);

    try
    {
        bool showTrigProgress = false;
        static vector<PosInfo2> highlights; // Reset every for every update for pages 1 & 2, persist for page 3.

        switch ( g_TextUI.BigPanelPage() )
        {
        case BaseUI::one:
//            wclrtobot(g_TextUI.BigPanel());
            g_TextUI.ClearArea(BaseUI::big_panel);
//            wprintw(g_TextUI.BigPanel(), g_PatternStore.CurrentPlayPattern().Display(2, highlights, 25, 79).c_str());
            g_TextUI.Text(BaseUI::big_panel, 0, 0, g_PatternStore.CurrentPlayPattern().Display(2, highlights, 25, 79).c_str());
            showTrigProgress = true;
            break;
        case BaseUI::two:
//            wclrtobot(g_TextUI.BigPanel());
            g_TextUI.ClearArea(BaseUI::big_panel);
//            wprintw(g_TextUI.BigPanel(), g_PatternStore.CurrentPlayPattern().Display(1, highlights, 25, 79).c_str());
            g_TextUI.Text(BaseUI::big_panel, 0, 0, g_PatternStore.CurrentPlayPattern().Display(1, highlights, 25, 79).c_str());
            showTrigProgress = true;
            break;
        case BaseUI::three:
//            layout_pattern_extra_panel_test();
            layout_pattern_extra_panel(g_PatternStore.TranslateTableForPlay().Diags().InOutPairs());
//            wprintw(g_TextUI.BigPanel(), g_PatternStore.TranslateTableForPlay().Diags().Log(highlights).c_str());
            g_TextUI.Text(BaseUI::big_panel, 0, 0, g_PatternStore.TranslateTableForPlay().Diags().Log(highlights).c_str());
            break;
        default:
            break;
        }

        switch ( g_TextUI.BigPanelPage() )
        {
        case BaseUI::one:
        case BaseUI::two:
            for ( auto it = highlights.begin(); it < highlights.end(); it++ )
            {
//                mvwchgat(g_TextUI.BigPanel(), it->row, it->offset, it->length, 0,
//                    it->row < 2 ? CP_PATTERN_LIST_PANEL_HIGHLIGHT_TRIG : CP_PATTERN_LIST_PANEL_HIGHLIGHT,
//                    NULL);
                g_TextUI.Highlight(BaseUI::big_panel, it->row, it->offset, it->length,
                    it->row < 2 ? CP_PATTERN_LIST_PANEL_HIGHLIGHT_TRIG : CP_PATTERN_LIST_PANEL_HIGHLIGHT);
            }
            highlights.clear(); // Clear highlights here in case page switches before next update.
            break;
        case BaseUI::three:
            // Row highlighting here depends on the current row position inserted at the front of the list.
//            mvwchgat(g_TextUI.BigPanel(), highlights.back().row, highlights.back().offset, highlights.back().length, 0, CP_PATTERN_LIST_PANEL, NULL);
            g_TextUI.Highlight(BaseUI::big_panel, highlights.back().row, highlights.back().offset, highlights.back().length, CP_PATTERN_LIST_PANEL);
//            mvwchgat(g_TextUI.BigPanel(), highlights.front().row, highlights.front().offset, highlights.front().length, 0, CP_PATTERN_LIST_PANEL_HIGHLIGHT_TRIG, NULL);
            g_TextUI.Highlight(BaseUI::big_panel, highlights.front().row, highlights.front().offset, highlights.front().length, CP_PATTERN_LIST_PANEL_HIGHLIGHT_TRIG);
            if ( highlights.size() > 1 )
                highlights.pop_back();
            break;
        default:
            break;
        }

        // Kludge to show overall trig position.

        if ( showTrigProgress )
//            mvwchgat(g_TextUI.BigPanel(), 0, 4, g_PatternStore.CurrentPlayPattern().TrigPlayPosition() + 1,
//                A_UNDERLINE, CP_PATTERN_LIST_PANEL, NULL);
            g_TextUI.Highlight(BaseUI::big_panel, 0, 4, g_PatternStore.CurrentPlayPattern().TrigPlayPosition() + 1,
                CP_PATTERN_LIST_PANEL, BaseUI::attr_underline);
    }
    catch (... /*string s*/)
    {
        // wprintw(g_TextUI.BigPanel(), s.c_str());
    }

//    wrefresh(g_TextUI.BigPanel());

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
        g_TextUI.Highlight(BaseUI::whole_screen, STAT_POS_MENU + highlights.at(i).offset - adjustOffset, highlights.at(i).length, CP_MENU_HIGHLIGHT, BaseUI::attr_bold);

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
