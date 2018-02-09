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

#include "maScreen.h"

#include <cstdarg>

#ifdef MA_BLUE
#include <cstdio>
#include "maSequencer.h"
extern Sequencer g_Sequencer;
#else
#include "platform_Linux/maAlsaSequencer.h"
#include "platform_Linux/maAlsaSequencerQueue.h"
extern AlsaSequencer g_Sequencer;
#endif // MA_BLUE
#include "maListBuilder.h"
#include "maPatternStore.h"
#include "maState.h"
#include "maTranslateTable.h"
#include "maUtility.h"

using namespace std;

// Global ALSA Sequencer instance.

extern ListBuilder g_ListBuilder;
extern PatternStore g_PatternStore;
extern State g_State;

extern AnsiUI g_TextUI;
#ifndef MA_BLUE
extern CairoUI g_CairoUI;
#endif


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
#ifndef MA_BLUE
    g_CairoUI.Text(BaseUI::whole_screen, y+2, x, text, BaseUI::attr_normal);
#endif
}

//void set_status_w(WINDOW * w, int y, int x, const char *format, ...)
void set_status_w(AnsiUI::window_area_t area, int y, int x, const char *format, ...)
{
    char text[100];
    va_list args;
    va_start(args, format);
    vsnprintf(text, 100, format, args);
    va_end(args);

    g_TextUI.Text(area, y, x, text);
#ifndef MA_BLUE
    g_CairoUI.Text(BaseUI::whole_screen, y + 3, x, text, BaseUI::attr_normal);
#endif
}


void set_top_line()
{
// Hack    g_TextUI.SetTopLine(g_Sequencer.MidiChannel() + 1,
// Hack                        g_State.CurrentStepValue(),
// Hack                        g_State.Quantum(),
// Hack                        g_State.RunState(),
// Hack                        g_ListBuilder.MidiInputMode() );

#ifndef MA_BLUE
    g_CairoUI.SetTopLine(g_Sequencer.MidiChannel() + 1,
                        g_State.CurrentStepValue(),
                        g_State.Quantum(),
                        g_State.RunState(),
                        g_ListBuilder.MidiInputMode() );
#endif
}

void update_progress_bar()
{
    double progress, stepWidth;
    g_State.Progress(progress, stepWidth);

// Hack    g_TextUI.Progress(progress,
// Hack                    stepWidth,
// Hack                    g_State.Phase() + 1,
// Hack                    g_PatternStore.CurrentPosPatternChain() + 1,
// Hack                    g_PatternStore.LastRealTimeBeat(),
// Hack                    g_Sequencer.ScheduleTimeSeconds(),
// Hack                    g_Sequencer.ScheduleTimeNanoSeconds() / 100000000);

#ifndef MA_BLUE
    g_CairoUI.Progress(progress,
                    stepWidth,
                    g_State.Phase() + 1,
                    g_PatternStore.CurrentPosPatternChain() + 1,
                    g_PatternStore.LastRealTimeBeat(),
                    g_Sequencer.ScheduleTimeSeconds(),
                    g_Sequencer.ScheduleTimeNanoSeconds() / 100000000);
#endif

}

// If Pattern Chains active, show pattern chain status. Otherwise
// show pattern store status summary.
// Called from queue_next_step() when phase zero to catch pattern chain updates.
// Called from update_edit_panels() to catch any pattern store changes.

void update_pattern_status_panel()
{
// Hack    set_status_w(STAT_POS_PATTERN_EDIT, g_PatternStore.PatternStatusEdit().c_str());
// Hack
// Hack    if ( g_PatternStore.PatternChainMode() == PatternChain::off ||
// Hack         g_PatternStore.Empty() ||
// Hack         g_PatternStore.PatternChainEmpty() )
// Hack    {
// Hack        set_status_w(STAT_POS_PATTERN, g_PatternStore.PatternStatusPlay().c_str());
// Hack        return;
// Hack    }
// Hack
// Hack    // We're displaying a Pattern Chain and we have to pick a row to display.
// Hack
// Hack    static int firstRow = 0;
// Hack    const int rows = 1;
// Hack
// Hack    int selection = g_PatternStore.CurrentPosPatternChain();
// Hack
// Hack    while ( selection >= 4 * (firstRow + rows) )
// Hack        firstRow += 1;
// Hack
// Hack    while ( selection < 4 * firstRow )
// Hack        firstRow -= 1;
// Hack
// Hack    int rowSelect = selection/4 - firstRow;
// Hack
// Hack    g_TextUI.Text(BaseUI::small_panel, 1, 0, g_PatternStore.PatternChainToStringForDisplay(firstRow, rows).c_str());
// Hack    g_TextUI.Highlight(BaseUI::small_panel, 1 + rowSelect, 5 + 12 * (selection % 4), 12, CP_PATTERN_CHAIN_HIGHLIGHT);
}

void update_edit_panels(bool refreshList)
{
    update_pattern_status_panel();

    switch ( g_TextUI.BigPanelPage() )
    {
    case AnsiUI::one:
    case AnsiUI::two:
        break;
    default:
        return;
    }

 // Hack   if ( g_PatternStore.Empty() )
 // Hack   {
 // Hack       g_TextUI.ClearArea(BaseUI::edit_list_panel);
 // Hack       g_TextUI.ClearArea(BaseUI::edit_summary_panel);
 // Hack       return;
 // Hack   }

    // Selection List

    static int listStart = 0;
    const int rows = 4;

// Hack    int selection = g_PatternStore.CurrentPosEdit();
// Hack
// Hack    while ( selection >= listStart + rows )
// Hack        listStart += 1;
// Hack
// Hack    while ( selection < listStart )
// Hack        listStart -= 1;
// Hack
// Hack    g_TextUI.ClearArea(BaseUI::edit_list_panel);
// Hack
// Hack    g_TextUI.Text(BaseUI::edit_list_panel, 0, 0, g_PatternStore.PatternSelectionList(listStart, rows).c_str());
// Hack    g_TextUI.Highlight(BaseUI::edit_list_panel, selection - listStart, 0, 20, CP_SUMMARY_PANEL_BKGND);
// Hack
// Hack    // Summary Panel
// Hack
// Hack    g_TextUI.ClearArea(BaseUI::edit_summary_panel);
// Hack
// Hack    char text[100];
// Hack    Pattern & p = g_PatternStore.CurrentEditPattern();
// Hack
// Hack    snprintf(text, 100, "List(s) %i, Real Time %i, Trigs %i", p.StepListCount(), p.RealTimeListCount(), p.TrigListCount());
// Hack    g_TextUI.Text(BaseUI::edit_summary_panel, 0, 1, text);
// Hack
// Hack    snprintf(text, 100, "Step value %.2f, Vel %i, Gate %.0f%% (Hold %s)", p.StepValue(),
// Hack        p.Velocity(), p.Gate() * 100, p.GateHold() ? "on" : "off");
// Hack    g_TextUI.Text(BaseUI::edit_summary_panel, 1, 1, text);
// Hack
// Hack    TranslateTable & table = p.PatternTranslateTable();
// Hack
// Hack    snprintf(text, 100, "Chromatic %i, Tonal %i (%s), %s-%s",
// Hack            table.Transpose(),
// Hack            table.DegreeShift(),
// Hack            table.ShiftName(),
// Hack            table.RootName().c_str(),
// Hack            table.ScaleName());
// Hack    g_TextUI.Text(BaseUI::edit_summary_panel, 2, 1, text);
// Hack
// Hack    snprintf(text, 100, "Premap %s, Accidentals %s",
// Hack            table.PremapModeName(),
// Hack            table.AccidentalsModeName());
// Hack    g_TextUI.Text(BaseUI::edit_summary_panel, 3, 1, text);

}

void layout_pattern_extra_panel(vector<InOutPair> & pairs)
{
    g_TextUI.ShowNoteTransforms(pairs);

    pairs.clear();
}


void update_pattern_panel()
{
    if ( g_TextUI.BigPanelHold() )
        return;

#ifndef MA_BLUE
    try
    {
#endif
        bool showTrigProgress = false;
        static vector<PosInfo2> highlights; // Reset every for every update for pages 1 & 2, persist for page 3.

        switch ( g_TextUI.BigPanelPage() )
        {
        case BaseUI::one:
            g_TextUI.ClearArea(BaseUI::big_panel);
// Hack            g_TextUI.Text(BaseUI::big_panel, 0, 0, g_PatternStore.CurrentPlayPattern().Display(2, highlights, 25, 79).c_str());
            showTrigProgress = true;
            break;
        case BaseUI::two:
            g_TextUI.ClearArea(BaseUI::big_panel);
// Hack            g_TextUI.Text(BaseUI::big_panel, 0, 0, g_PatternStore.CurrentPlayPattern().Display(1, highlights, 25, 79).c_str());
            showTrigProgress = true;
            break;
        case BaseUI::three:
// Hack            layout_pattern_extra_panel(g_PatternStore.TranslateTableForPlay().Diags().InOutPairs());
// Hack            g_TextUI.Text(BaseUI::big_panel, 0, 0, g_PatternStore.TranslateTableForPlay().Diags().Log(highlights).c_str());
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
                g_TextUI.Highlight(BaseUI::big_panel, it->row, it->offset, it->length,
                    it->row < 2 ? CP_PATTERN_LIST_PANEL_HIGHLIGHT_TRIG : CP_PATTERN_LIST_PANEL_HIGHLIGHT);
            }
            highlights.clear(); // Clear highlights here in case page switches before next update.
            break;
        case BaseUI::three:
            // Row highlighting here depends on the current row position inserted at the front of the list.
            g_TextUI.Highlight(BaseUI::big_panel, highlights.back().row, highlights.back().offset, highlights.back().length, CP_PATTERN_LIST_PANEL);
            g_TextUI.Highlight(BaseUI::big_panel, highlights.front().row, highlights.front().offset, highlights.front().length, CP_PATTERN_LIST_PANEL_HIGHLIGHT_TRIG);
            if ( highlights.size() > 1 )
                highlights.pop_back();
            break;
        default:
            break;
        }

        // Kludge to show overall trig position.

// Hack        if ( showTrigProgress )
// Hack            g_TextUI.Highlight(BaseUI::big_panel, 0, 4, g_PatternStore.CurrentPlayPattern().TrigPlayPosition() + 1,
// Hack                CP_PATTERN_LIST_PANEL, BaseUI::attr_underline);
#ifndef MA_BLUE
    }
    catch (... /*string s*/)
    {
        // wprintw(g_TextUI.BigPanel(), s.c_str());
    }
#endif

}



void show_status()
{
// Hack    set_status(STAT_POS_2, g_PatternStore.PatternOverview().c_str());
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
//    string status = g_CursorKeys.Status();
    string status = CursorKeys::Status();

    // Although I started off with a mechanism which allows for multiple
    // highlights, so far I've only ever set one at a time. Now that I
    // need to handle long strings and need to make sure the highlight
    // is visible I'm relying on there only being one highlight and using
    // that to position the whole string within the available width. Any
    // highlights set after the first may end up off screen.

    if ( status.size() < width )
        adjustOffset = 0;

    vector<screen_pos_t> & highlights = CursorKeys::GetHighlights();

    if ( !highlights.empty() )
    {
        screen_pos_t cursor = highlights.front();

        if ( CursorKeys::FirstField() )
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
// Hack    set_status(STAT_POS_2, "I: %s     Map: %s", g_PatternStore.TranslateTableForEdit(false).ShowScale().c_str(), g_PatternStore.TranslateTableForEdit(false).ShowNoteMap().c_str());
}

void show_translation_status()
{
// Hack    set_status(STAT_POS_2, g_PatternStore.TranslateTableForEdit().Status().c_str());
}


void show_listbuilder_status()
{
// Hack::string temp = g_ListBuilder.ToString();
// Hack len = temp.size();
// Hack( len > 60 )
// Hack set_status(STAT_POS_2, "%c: ...%s", g_ListBuilder.MidiInputModeAsChar(), temp.c_str() + len - 60 );
// Hacke
// Hack set_status(STAT_POS_2, "%c:%s", g_ListBuilder.MidiInputModeAsChar(), temp.c_str());
}
