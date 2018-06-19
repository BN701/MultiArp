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

#include "maNotes.h"

#include <algorithm>
#include <cmath>
#include <unordered_map>

#if defined(MA_BLUE)
#include <cstdio>
#endif

#include "maUtility.h"

#include "maListGroup.h"
#include "maRealTimeList.h"
#include "maStepList.h"

using namespace std;

unordered_map<int, string> mapNumbersToNotes =
{
    {-1, ""},
    {0, "C"},
    {1, "C#"},
    {2, "D"},
    {3, "Eb"},
    {4, "E"},
    {5, "F"},
    {6, "Gb"},
    {7, "G"},
    {8, "Ab"},
    {9, "A"},
    {10, "Bb"},
    {11, "B"},
};

unordered_map<string, int> mapNotesToNumbers =
{

    {"R", -1}, {"Rest", -1}, {"r", -1}, {"rest", -1}, {"-", -1}, {"~", -1},

    {"B#0", 0},{"C0", 0},
    {"C#0", 1},{"Db0", 1},
    {"D0", 2},
    {"D#0", 3},{"Eb0", 3},
    {"E0", 4},{"Fb0", 4},
    {"E#0", 5},{"F0", 5},
    {"F#0", 6},{"Gb0", 6},
    {"G0", 7},
    {"G#0", 8},{"Ab0", 8},
    {"A0", 9},
    {"A#0", 10},{"Bb0", 10},
    {"B0", 11},{"Cb0", 11},

    {"B#1", 12},{"C1", 12},
    {"C#1", 13},{"Db1", 13},
    {"D1", 14},
    {"D#1", 15},{"Eb1", 15},
    {"E1", 16},{"Fb1", 16},
    {"E#1", 17},{"F1", 17},
    {"F#1", 18},{"Gb1", 18},
    {"G1", 19},
    {"G#1", 20},{"Ab1", 20},
    {"A1", 21},
    {"A#1", 22},{"Bb1", 22},
    {"B1", 23},{"Cb1", 23},

    {"B#2", 24},{"C2", 24},
    {"C#2", 25},{"Db2", 25},
    {"D2", 26},
    {"D#2", 27},{"Eb2", 27},
    {"E2", 28},{"Fb2", 28},
    {"E#2", 29},{"F2", 29},
    {"F#2", 30},{"Gb2", 30},
    {"G2", 31},
    {"G#2", 32},{"Ab2", 32},
    {"A2", 33},
    {"A#2", 34},{"Bb2", 35},
    {"B2", 35},{"Cb2", 35},


    {"B#3", 36},{"C3", 36},
    {"C#3", 37},{"Db3", 37},
    {"D3", 38},
    {"D#3", 39},{"Eb3", 39},
    {"E3", 40},{"Fb3", 40},
    {"E#3", 41},{"F3", 41},
    {"F#3", 42},{"Gb3", 42},
    {"G3", 43},
    {"G#3", 44},{"Ab3", 44},
    {"A3", 45},
    {"A#3", 46},{"Bb3", 46},
    {"B3", 47},{"Cb3", 47},

    {"B#4", 48},{"C4", 48},
    {"C#4", 49},{"Db4", 49},
    {"D4", 50},
    {"D#4", 51},{"Eb4", 51},
    {"E4", 52},{"Fb4", 52},
    {"E#4", 53},{"F4", 53},
    {"F#4", 54},{"Gb4", 54},
    {"G4", 55},
    {"G#4", 56},{"Ab4", 56},
    {"A4", 57},
    {"A#4", 58},{"Bb4", 58},
    {"B4", 59},{"Cb4", 59},

    {"B#5", 60},{"C5", 60},
    {"C#5", 61},{"Db5", 61},
    {"D5", 62},
    {"D#5", 63},{"Eb5", 63},
    {"E5", 64},{"Fb5", 64},
    {"E#5", 65},{"F5", 65},
    {"F#5", 66},{"Gb5", 66},
    {"G5", 67},
    {"G#5", 68},{"Ab5", 68},
    {"A5", 69},
    {"A#5", 70},{"Bb5", 70},
    {"B5", 71},{"Cb5", 71},

    // Note names without numbers default to middle octave

    {"C", 60},
    {"C#", 61},{"Db", 61},
    {"D", 62},
    {"D#", 63},{"Eb", 63},
    {"E", 64},{"Fb", 64},
    {"E#", 65},{"F", 65},
    {"F#", 66},{"Gb", 66},
    {"G", 67},
    {"G#", 68},{"Ab", 68},
    {"A", 69},
    {"A#", 70},{"Bb", 70},
    {"B", 71},{"Cb", 71},

    {"B#6", 72},{"C6", 72},
    {"C#6", 73},{"Db6", 73},
    {"D6", 74},
    {"D#6", 75},{"Eb6", 75},
    {"E6", 76},{"Fb6", 76},
    {"E#6", 77},{"F6", 77},
    {"F#6", 78},{"Gb6", 78},
    {"G6", 79},
    {"G#6", 80},{"Ab6", 80},
    {"A6", 81},
    {"A#6", 82},{"Bb6", 82},
    {"B6", 83},{"Cb6", 83},

    {"B#7", 84},{"C7", 84},
    {"C#7", 85},{"Db7", 85},
    {"D7", 86},
    {"D#7", 87},{"Eb7", 87},
    {"E7", 88},{"Fb7", 88},
    {"E#7", 89},{"F7", 89},
    {"F#7", 90},{"Gb7", 90},
    {"G7", 91},
    {"G#7", 92},{"Ab7", 92},
    {"A7", 93},
    {"A#7", 94},{"Bb7", 94},
    {"B7", 95},{"Cb7", 95},

    {"B#8", 96},{"C8", 96},
    {"C#8", 97},{"Db8", 97},
    {"D8", 98},
    {"D#8", 99},{"Eb8", 99},
    {"E8", 100},{"Fb8", 100},
    {"E#8", 101},{"F8", 101},
    {"F#8", 102},{"Gb8", 102},
    {"G8", 103},
    {"G#8", 104},{"Ab8", 104},
    {"A8", 105},
    {"A#8", 106},{"Bb8", 106},
    {"B8", 107},{"Cb8", 107},

    {"B#9", 108},{"C9", 108},
    {"C#9", 109},{"Db9", 109},
    {"D9", 110},
    {"D#9", 111},{"Eb9", 111},
    {"E9", 112},{"Fb9", 112},
    {"E#9", 113},{"F9", 113},
    {"F#9", 114},{"Gb9", 114},
    {"G9", 115},
    {"G#9", 116},{"Ab9", 116},
    {"A9", 117},
    {"A#9", 118},{"Bb9", 118},
    {"B9", 119},{"Cb9", 119},

    {"B#10", 120},{"C10", 120},
    {"C#10", 121},{"Db10", 121},
    {"D10", 122},
    {"D#10", 123},{"Eb10", 123},
    {"E10", 124},{"Fb10", 124},
    {"E#10", 125},{"F10", 125},
    {"F#10", 126},{"Gb10", 126},
    {"G10", 127}
};

//
// Note
//
//////////////////////////////////////////////////////////////////////////////////

const char * Note::NoteNameOnly(int n)
{
    if ( n >= 0 )
        return mapNumbersToNotes.at(n % 12).c_str();
    else
        return "~";
}

string Note::NoteString(int n)
{
    if ( n >= 0 )
    {
        char buffer[25];
        snprintf(buffer, 25, "%s%i", mapNumbersToNotes.at(n % 12).c_str(), n / 12);
        return buffer;
    }
    else
        return "~";
}

int Note::NoteNumberFromString(string note)
{
#ifdef MA_BLUE
    if ( mapNotesToNumbers.count(note.c_str()) == 1 )
        return mapNotesToNumbers.at(note.c_str());
    else
        return -1;
#else
    try
    {
        return mapNotesToNumbers.at(note.c_str());
    }
    catch (...)
    {
        return -1;
    }
#endif
}


string Note::ToString(bool fullFormat)
{
    if ( m_NoteNumber == -1 )
        return "~";

    char buffer[25];
    if ( fullFormat && m_NoteVelocity >= 0 )
        snprintf(buffer, 25, "%s%i:%i:%.3f:%.3f",
            mapNumbersToNotes.at(m_NoteNumber % 12).c_str(),
            m_NoteNumber / 12,
            m_NoteVelocity,
            m_Phase,
            m_Length);
    else
        snprintf(buffer, 25, "%s%i", mapNumbersToNotes.at(m_NoteNumber % 12).c_str(), m_NoteNumber / 12);

    return buffer;
}

bool Note::NoteFromString(string s)
{

    // We may have leading white space, which will throw the map look-up,
    // so convert separators to white space and then parse on that.

    replace( s.begin(), s.end(), ':', ' ');
    vector<string> tokens = split(s.c_str(), ' ');

    if ( tokens.size() >= 1 )
    {
        if ( mapNotesToNumbers.count(tokens.at(0)) == 0 )
            return false;
        m_NoteNumber = mapNotesToNumbers.at(tokens.at(0));
    }
    else
        m_NoteNumber = -1;

    if ( tokens.size() >= 2 )
    {
        m_NoteVelocity = strtol(tokens.at(1).c_str(), NULL, 0);
        if ( m_NoteVelocity < 0 || m_NoteVelocity > 127 )
            m_NoteVelocity = -1;
    }
    else
        m_NoteVelocity = -1;

    if ( tokens.size() >= 3 )
        m_Phase = strtod(tokens.at(2).c_str(), NULL);
    else
        m_Phase = 0.0;

    if ( tokens.size() >= 4 )
        m_Length = strtod(tokens.at(3).c_str(), NULL);
    else
        m_Length = 0.0;

    return true;
}

void Note::SetStatus()
{
    int pos = 0;
    char buff[200];

    m_FieldPositions.clear();
    m_Highlights.clear();

    if ( m_GotFocus )
        snprintf(buff, 200, "[Note %i] ", m_ItemID);
    else
        snprintf(buff, 200, " Note %i  ", m_ItemID);

    InitStatus();
    m_Status += buff;

    pos = m_Status.size();
    m_Status += ToString(false);
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += ", Vel ";
    pos = m_Status.size();
    if ( m_NoteVelocity > 0 )
    {
        snprintf(buff, 200, "%i", m_NoteVelocity);
        m_Status += buff;
    }
    else
        m_Status += "Off";
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += ", Len ";
    pos = m_Status.size();
    if ( lround(100 * m_Length) > 0 )
    {
        snprintf(buff, 200, "%.2f", m_Length);
        m_Status += buff;
    }
    else
        m_Status +=  "Off";
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Highlights.push_back(m_FieldPositions.at(m_NoteEditFocus));

}

bool Note::HandleKey(BaseUI::key_command_t k)
{
    double inc = 0.1;
    int note_inc = 1;
    note_edit_menu_focus_t editFocus;

    if ( m_Visible )
        editFocus = m_NoteEditFocus;
    else
        editFocus = nef_note_number;

    switch ( k )
    {
    case BaseUI::key_cmd_back:
        if ( m_MenuListPtr != NULL )
        {
            SetVisible(false);
            m_MenuListPtr->m_Container->SetRedraw();
            m_MenuListPtr->DownCursorPos();
            m_MenuListPtr->Remove(m_PosInMenuList);
        }
        break;

#if 0
    case BaseUI::key_cmd_up:
        if ( m_MenuListPtr != NULL )
            m_MenuListPtr->DownCursorPos();
        return true;

    case BaseUI::key_cmd_down:
        if ( m_MenuList != NULL )
            m_MenuList->UpCursorPos();
        return true;
#endif

    case BaseUI::key_cmd_left:
        if ( m_NoteEditFocus > 0 )
            m_NoteEditFocus = static_cast<note_edit_menu_focus_t>(m_NoteEditFocus - 1);
        break;

    case BaseUI::key_cmd_right:
        if ( m_NoteEditFocus < num_nef_types - 1 )
            m_NoteEditFocus = static_cast<note_edit_menu_focus_t>(m_NoteEditFocus + 1);
        break;

    case BaseUI::key_cmd_inc_2:
        inc = 0.01;
        note_inc = 12;
    case BaseUI::key_cmd_inc:
        switch ( editFocus )
        {
        case nef_note_number:
            if ( m_NoteNumber + note_inc < 128 )
                m_NoteNumber += note_inc;
            break;
        case nef_velocity:
            if ( m_NoteVelocity < 127 )
                m_NoteVelocity += 1;
            break;
        case nef_length:
            m_Length += inc;
            break;
        default:
            break;
        }
//        if ( m_ReturnFocus != NULL )
//        {
//            m_ReturnFocus->SetRedraw();
//        }
        break;

    case BaseUI::key_cmd_dec_2:
        inc = 0.01;
        note_inc = 12;
    case BaseUI::key_cmd_dec:
        switch ( editFocus )
        {
        case nef_note_number:
            if ( m_NoteNumber - note_inc >= -1 )
                m_NoteNumber -= note_inc;
            break;
        case nef_velocity:
            if ( m_NoteVelocity > 0 )
                m_NoteVelocity -= 1;
            break;
        case nef_length:
            m_Length -= inc;
            if ( m_Length < 0 )
                m_Length = 0;
            break;
        default:
            break;
        }
//        if ( m_ReturnFocus != NULL )
//            m_ReturnFocus->SetRedraw();
        break;

    default:
        return false;
    }

    m_FirstField = m_NoteEditFocus == 0;

    SetRedraw(m_NoteEditFocus == nef_note_number);

    return true;
}

void Note::AdjustPhase( double multiplier, double phase, double globalPhase, double base )
{
    double phaseOffset = (phase - m_Phase - base) / multiplier;
    m_Phase = globalPhase - phaseOffset;
}


bool Note::IncrementAndCheckTarget()
{
    if ( equals_3(m_Inc, 0) || equals_3(m_Target, 0) )
        return false;

    m_Moved += m_Inc;

    return m_Moved >= m_Target;
}


//
// Cluster
//
///////////////////////////////////////////////////////////////////

bool Cluster::IsRest()
{
    if ( Empty() )
        return true;

//    for ( vector<int>::size_type i = 0; i < m_Notes.size(); i++ )
//        if ( m_Notes[i].m_NoteNumber >= 0 )
//            return false;
    for ( auto n = m_Notes.begin(); n != m_Notes.end(); n++ )
        if ( n->m_NoteNumber >= 0 )
            return false;

    return true;
}

void Cluster::SetPhaseAllNotes(double phase)
{
    for ( auto n = m_Notes.begin(); n != m_Notes.end(); n++ )
        n->SetPhase(phase);
}

string Cluster::ToString(bool fullFormat)
{
    string result;
    for ( vector<Note>::iterator i = m_Notes.begin(); i != m_Notes.end(); i++ )
    {
        if ( result.size() > 0 )
            result += '/';
        result += i->ToString(fullFormat);
    }
    return result;
}

bool Cluster::ClusterFromString(string s)
{
    replace( s.begin(), s.end(), '/', ' ');         // Support both spacers.
    vector<string> noteStrings = split(s.c_str());

    if ( noteStrings.size() == 0 )
    {
        // Empty notes are rests, so leave the note list empty.
        return true;
    }

    for ( vector<string>::iterator it = noteStrings.begin(); it != noteStrings.end(); it++ )
    {
        Note n;
        if ( n.NoteFromString(*it) )
            m_Notes.push_back(n);
    }

    return ! m_Notes.empty();
}

void Cluster::SetStatus()
{
    int pos = 0;
    char buff[50];

    m_FieldPositions.clear();
    m_Highlights.clear();

    if ( m_GotFocus )
        snprintf(buff, 50, "[Cluster %i] ", m_ItemID);
    else
        snprintf(buff, 50, " Cluster %i  ", m_ItemID);

    InitStatus();
    m_Status += buff;

    for ( unsigned i = 0; i < m_Notes.size(); i++ )
    {
        if ( i > 0 )
            m_Status += '/';
        pos = m_Status.size();
        m_Status += m_Notes.at(i).ToString(false);
        m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));
    }

    if ( m_GotFocus && !m_FieldPositions.empty() )
        m_Highlights.push_back(m_FieldPositions.at(m_PosEdit));

}

bool Cluster::HandleKey(BaseUI::key_command_t k)
{
    switch ( k )
    {
    case BaseUI::key_cmd_enter:
        if ( !m_Notes.empty() )
        {
            Note & n = m_Notes.at(m_PosEdit);
            n.SetDisplayIndent(m_MenuListIndent + 2);
            n.SetItemID(m_PosEdit + 1);
            n.SetVisible(m_Visible);
            m_MenuListPtr->InsertAfter(m_PosInMenuList, & n, true);  // This selects it, too.
            n.SetReturnFocus(this);                         // Override return focus.
//            n.SetFocus();
//            MenuInsert(m_PosInMenuList, &n);
        }
        break;

    case BaseUI::key_cmd_back:
        if ( m_MenuListPtr != NULL )
        {
            SetVisible(false);
            m_MenuListPtr->m_Container->SetRedraw();
            m_MenuListPtr->DownCursorPos();
            m_MenuListPtr->Remove(m_PosInMenuList);
        }
        break;

#if 0
    case BaseUI::key_cmd_up:
        if ( m_MenuList != NULL )
            m_MenuList->DownCursorPos();
        return true;

    case BaseUI::key_cmd_down:
        if ( m_MenuList != NULL )
            m_MenuList->UpCursorPos();
        return true;
#endif

    case BaseUI::key_cmd_left:
        if ( m_PosEdit > 0 )
            m_PosEdit -= 1;
        break;

    case BaseUI::key_cmd_right:
        if ( m_PosEdit < m_Notes.size() - 1 )
            m_PosEdit += 1;
        break;

    case BaseUI::key_cmd_inc:
    case BaseUI::key_cmd_inc_2:
    case BaseUI::key_cmd_dec:
    case BaseUI::key_cmd_dec_2:
        if ( m_Notes.empty() )
        {
            m_Notes.emplace_back();
            m_PosEdit = 0;
        }
        m_Notes.at(m_PosEdit).HandleKey(k);
//        if ( m_ReturnFocus != NULL )
//            m_ReturnFocus->SetRedraw();
        break;

    case BaseUI::key_cmd_copy_left:
        CopyLeft();
        break;

    case BaseUI::key_cmd_copy_right:
        CopyRight();
        break;

    case BaseUI::key_cmd_insert_left:
        InsertLeft();
        break;

    case BaseUI::key_cmd_delete:
        DeleteNote();
        break;

    case BaseUI::key_cmd_insert_right:
        InsertRight();
        break;

    default:
        return false;
    }

    m_FirstField = m_PosEdit == 0;

    SetRedraw(true);

    return true;
}

void Cluster::InsertLeft()
{
    if ( m_Notes.empty() )
    {
        m_Notes.emplace_back();
        m_PosEdit = 0;
    }
    else
        m_Notes.insert(m_Notes.begin() + m_PosEdit, *(new Note()));
}

void Cluster::InsertRight()
{
    if ( m_Notes.empty() )
    {
        m_Notes.emplace_back();
        m_PosEdit = 0;
    }
    else
    {
        m_Notes.insert(m_Notes.begin() + m_PosEdit + 1, *(new Note()));
        m_PosEdit += 1;
    }
}

void Cluster::CopyLeft()
{
    if ( !m_Notes.empty() )
    {
        m_Notes.insert(m_Notes.begin() + m_PosEdit, m_Notes.at(m_PosEdit));
    }
}

void Cluster::CopyRight()
{
    if ( !m_Notes.empty() )
    {
        m_Notes.insert(m_Notes.begin() + m_PosEdit + 1, m_Notes.at(m_PosEdit));
        m_PosEdit += 1;
    }
}

void Cluster::DeleteNote()
{
    if ( !m_Notes.empty() )
    {
        m_Notes.erase(m_Notes.begin() + m_PosEdit);
        if ( m_PosEdit == m_Notes.size() )
            m_PosEdit -= 1;
    }
}

