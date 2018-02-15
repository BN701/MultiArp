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

#ifndef MAANSIUI_H_INCLUDED
#define MAANSIUI_H_INCLUDED

#include <vector>


#if defined(MA_BLUE) && ! defined(MA_BLUE_PC)

// Only define this if we're being built by the Arduino IDE,
// otherwise we're on a PC testing out the rest of the 'blue' build.

#define USE_SERIAL

#endif

#ifndef USE_SERIAL
#include <termios.h>
#endif

#include "maBaseUI.h"
#include "maCursorKeys.h"
#include "maNotes.h"

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


class AnsiUI : public BaseUI
{
    public:

        AnsiUI();
        ~AnsiUI();

//        int CursesAttribute(text_attribute_t attribute);

        void SetAttribute(text_attribute_t = attr_normal);
        virtual void Text(window_area_t area, int row, int col, const char * text, std::vector<PosInfo2> * highlights = NULL, text_attribute_t attribute = attr_normal);
        void SetTextRowHighlight(int row)   // Only support inverse video attribute for now.
        {
            m_RowHighlight = row;
        }
        void HighlightLastWrite(uint16_t col, uint16_t len, uint16_t colour = 0, text_attribute_t attr = attr_reverse);
        void Highlight(window_area_t area, int row, int col, int len, int colour, text_attribute_t attr = attr_normal);
        void ClearArea(window_area_t area);
        void PlaceCursor(int row, int col);
        void PlaceCursorXY(int col, int row)
        {
            PlaceCursor(row, col);
        }
        virtual void MoveCursorToHome()
        {
            PlaceCursor(m_HomeRow, m_HomeCol);
        }
        void SendSaveCursor();          // Calls to these two cannot be nested as
        void SendRestoreCursor();       // the terminal doesn't maintain a stack of positions.
        void SendSaveAndHideCursor();

//        void KeyInput(CursorKeys::key_type_t & curKey, xcb_keysym_t & sym);
        key_command_t KeyInput();

        void SetTopLine(int midiChannel, double stepValue, double tempo, double quantum, int runState, int midiMode);

        virtual void Progress(double progress, double stepWidth, double beat,
                int pattern_progress, double rtBeat, unsigned int queueSecs, unsigned int queueNano);
        void ShowNoteTransforms(std::vector<InOutPair> & pairs);

        key_command_t GetCSISequence(int firstChar);

        int Read();
        void ResetScreen();
        void ClearEOL();
        size_t FWriteXY(int col, int row, const char *format, ...);

    protected:
        size_t Write(const char * s);
        size_t WriteXY(int col, int row, const char * s);

    private:
        std::string m_LastWrite;
        int m_LastRow = 0;
        int m_LastCol = 0;
        int m_RowHighlight = -1;
#ifndef USE_SERIAL
        struct termios m_old_tio;
        struct termios m_new_tio;
#endif
};

#endif // MAANSIUI_H_INCLUDED
