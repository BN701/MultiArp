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

#ifndef MACURSESUI_H_INCLUDED
#define MACURSESUI_H_INCLUDED

#include <vector>

#include <ncurses.h>


#include "maBaseUI.h"
#include "maCursorKeys.h"

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


class CursesUI : public BaseUI
{
    public:

        CursesUI();
        ~CursesUI();

        WINDOW * AreaToWindow(window_area_t area);
        int CursesAttribute(text_attribute_t attribute);

        virtual void Text(window_area_t area, int row, int col, const char * text, text_attribute_t attribute = attr_normal);
        void Highlight(window_area_t area, int row, int col, int len, int colour, text_attribute_t attr = attr_normal);
        void ClearArea(window_area_t area);
        void PlaceCursor(int row, int col);

//        void KeyInput(CursorKeys::key_type_t & curKey, xcb_keysym_t & sym);
        key_command_t KeyInput();

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

#endif // MACURSESUI_H_INCLUDED
