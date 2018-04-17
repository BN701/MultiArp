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
// Global ALSA Sequencer instance.
extern AlsaSequencer g_Sequencer;

#define LINK_PLATFORM_LINUX
#include <ableton/Link.hpp>

// Global Link instance.

extern ableton::Link g_Link;

#endif // MA_BLUE

#include "maListBuilder.h"
#include "maPatternStore.h"
#include "maState.h"
#include "maTranslateTable.h"
#include "maUtility.h"

using namespace std;


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

    g_TextUI.Text(BaseUI::whole_screen, y, x, text, NULL, BaseUI::attr_normal);
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
    g_TextUI.SetTopLine(g_Sequencer.MidiChannel() + 1,
                        g_State.CurrentStepValue(),
#if defined(MA_BLUE)
                        g_State.Tempo(),
#else
                        g_Link.captureAppTimeline().tempo(),
#endif
                        g_State.Quantum(),
                        g_State.RunState(),
                        g_State.RecState(),
                        g_ListBuilder.MidiInputMode() );

#ifndef MA_BLUE
    g_CairoUI.SetTopLine(g_Sequencer.MidiChannel() + 1,
                        g_State.CurrentStepValue(),
                        g_Link.captureAppTimeline().tempo(),
                        g_State.Quantum(),
                        g_State.RunState(),
                        g_State.RecState(),
                        g_ListBuilder.MidiInputMode() );
#endif
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
// Called from update_pattern_list_panels() to catch any pattern store changes.

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

void update_pattern_list_panels(bool refreshList)
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

    if ( g_PatternStore.Empty() )
    {
        g_TextUI.ClearArea(BaseUI::pattern_list_panel);
        g_TextUI.ClearArea(BaseUI::pattern_list_summary_panel);
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

    g_TextUI.ClearArea(BaseUI::pattern_list_panel);

    g_TextUI.SetTextRowHighlight(selection - listStart);
    g_TextUI.Text(BaseUI::pattern_list_panel, 0, 0, g_PatternStore.PatternSelectionList(listStart, rows).c_str());
    g_TextUI.Highlight(BaseUI::pattern_list_panel, selection - listStart, 0, 20, CP_SUMMARY_PANEL_BKGND);

    // Summary Panel

    g_TextUI.ClearArea(BaseUI::pattern_list_summary_panel);

    char text[100];
    Pattern & p = g_PatternStore.CurrentEditPattern();

// TODO:LG
//    snprintf(text, 100, "List(s) %i, Real Time %i, Trigs %i", p.StepListCount(), p.RealTimeListCount(), p.TrigListCount());
    g_TextUI.Text(BaseUI::pattern_list_summary_panel, 0, 1, text);

    snprintf(text, 100, "Step value %.2f, Vel %i, Gate %.0f%% (Hold %s)", p.StepValue(),
        p.Velocity(), p.Gate() * 100, p.GateHold() ? "on" : "off");
    g_TextUI.Text(BaseUI::pattern_list_summary_panel, 1, 1, text);

    TranslateTable & table = p.PatternTranslateTable();

    snprintf(text, 100, "Chromatic %i, Tonal %i (%s), %s-%s",
            table.Transpose(),
            table.DegreeShift(),
            table.ShiftName(),
            table.RootName().c_str(),
            table.ScaleName());
    g_TextUI.Text(BaseUI::pattern_list_summary_panel, 2, 1, text);

    snprintf(text, 100, "Premap %s, Accidentals %s",
            table.PremapModeName(),
            table.AccidentalsModeName());
    g_TextUI.Text(BaseUI::pattern_list_summary_panel, 3, 1, text);

}

void layout_pattern_extra_panel(vector<InOutPair> & pairs)
{
    g_TextUI.ShowNoteTransforms(pairs);

    pairs.clear();
}

void update_big_panel_v1()
{
    if ( g_TextUI.BigPanelHold() )
        return;

#if !defined(MA_BLUE) // || defined(MA_BLUE_PC)
    try
    {
#endif
        bool showTrigProgress = false;
        static vector<PosInfo2> highlights; // Reset every for every update for pages 1 & 2, persist for page 3.
        string text;
        switch ( g_TextUI.BigPanelPage() )
        {
        case BaseUI::one:
            g_TextUI.ClearArea(BaseUI::big_panel);
            text = g_PatternStore.CurrentPlayPattern().Display(2, highlights, 25, 79);
            g_TextUI.Text(BaseUI::big_panel, 0, 0, text.c_str(), & highlights);
//#if defined(MA_BLUE) // || defined(MA_BLUE_PC)
            highlights.clear();
//#endif
            showTrigProgress = true;
            break;
        case BaseUI::two:
            g_TextUI.ClearArea(BaseUI::big_panel);
            text = g_PatternStore.CurrentPlayPattern().Display(1, highlights, 25, 79);
            g_TextUI.Text(BaseUI::big_panel, 0, 0, text.c_str(), & highlights);
            showTrigProgress = true;
//#if defined(MA_BLUE) // || defined(MA_BLUE_PC)
            highlights.clear();
//#endif
            break;
        case BaseUI::three:
            layout_pattern_extra_panel(g_PatternStore.TranslateTableForPlay().Diags().InOutPairs());
            text = g_PatternStore.TranslateTableForPlay().Diags().Log(highlights);
            g_TextUI.Text(BaseUI::big_panel, 0, 0, text.c_str(), & highlights);
            break;
        default:
            break;
        }

#if 0
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
#endif

#if !defined(MA_BLUE) // || defined(MA_BLUE_PC)
    }
    catch (... /*string s*/)
    {
        // wprintw(g_TextUI.BigPanel(), s.c_str());
    }
#endif

}



void show_status()
{
    set_status(STAT_POS_2, g_PatternStore.PatternOverview().c_str());
}

void show_status_after_navigation()
{
    return;

    const int width = 72;

//    if ( ItemMenu::RedrawMenuList() )
//        update_big_panel();

    static int adjustOffset = 0;
    string status = ItemMenu::Status();

    // Although I started off with a mechanism which allows for multiple
    // highlights, so far I've only ever set one at a time. Now that I
    // need to handle long strings and need to make sure the highlight
    // is visible I'm relying on there only being one highlight and using
    // that to position the whole string within the available width. Any
    // highlights set after the first may end up off screen.

    if ( status.size() < width )
        adjustOffset = 0;

    vector<screen_pos_t> & highlights = ItemMenu::GetHighlights();

    if ( !highlights.empty() )
    {
        screen_pos_t cursor = highlights.front();

        if ( ItemMenu::FirstField() )
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

    g_TextUI.SendSaveCursor();

    int row, col;
    if ( ItemMenu::DisplayPos(row, col) )
        set_status(row, col, status.substr(adjustOffset, width).c_str());
    else
        set_status(STAT_POS_MENU, status.substr(adjustOffset, width).c_str());

    for ( size_t i = 0; i < highlights.size(); i++ )
//        g_TextUI.Highlight(BaseUI::whole_screen, STAT_POS_MENU + highlights.at(i).offset - adjustOffset, highlights.at(i).length, CP_MENU_HIGHLIGHT, BaseUI::attr_bold);
        g_TextUI.HighlightLastWrite(highlights.at(i).offset - adjustOffset + 2, highlights.at(i).length, CP_MENU_HIGHLIGHT, BaseUI::attr_bold);

    g_TextUI.SendRestoreCursor();

}

void update_item_menus()
{
    if ( ItemMenu::RedrawList().empty() )
        return;

    // One loop for each UI interested in this.

    for ( auto it = ItemMenu::RedrawList().begin(); it != ItemMenu::RedrawList().end(); it++ )
    {
        ItemMenu * menuItem = *it;
        menuItem->SetStatus();
        Rectangle clearArea = {0, 0, 0, 0};   // Fill this in if an area of screen needs to be cleared first.
        int row, col, width;

        if ( !menuItem->GetDisplayInfo(g_TextUI, row, col, width, clearArea) )  // Returns false means item not visible.
            continue;

        if ( clearArea.m_iHeight > 0 )
            g_TextUI.ClearArea(&clearArea);

        static int adjustOffset = 0;
        string status = menuItem->StatusString();

        // Although I started off with a mechanism which allows for multiple
        // highlights, so far I've only ever set one at a time. Now that I
        // need to handle long strings and need to make sure the highlight
        // is visible I'm relying on there only being one highlight and using
        // that to position the whole string within the available width. Any
        // highlights set after the first may end up off screen.

        if ( status.size() < width )
            adjustOffset = 0;

        vector<screen_pos_t> & highlights = menuItem->ItemHighlights();

        if ( !highlights.empty() )
        {
            screen_pos_t cursor = highlights.front();

            if ( menuItem->FirstField() )
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

        set_status(row, col, status.substr(adjustOffset, width).c_str());

        for ( auto h = highlights.begin(); h != highlights.end(); h++ )
            g_TextUI.HighlightLastWrite(h->offset - adjustOffset, h->length, CP_MENU_HIGHLIGHT, BaseUI::attr_bold);
    }

    // Then clear the 'dirty' list.

    ItemMenu::ClearRedrawList();
}

void update_big_panel()
{

    return;

    g_TextUI.ClearArea(BaseUI::big_panel);

    if ( g_PatternStore.Empty() )
        return;

//    static vector<PosInfo2> highlights; // Reset every for every update for pages 1 & 2, persist for page 3.

    MenuList & menus = g_PatternStore.CurrentEditPattern().m_MenuList;

    for ( auto it = menus.m_Items.begin(); it != menus.m_Items.end(); it++ )
    {
        ItemMenu & menuItem = **it;
        menuItem.SetStatus();
        Rectangle clearArea = {0, 0, 0, 0};
        int row, col, width;
        if ( menuItem.GetDisplayInfo(g_TextUI, row, col, width, clearArea) )
        {
            g_TextUI.MapToFullScreen(BaseUI::big_panel, row, col);
            set_status(row, col, menuItem.StatusString().c_str());
        }
        else
            set_status(STAT_POS_MENU, menuItem.StatusString().c_str());
//        char text[60];
//        string pad((*menuItem)->DisplayIndent(), ' ');
//        snprintf(text, 60, "%s%s%s", menuItem == cursor ? " -> " : "    ", pad.c_str(), (*menuItem)->StatusString().c_str());
//        g_TextUI.Text(BaseUI::big_panel, row, 0, text, & highlights);
    }

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
