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
    g_CairoUI.Text(BaseUI::whole_screen, y+2, x, text, BaseUI::attr_normal);
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
    g_CairoUI.Text(BaseUI::whole_screen, y + 3, x, text, BaseUI::attr_normal);
}


void set_top_line()
{
    g_TextUI.SetTopLine(g_Sequencer.MidiChannel() + 1,
                        g_State.CurrentStepValue(),
                        g_State.Quantum(),
                        g_State.RunState(),
                        g_ListBuilder.MidiInputMode() );

    g_CairoUI.SetTopLine(g_Sequencer.MidiChannel() + 1,
                        g_State.CurrentStepValue(),
                        g_State.Quantum(),
                        g_State.RunState(),
                        g_ListBuilder.MidiInputMode() );
}

void update_progress_bar()
{
    double progress, stepWidth;
    g_State.Progress(progress, stepWidth);

    g_TextUI.Progress(progress,
                    stepWidth,
                    g_State.Phase() + 1,
                    g_PatternStore.CurrentPosPatternChain() + 1,
                    g_PatternStore.LastRealTimeBeat(),
                    g_Sequencer.ScheduleTimeSeconds(),
                    g_Sequencer.ScheduleTimeNanoSeconds() / 100000000);

    g_CairoUI.Progress(progress,
                    stepWidth,
                    g_State.Phase() + 1,
                    g_PatternStore.CurrentPosPatternChain() + 1,
                    g_PatternStore.LastRealTimeBeat(),
                    g_Sequencer.ScheduleTimeSeconds(),
                    g_Sequencer.ScheduleTimeNanoSeconds() / 100000000);
//    char text[80];
//    sprintf(text, "Beat%9.2f (Sec%6i:%i)",
//               g_PatternStore.LastRealTimeBeat(),
//               g_Sequencer.ScheduleTimeSeconds(),
//               g_Sequencer.ScheduleTimeNanoSeconds() / 100000000);
//
//    set_status_w(STAT_POS_STEP, " Beat%9.2f\n (Sec%6i:%i)",
//               g_PatternStore.LastRealTimeBeat(),
//               g_Sequencer.ScheduleTimeSeconds(),
//               g_Sequencer.ScheduleTimeNanoSeconds() / 100000000);
//

}

// If Pattern Chains active, show pattern chain status. Otherwise
// show pattern store status summary.
// Called from queue_next_step() when phase zero to catch pattern chain updates.
// Called from update_edit_panels() to catch any pattern store changes.

void update_pattern_status_panel()
{
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

    g_TextUI.Text(BaseUI::small_panel, 1, 0, g_PatternStore.PatternChainToStringForDisplay(firstRow, rows).c_str());
    g_TextUI.Highlight(BaseUI::small_panel, 1 + rowSelect, 5 + 12 * (selection % 4), 12, CP_PATTERN_CHAIN_HIGHLIGHT);
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
        g_TextUI.ClearArea(BaseUI::edit_summary_panel);
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

    g_TextUI.ClearArea(BaseUI::edit_list_panel);

    g_TextUI.Text(BaseUI::edit_list_panel, 0, 0, g_PatternStore.PatternSelectionList(listStart, rows).c_str());
    g_TextUI.Highlight(BaseUI::edit_list_panel, selection - listStart, 0, 20, CP_SUMMARY_PANEL_BKGND);

    // Summary Panel

    g_TextUI.ClearArea(BaseUI::edit_summary_panel);

    char text[100];
    Pattern & p = g_PatternStore.CurrentEditPattern();

    snprintf(text, 100, "List(s) %i, Real Time %i, Trigs %i", p.StepListCount(), p.RealTimeListCount(), p.TrigListCount());
    g_TextUI.Text(BaseUI::edit_summary_panel, 0, 1, text);

    snprintf(text, 100, "Step value %.2f, Vel %i, Gate %.0f%% (Hold %s)", p.StepValue(),
        p.Velocity(), p.Gate() * 100, p.GateHold() ? "on" : "off");
    g_TextUI.Text(BaseUI::edit_summary_panel, 1, 1, text);

    TranslateTable & table = p.PatternTranslateTable();

    snprintf(text, 100, "Chromatic %i, Tonal %i (%s), %s-%s",
            table.Transpose(),
            table.DegreeShift(),
            table.ShiftName(),
            table.RootName().c_str(),
            table.ScaleName());
    g_TextUI.Text(BaseUI::edit_summary_panel, 2, 1, text);

    snprintf(text, 100, "Premap %s, Accidentals %s",
            table.PremapModeName(),
            table.AccidentalsModeName());
    g_TextUI.Text(BaseUI::edit_summary_panel, 3, 1, text);

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

    try
    {
        bool showTrigProgress = false;
        static vector<PosInfo2> highlights; // Reset every for every update for pages 1 & 2, persist for page 3.

        switch ( g_TextUI.BigPanelPage() )
        {
        case BaseUI::one:
            g_TextUI.ClearArea(BaseUI::big_panel);
            g_TextUI.Text(BaseUI::big_panel, 0, 0, g_PatternStore.CurrentPlayPattern().Display(2, highlights, 25, 79).c_str());
            showTrigProgress = true;
            break;
        case BaseUI::two:
            g_TextUI.ClearArea(BaseUI::big_panel);
            g_TextUI.Text(BaseUI::big_panel, 0, 0, g_PatternStore.CurrentPlayPattern().Display(1, highlights, 25, 79).c_str());
            showTrigProgress = true;
            break;
        case BaseUI::three:
            layout_pattern_extra_panel(g_PatternStore.TranslateTableForPlay().Diags().InOutPairs());
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

        if ( showTrigProgress )
            g_TextUI.Highlight(BaseUI::big_panel, 0, 4, g_PatternStore.CurrentPlayPattern().TrigPlayPosition() + 1,
                CP_PATTERN_LIST_PANEL, BaseUI::attr_underline);
    }
    catch (... /*string s*/)
    {
        // wprintw(g_TextUI.BigPanel(), s.c_str());
    }

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
