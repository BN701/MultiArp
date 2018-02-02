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

#include "maUtility.h"

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

    {"R", -1}, {"Rest", -1}, {"-", -1},

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
        return "-";
}

string Note::NoteString(int n)
{
    if ( n >= 0 )
    {
        char buffer[25];
        sprintf(buffer, "%s%i", mapNumbersToNotes.at(n % 12).c_str(), n / 12);
        return buffer;
    }
    else
        return "-";
}

int Note::NoteNumberFromString(string note)
{
    try
    {
        return mapNotesToNumbers.at(note.c_str());
    }
    catch (...)
    {
        return -1;
    }
}


string Note::ToString(bool fullFormat)
{
    if ( m_NoteNumber == -1 )
        return "-";

    char buffer[25];
    if ( fullFormat && m_NoteVelocity >= 0 )
        sprintf(buffer, "%s%i:%i:%.3f:%.3f",
            mapNumbersToNotes.at(m_NoteNumber % 12).c_str(),
            m_NoteNumber / 12,
            m_NoteVelocity,
            m_Phase,
            m_Length);
    else
        sprintf(buffer, "%s%i", mapNumbersToNotes.at(m_NoteNumber % 12).c_str(), m_NoteNumber / 12);

    return buffer;
}

void Note::FromString(string s)
{

    // We may have leading white space, which will throw the map look-up,
    // so convert separators to white space and then parse on that.

    replace( s.begin(), s.end(), ':', ' ');
    vector<string> tokens = split(s.c_str(), ' ');

    if ( tokens.size() >= 1 )
        m_NoteNumber = mapNotesToNumbers.at(tokens.at(0));
    else
        m_NoteNumber = -1;

    if ( tokens.size() >= 2 )
    {
        m_NoteVelocity = stoi(tokens.at(1));
        if ( m_NoteVelocity < 0 || m_NoteVelocity > 127 )
            m_NoteVelocity = -1;
    }
    else
        m_NoteVelocity = -1;

    if ( tokens.size() >= 3 )
        m_Phase = stod(tokens.at(2));
    else
        m_Phase = 0.0;

    if ( tokens.size() >= 4 )
        m_Length = stod(tokens.at(3));
    else
        m_Length = 0.0;

}

void Note::SetStatus()
{
    int pos = 0;
    char buff[200];

    m_FieldPositions.clear();
    m_Highlights.clear();

    sprintf(buff, "[Note %i] ", m_ItemID);
    m_Status = buff;

    pos = m_Status.size();
    m_Status += ToString(false);
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += ", Vel ";
    pos = m_Status.size();
    if ( m_NoteVelocity > 0 )
    {
        sprintf(buff, "%i", m_NoteVelocity);
        m_Status += buff;
    }
    else
        m_Status += "Off";
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += ", Len ";
    pos = m_Status.size();
    if ( lround(100 * m_Length) > 0 )
    {
        sprintf(buff, "%.2f", m_Length);
        m_Status += buff;
    }
    else
        m_Status +=  "Off";
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Highlights.push_back(m_FieldPositions.at(m_NoteEditFocus));

}

bool Note::HandleKey(key_type_t k)
{
    double inc = 0.1;
    int note_inc = 1;

    switch ( k )
    {
    case enter:
    case back_space:
    case escape:
        ReturnFocus();
        break;

    case left:
        if ( m_NoteEditFocus > 0 )
            m_NoteEditFocus = static_cast<note_edit_menu_focus_t>(m_NoteEditFocus - 1);
        break;

    case right:
        if ( m_NoteEditFocus < num_nef_types - 1 )
            m_NoteEditFocus = static_cast<note_edit_menu_focus_t>(m_NoteEditFocus + 1);
        break;

    case shift_up:
        inc = 0.01;
        note_inc = 12;
    case up:
        switch ( m_NoteEditFocus )
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
        break;

    case shift_down:
        inc = 0.01;
        note_inc = 12;
    case down:
        switch ( m_NoteEditFocus )
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
        break;

    default:
        return false;
    }

    m_FirstField = m_NoteEditFocus == 0;

    SetStatus();

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

void Cluster::FromString(string s)
{
    replace( s.begin(), s.end(), '/', ' ');         // Support both spacers.
    vector<string> noteStrings = split(s.c_str());

    if ( noteStrings.size() == 0 )
    {
        // Empty notes are rests, so leave the note list empty.
        return;
    }

    for ( vector<string>::iterator it = noteStrings.begin(); it != noteStrings.end(); it++ )
    {
        Note n;
        try
        {
            n.FromString(*it);
            m_Notes.push_back(n);
        }
        catch (...)
        {
            // Do nothing with this, we just want to
            // carry on and try the next note.
        }

    }
}

Cluster * StepList::Step()
{
    if ( m_Clusters.empty() )
        return NULL;

    m_LastRequestedPos = m_Pos;
    Cluster *pCluster = & m_Clusters[m_Pos++];

    // Look ahead for rests.

    if ( !pCluster->IsRest() )
    {
        vector<int>::size_type p = m_Pos;
        pCluster->m_StepLength = 0;

        do
        {
            if ( p == m_Clusters.size() )
                p = 0;

            if ( m_Clusters[p++].IsRest() )
                pCluster->m_StepLength += 1;
            else
                break;

        } while ( true );
    }
    else
        pCluster = NULL;

    // Set completion flag.

    if ( m_Pos >= m_Clusters.size() )
    {
        m_Complete = true;
        m_Pos = 0;
    }
    else
    {
        m_Complete = false;
    }

    return pCluster;
}

void Cluster::SetStatus()
{
    int pos = 0;
    char buff[50];

    m_FieldPositions.clear();
    m_Highlights.clear();

    sprintf(buff, "[Cluster %i] ", m_ItemID);
    m_Status = buff;

    for ( unsigned i = 0; i < m_Notes.size(); i++ )
    {
        if ( i > 0 )
            m_Status += '/';
        pos = m_Status.size();
        m_Status += m_Notes.at(i).ToString(false);
        m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));
    }

    if ( !m_FieldPositions.empty() )
        m_Highlights.push_back(m_FieldPositions.at(m_PosEdit));

}

bool Cluster::HandleKey(key_type_t k)
{
    switch ( k )
    {
    case enter:
        if ( !m_Notes.empty() )
        {
            Note & n = m_Notes.at(m_PosEdit);
            n.SetItemID(m_PosEdit + 1);
            n.SetFocus();
            n.SetStatus();
            n.SetReturnFocus(this);
        }
        break;

    case back_space:
    case escape:
        ReturnFocus();
        break;

    case left:
        if ( m_PosEdit > 0 )
            m_PosEdit -= 1;
        break;

    case right:
        if ( m_PosEdit < m_Notes.size() - 1 )
            m_PosEdit += 1;
        break;

    case up:
        if ( m_ReturnFocus != NULL )
        {
            m_ReturnFocus->HandleKey(right);
            m_ReturnFocus->HandleKey(enter);
        }
        return true;

    case down:
        if ( m_ReturnFocus != NULL )
        {
            m_ReturnFocus->HandleKey(left);
            m_ReturnFocus->HandleKey(enter);
        }
        return true;

    case ctrl_left:
        if ( !m_Notes.empty() )
        {
            m_Notes.insert(m_Notes.begin() + m_PosEdit, m_Notes.at(m_PosEdit));
        }
        break;

    case ctrl_right:
        if ( !m_Notes.empty() )
        {
            m_Notes.insert(m_Notes.begin() + m_PosEdit + 1, m_Notes.at(m_PosEdit));
            m_PosEdit += 1;
        }
        break;

    case shift_left:
        if ( m_Notes.empty() )
        {
            m_Notes.emplace_back();
            m_PosEdit = 0;
        }
        else
            m_Notes.insert(m_Notes.begin() + m_PosEdit, *(new Note()));
        break;

    case shift_delete:
        if ( !m_Notes.empty() )
        {
            m_Notes.erase(m_Notes.begin() + m_PosEdit);
            if ( m_PosEdit == m_Notes.size() )
                m_PosEdit -= 1;
        }
        break;

    case shift_right:
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
        break;

    default:
        return false;
    }

    m_FirstField = m_PosEdit == 0;

    SetStatus();

    return true;
}

//
// StepList
//
//////////////////////////////////////////////////////////////////////////

string StepList::ToString(bool fullFormat)
{
    string result;
    for ( vector<Cluster>::iterator i = m_Clusters.begin(); i != m_Clusters.end(); )
    {
        if ( fullFormat && (i - m_Clusters.begin()) % 4 == 0 )
            result += " \\\n    ";
        result += Cluster(*i).ToString(fullFormat);
        if ( ++i < m_Clusters.end() )
            result += ", ";
    }
    return result;
}

string StepList::ToStringForDisplay(int & offset, int & length)
{
    string result;

    char buff[50];

    offset = 0;
    length = 0;

    for ( unsigned i = 0; i < m_Clusters.size(); i++ )
    {
        if ( i > 0 )
            result += ' ';
        if ( i == m_LastRequestedPos )
        {
            sprintf(buff, "%3lu| ", m_LastRequestedPos + 1);
            result += buff;
        }
        if ( i == 0 )
            offset = result.size();

        result += m_Clusters.at(i).ToString(false);

        if ( i == 0 )
            length = result.size() - offset;
    }

    return result;
}

string StepList::ToStringForDisplay2(int & offset, int & length, unsigned width)
{
    string result;

    offset = 0;
    length = 0;

    for ( unsigned i = 0; i < m_Clusters.size(); i++ )
    {
        if ( i > 0 )
            result += ' ';
        if ( i == m_LastRequestedPos )
        {
            offset = result.size();
        }
        result += m_Clusters.at(i).ToString(false);
        if ( i == m_LastRequestedPos )
        {
            length = result.size() - offset;
        }
    }

    // Scroll left if highlight is beyond width.

    while ( static_cast<unsigned>(offset + length) > width )
    {
        int scroll = 3 * width / 4;
        result.erase(0, scroll + 3);
        result.insert(0, "...");
        offset -= scroll;
    }

    // Truncate if line itself goes beyond width.

    if ( result.size() > width )
    {
        result = result.substr(0, width - 4);
        result += "... ";
    }

    return result;
}

void StepList::FromString(string s)
{
    vector<string> chordStrings = split(s.c_str(), ',', true);

    if ( chordStrings.size() == 0 )
        throw string("Note List parse error: nothing found.");

    Clear();

    for ( vector<string>::iterator it = chordStrings.begin(); it != chordStrings.end(); it++ )
    {
        Cluster chord;
        chord.FromString(*it);
        m_Clusters.push_back(chord);
    }
}

void StepList::SetStatus()
{
    int pos = 0;
    char buff[200];

    m_FieldPositions.clear();
    m_Highlights.clear();

    sprintf(buff, "[Step List %i] ", m_ItemID);
    m_Status = buff;


    for ( unsigned i = 0; i < m_Clusters.size(); i++ )
    {
        if ( i > 0 )
            m_Status += ", ";
        pos = m_Status.size();
        Cluster & c = m_Clusters.at(i);
        if ( !c.Empty() )
            m_Status += c.ToString(false);
        else
            m_Status += "(Empty)";
        m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));
    }

    if ( !m_FieldPositions.empty() )
        m_Highlights.push_back(m_FieldPositions.at(m_PosEdit));
}

bool StepList::HandleKey(key_type_t k)
{
    switch ( k )
    {
    case enter:
        if ( ! m_Clusters.empty() )
        {
            Cluster & c = m_Clusters.at(m_PosEdit);
            c.SetItemID(m_PosEdit + 1);
            c.SetFocus();
            c.SetStatus();
            c.SetReturnFocus(this);
        }
        break;

    case back_space:
    case escape:
        ReturnFocus();
        break;

    case left:
        if ( m_PosEdit > 0 )
            m_PosEdit -= 1;
        break;

    case right:
        if ( m_PosEdit < m_Clusters.size() - 1 )
            m_PosEdit += 1;
        break;

    case up:
    case down:
        if ( m_ReturnFocus != NULL )
        {
            m_ReturnFocus->HandleKey(k);
            m_ReturnFocus->HandleKey(enter);
        }
        return true;

    case ctrl_left:
        if ( !m_Clusters.empty() )
        {
            m_Clusters.insert(m_Clusters.begin() + m_PosEdit, m_Clusters.at(m_PosEdit));
        }
        break;

    case ctrl_right:
        if ( !m_Clusters.empty() )
        {
            m_Clusters.insert(m_Clusters.begin() + m_PosEdit + 1, m_Clusters.at(m_PosEdit));
            m_PosEdit += 1;
        }
        break;

    case shift_left:
        if ( m_Clusters.empty() )
        {
            m_Clusters.emplace_back();
            m_PosEdit = 0;
        }
        else
            m_Clusters.insert(m_Clusters.begin() + m_PosEdit, *(new Cluster()));
        break;

    case shift_right:
        if ( m_Clusters.empty() )
        {
            m_Clusters.emplace_back();
            m_PosEdit = 0;
        }
        else
        {
            m_Clusters.insert(m_Clusters.begin() + m_PosEdit + 1, *(new Cluster()));
            m_PosEdit += 1;
        }
        break;

    case shift_delete:
        if ( !m_Clusters.empty() )
        {
            m_Clusters.erase(m_Clusters.begin() + m_PosEdit);
            if ( m_PosEdit == m_Clusters.size() )
                m_PosEdit -= 1;
        }
        break;

    default:
        return false;
    }

    m_FirstField = m_PosEdit == 0;

    SetStatus();

    return true;
}


//
// RealTimeList
//
//////////////////////////////////////////////////////////////////////////

unordered_map<RealTimeListParams::rtp_window_mode_t, const char *> rtp_window_mode_names =
{
    {RealTimeListParams::normal, "centered"},
    {RealTimeListParams::small, "centered-small"},
    {RealTimeListParams::look_ahead, "ahead"}
};

RealTimeListParams::rtp_window_mode_t rtp_window_mode_lookup(string s)
{
    static unordered_map<string, RealTimeListParams::rtp_window_mode_t> map;

    // Initialize on first use.

    if ( map.size() == 0 )
        for ( RealTimeListParams::rtp_window_mode_t m = static_cast<RealTimeListParams::rtp_window_mode_t>(0);
              m < RealTimeListParams::num_rtp_window_modes;
              m = static_cast<RealTimeListParams::rtp_window_mode_t>(static_cast<int>(m) + 1) )
        {
            map.insert(std::make_pair(rtp_window_mode_names.at(m), m));
        }

    return map.at(s);
}

void RealTimeListParams::NextWindowMode( int dir )
{
    // Move up and down list but don't wrap.

    int t = static_cast<int>(m_WindowMode) + dir;

    if ( t >= 0 && t < static_cast<int>(num_rtp_window_modes))
    {
        m_WindowMode = static_cast<rtp_window_mode_t>(t);
    }
}


void RealTimeListParams::SetStatus()
{
    int pos = 0;
    char buff[200];

    m_FieldPositions.clear();
    m_Highlights.clear();

    sprintf(buff, "[RT List %i] ", m_ItemID);
    m_Status = buff;

    m_Status += " Loop - ";

    m_Status += "S: ";
    pos = m_Status.size();
    sprintf(buff, "%.2f", m_LoopStart);
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += " Q: ";
    pos = m_Status.size();
    sprintf(buff, "%.2f", m_Quantum);
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += " Mul: ";
    pos = m_Status.size();
    sprintf(buff, "%.2f", m_Multiplier);
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += " Window Mode: ";
    pos = m_Status.size();
    sprintf(buff, "%s", rtp_window_mode_names.at(m_WindowMode));
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Highlights.push_back(m_FieldPositions.at(m_RTParamsFocus));
}

bool RealTimeListParams::HandleKey(key_type_t k)
{
    int temp;
    double inc = 0.1;
    switch ( k )
    {
    case enter:
    case back_space:
    case escape:
        ReturnFocus();
        break;

    case left:
        temp = static_cast<int>(m_RTParamsFocus) - 1;
        if ( temp >= 0 && temp < num_rt_params_menu_focus_modes )
            m_RTParamsFocus = static_cast<rt_params_menu_focus_t>(temp);
        break;

    case right:
        temp = static_cast<int>(m_RTParamsFocus) + 1;
        if ( temp >= 0 && temp < num_rt_params_menu_focus_modes )
            m_RTParamsFocus = static_cast<rt_params_menu_focus_t>(temp);
        break;

    case del:
        switch ( m_RTParamsFocus )
        {
        case rtp_loop_start:
            m_LoopStart = 0;
            break;
        case rtp_local_quantum:
            m_Quantum = m_QuantumAtCapture;
            break;
        case rtp_multiplier:
            m_Multiplier = 1.0;
            break;
        default:
            break;
        }
        break;

    case shift_up:
        inc = 0.01;
    case up:
        switch ( m_RTParamsFocus )
        {
        case rtp_loop_start:
            m_LoopStart = tidy_3(m_LoopStart + inc);
            break;
        case rtp_local_quantum:
            m_Quantum = tidy_3(m_Quantum + inc);
            break;
        case rtp_multiplier:
            m_Multiplier = tidy_3(m_Multiplier + inc);
            break;
        case rtp_window_adjust:
            NextWindowMode(-1);
            break;
        default:
            break;
        }
        break;

    case shift_down:
        inc = 0.01;
    case down:
        switch ( m_RTParamsFocus )
        {
        case rtp_loop_start:
            m_LoopStart = tidy_3(m_LoopStart - inc);
            break;
        case rtp_local_quantum:
            m_Quantum = tidy_3(m_Quantum - inc);
            if ( m_Quantum < 0.01 )
                m_Quantum = 0.01;
            break;
        case rtp_multiplier:
            m_Multiplier = tidy_3(m_Multiplier - inc);
            break;
        case rtp_window_adjust:
            NextWindowMode(1);
            break;
        default:
            break;
        }
        break;

    default:
        return false;
    }

    m_FirstField = m_RTParamsFocus == 0;

    SetStatus();

    return true;
}

map<double,Note>::iterator RealTimeList::MoveNote(map<double, Note>::iterator it, double newPhase)
{
    // Avoid clashes.

    while ( true )
    {
        if ( m_RealTimeList.find(newPhase) == m_RealTimeList.end() )
            break;
        newPhase += 0.001;
    }

    // Make a copy of the note and erase the original.

    auto ret = m_RealTimeList.insert(make_pair(newPhase, it->second));
    m_RealTimeList.erase(it);

    // Increment phase.

    ret.first->second.SetPhase(newPhase);

    return ret.first;
}

map<double,Note>::iterator RealTimeList::CopyNote(map<double, Note>::iterator it)
{
    double newPhase = it->second.m_Phase + 0.001;

    while ( true )
    {
        if ( m_RealTimeList.find(newPhase) == m_RealTimeList.end() )
            break;
        newPhase += 0.001;
    }

    auto ret = m_RealTimeList.insert(make_pair(newPhase, it->second));
    ret.first->second.SetPhase(newPhase);
    return ret.first;
}


void RealTimeList::SetStatus()
{
    int pos = 0;
    char buff[200];

    m_FieldPositions.clear();
    m_Highlights.clear();

    sprintf(buff, "[RT List %i] ", m_ItemID);
    m_Status = buff;

    pos = m_Status.size();
    m_Status += "Params";
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    map<double, Note>::iterator it;

    for ( it = m_RealTimeList.begin(); it != m_RealTimeList.end(); it++ )
    {
        pos = m_Status.size() + 1; // "+ 1" because there's a space before the phase value.
        sprintf(buff, " %.2f:", it->first);
        m_Status += buff;
        m_Status += it->second.ToString(false);
        m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));
    }

    m_Highlights.push_back(m_FieldPositions.at(m_RTListFocus));

}

bool RealTimeList::HandleKey(key_type_t k)
{
    double inc = 0.1;

    map<double, Note>::iterator it = m_RealTimeList.begin();
    for ( int i = 0; i < m_RTListFocus - m_SubMenus; i++ )
        it++;   // There's no + operator on iterators for maps, apparently, so we move step-by-step.

    switch ( k )
    {
    case enter:
        if ( m_RTListFocus < m_SubMenus )
        {
            m_Params.SetItemID(m_ItemID);
            m_Params.SetFocus();
            m_Params.SetStatus();
            m_Params.SetReturnFocus(this);
        }
        else
        {
            Note & n = it->second;
            n.SetFocus();
            n.SetStatus();
            n.SetReturnFocus(this);
        }
        break;

    case back_space:
    case escape:
        ReturnFocus();
        break;

    case left:
        if ( m_RTListFocus > 0 )
            m_RTListFocus -= 1;
        break;

    case right:
        if ( static_cast<unsigned>(m_RTListFocus) < m_FieldPositions.size() - 1 )
            m_RTListFocus += 1;
        break;

    case shift_up:
    case shift_down:
        inc = 0.01;
    case up:
    case down:
        if ( m_RTListFocus < m_SubMenus && m_ReturnFocus != NULL )
        {
            m_ReturnFocus->HandleKey(k);
            m_ReturnFocus->HandleKey(enter);
            return true;
        }
        else if ( m_RTListFocus >= m_SubMenus )   // Update note start time (phase).
        {
            if ( k == down || k == shift_down )
                inc *= -1;

            auto result = MoveNote(it, it->second.Phase() + inc);

            // We get back an iterator to the new entry. Set menu
            // focus by counting backwards to start of map.

            m_RTListFocus = m_SubMenus;
            while ( m_RealTimeList.begin() != result-- )
                m_RTListFocus += 1;
        }
        break;

    case shift_delete:
        if ( m_RTListFocus >= m_SubMenus )
        {
            m_UndoList.push_back(it->second);
            m_RealTimeList.erase(it);
            if ( m_RTListFocus - m_SubMenus >= static_cast<int>(m_RealTimeList.size()) )
                m_RTListFocus -= 1;
        }
        break;

    case ctrl_delete:
        if ( !m_UndoList.empty() )
        {
            // Re-insert and reset our cursor position.

            Note & n = m_UndoList.back();

            pair<map<double,Note>::iterator, bool> result = m_RealTimeList.insert(make_pair(n.Phase(), n));

            if ( result.second )
            {
                m_RTListFocus = m_SubMenus;
                while ( m_RealTimeList.begin() != result.first-- )
                    m_RTListFocus += 1;
            }

            m_UndoList.pop_back();
        }
        break;

    default:
        return false;
    }


    m_FirstField = m_RTListFocus == 0;

    SetStatus();

    return true;
}

enum rtl_element_names_t
{
    rtl_name_loop,
    rtl_name_quantum,
    rtl_name_multiplier,
    rtl_name_window_adjust,
    num_rtl_element_names
};


unordered_map<rtl_element_names_t, const char *> rtl_element_names = {
    {rtl_name_loop, "Loop"},
    {rtl_name_quantum, "Quantum"},
    {rtl_name_multiplier, "Multiplier"},
    {rtl_name_window_adjust, "Window"},
    {num_rtl_element_names, ""}
};

void RealTimeList::FromString(string s)
{
    vector<string> tokens = split(s.c_str(), ',', true);

    if ( tokens.size() == 0 )
        throw string("Note List parse error: nothing found.");

    // Expect field list in first token ...

    for ( rtl_element_names_t e = static_cast<rtl_element_names_t>(0);
          e < num_rtl_element_names;
          e = static_cast<rtl_element_names_t>(static_cast<int>(e) + 1) )
    {
        string token = find_token(tokens.at(0), rtl_element_names.at(e));

        if ( token.empty() )
            continue;

        try
        {
            switch (e)
            {
            case rtl_name_loop:
                m_Params.m_LoopStart = stod(token);
                break;
            case rtl_name_quantum:
                m_Params.m_Quantum = stod(token);
                break;
            case rtl_name_multiplier:
                m_Params.m_Multiplier = stod(token);
                break;
            case rtl_name_window_adjust:
                m_Params.m_WindowMode = rtp_window_mode_lookup(token);
                break;
            default:
                break;
            }
        }
        catch(invalid_argument ex)
        {

        }
        catch(out_of_range ex)
        {
        }
    }

    // Anything after that is a note.

    if ( tokens.size() == 1 )
        return;  // Leave note list intact.

    m_RealTimeList.clear();

    for ( vector<string>::iterator it = tokens.begin() + 1; it != tokens.end(); it++ )
    {
        Note note;
        note.FromString(*it);

        // Check for existing entries and adjust start time if found.

        while ( true )
        {
//            map<double,Note>::iterator it2 = m_RealTimeList.find(note.Phase());
            if ( m_RealTimeList.find(note.Phase()) == m_RealTimeList.end() )
                break;
            note.SetPhase(note.Phase() + 0.0001);
        }

        m_RealTimeList.insert(make_pair(note.Phase(), note));
    }
}

string RealTimeList::ToString()
{
    string result;

    char buff[200];
    sprintf(buff, " %s %.3f %s %.3f %s %.3f %s '%s'",
            rtl_element_names.at(rtl_name_loop), m_Params.m_LoopStart,
            rtl_element_names.at(rtl_name_quantum), m_Params.m_Quantum,
            rtl_element_names.at(rtl_name_multiplier), m_Params.m_Multiplier,
            rtl_element_names.at(rtl_name_window_adjust), rtp_window_mode_names.at(m_Params.m_WindowMode)
            );

    result += buff;

    int i = 0;
    for (map<double,Note>::iterator it = m_RealTimeList.begin(); it != m_RealTimeList.end(); it++)
    {
        result += ", ";
        if ( i++ % 4 == 0 )
            result += "\\\n     ";
        result += it->second.ToString();
    }

    return result;
}

// Less efficient (probably) but easier to read (possibly) ...

string RealTimeList::ToStringForDisplay(int & offset, int & length, unsigned width)
{
    offset = 0;
    length = 0;

    char buff[100];
    sprintf(buff, "%05.2f ", m_LastRequestedPhase);
//    sprintf(buff, "%05.2f ", m_InternalBeat);

    string result = buff;

    long topLimit = lround(1000 * m_Params.m_QuantumAtCapture);
    long midLimit = lround(1000 * m_LastRequestedPhase);

    double windowPos = m_LastRequestedPhase;
    double windowStep = 4.0/m_LastRequestedStepValue;

    Cluster notes;

    while ( lround(1000 * windowPos) < topLimit )
    {
        double windowStart = windowPos - windowStep/2;
        double windowEnd = windowPos + windowStep/2;

        for ( map<double,Note>::iterator it = m_RealTimeList.lower_bound(windowStart);
                        it != m_RealTimeList.upper_bound(windowEnd); it++ )
            notes.Add(it->second);

        if ( !notes.Empty() )
        {
            result += notes.ToString(false);
            notes.Clear();
        }
        else
            result += '-';

        windowPos += windowStep;
    }

    windowPos = 0;

    while ( lround(1000 * windowPos) < midLimit )
    {
        double windowStart = windowPos - windowStep/2;
        double windowEnd = windowPos + windowStep/2;

        for ( map<double,Note>::iterator it = m_RealTimeList.lower_bound(windowStart);
                        it != m_RealTimeList.upper_bound(windowEnd); it++ )
            notes.Add(it->second);

        if ( windowPos == 0 )
            offset = result.size();

        if ( !notes.Empty() )
        {
            result += notes.ToString(false);
            notes.Clear();
        }
        else
            result += '-';

        if ( windowPos == 0 )
            length = result.size() - offset;

        windowPos += windowStep;
    }

    if ( result.size() > width )
        result.resize(width);

#if 0
    sprintf(buff, "\n    Multiplier %.2f, Loop Start %.2f, Loop Quantum %.2f, Window Adjust %s",
        m_Params.m_Multiplier,
        m_Params.m_LoopStart,
        m_Params.m_LocalQuantum,
        m_Params.m_AdjustWindowToStep ? "ON" : "OFF");
    result += buff;
#endif

    return result;
}


void RealTimeList::Step(Cluster & cluster, double phase, double stepValue)
{
    bool localLoop = lround(m_Params.m_Quantum) > 0;

    phase *= m_Params.m_Multiplier;

    // Wrap to start of local loop.

    if ( localLoop )
        while ( phase > m_Params.m_Quantum )
            phase -= m_Params.m_Quantum;

    phase += m_Params.m_LoopStart;

    // Wrap to start of capture loop loop.

    if ( localLoop )
        while ( phase > m_Params.m_QuantumAtCapture )
            phase -= m_Params.m_QuantumAtCapture;

    m_LastRequestedStepValue = stepValue;
    m_LastRequestedPhase = phase;

    double window = 4.0 * m_Params.m_Multiplier/stepValue;
    double windowStart, windowEnd;

    switch ( m_Params.m_WindowMode )
    {
    case RealTimeListParams::normal:
    case RealTimeListParams::small:
        if ( m_Params.m_WindowMode == RealTimeListParams::small && abs(m_Params.m_Multiplier) < 1.0 )
            window *= m_Params.m_Multiplier;

        windowStart = phase - window/2;
        windowEnd = phase + window/2;
        break;

    case RealTimeListParams::look_ahead:
        windowStart = phase;
        windowEnd = phase + window;
        break;

    default:
        break;
    }

    for ( map<double,Note>::iterator it = m_RealTimeList.lower_bound(windowStart);
                    it != m_RealTimeList.upper_bound(windowEnd); it++ )
    {
        cluster.Add(it->second);
    }

    // When phase is zero, window start will be negative, so we also need to
    // look for notes at the top of the loop that would normally be quantized
    // up to next beat zero.

//    if ( windowStart < 0 )
//    {
//        windowStart += m_LinkQuantum;
//        for ( map<double,Note>::iterator it = m_RealTimeList.lower_bound(windowStart);
//                    it != m_RealTimeList.upper_bound(m_LinkQuantum); it++ )
//            m_Captured.Add(it->second);
//    }

//    return result;

}


void RealTimeList::BeginEcho(double inc, double target, int interval)
{
    // Iterator here is a map pair.

    // Make a set up references to current map members before duplicating
    // them. Updating the map while we're traversing it leads to confusion!

    vector<map<double,Note>::iterator> notes;

    for ( auto p = m_RealTimeList.begin(); p != m_RealTimeList.end(); p++ )
        notes.push_back(p);

    for ( auto p = notes.begin(); p != notes.end(); p++ )
    {
        double newPhase = (*p)->second.m_Phase + inc;
        pair<map<double,Note>::iterator,bool> ret = m_RealTimeList.insert(make_pair(newPhase, (*p)->second));

        if ( ret.second )
        {
            Note & n = ret.first->second;

            n.SetPhase(newPhase);
            n.m_Inc = inc;
            n.m_Moved = inc;        // We already added increment to start phase, above.
            n.m_Target = target;
            n.m_Interval = interval;
            n.m_NoteNumber += interval;
        }
    }
}

enum rte_target_action_mode_t
{
    rteam_delete,
    rteam_freeze,
    rteam_echo_all,
    rteam_echo_all_delete,
    rteam_echo_all_freeze,
    rteam_copy,
    rteam_copy_freeze,
    rteam_copy_freeze_target_multiplier,
    num_rte_target_action_modes
};

bool RealTimeList::NoteTargetAction(map<double,Note>::iterator mapEntry, double newPhase)
{
    bool result = false;
    rte_target_action_mode_t mode = rteam_copy_freeze_target_multiplier;

    switch(mode)
    {
    case rteam_delete:
        m_RealTimeList.erase(mapEntry);
        return true;
    default:
        break;
    }

    Note * note = & mapEntry->second;

    // Echo tasks ...

    switch(mode)
    {
    case rteam_echo_all:
    case rteam_echo_all_delete:
    case rteam_echo_all_freeze:
        BeginEcho(note->m_Inc, note->m_Target, note->m_Interval);
        break;
    default:
        break;
    }

    // The current note is copied ...

    Note * newNote = NULL;

    switch(mode)
    {
    case rteam_copy:
    case rteam_copy_freeze:
    case rteam_copy_freeze_target_multiplier:
        newNote = & CopyNote(mapEntry)->second;
        break;
    default:
        break;
    }

    // Copied note simple initialisation, including interval transpose.

    switch(mode)
    {
    case rteam_copy:
    case rteam_copy_freeze:
    case rteam_copy_freeze_target_multiplier:
        newNote->m_Moved = 0;
        newNote->m_Phase = note->m_Phase + note->m_Inc;
        newNote->m_NoteNumber += newNote->m_Interval;
        break;
    default:
        break;
    }

    double targetMultiplier = 0.5;

    switch(mode)
    {
    case rteam_copy_freeze_target_multiplier:
        newNote->m_Target *= targetMultiplier;
        break;
    default:
        break;
    }
    // Delete or freeze the original note?

    switch(mode)
    {
    case rteam_echo_all_delete:
        m_RealTimeList.erase(mapEntry);
        result = true;
        break;
    case rteam_freeze:
    case rteam_copy_freeze:
    case rteam_copy_freeze_target_multiplier:
    case rteam_echo_all_freeze:
        note->m_Inc = 0;
        note->m_Moved = 0;
        break;
    default:
        break;
    }

    return result;
}

void RealTimeList::DoEchoes()
{
    // Build a list of iterators to notes to be changed. (We
    // shouldn't modify the map whilst traversing it!)

    vector<map<double,Note>::iterator> entries;

    for ( auto p = m_RealTimeList.begin(); p != m_RealTimeList.end(); p++ )
        if ( !equals_3(p->second.m_Inc, 0))
            entries.push_back(p);

    for ( auto p = entries.begin(); p != entries.end(); p++ )
    {
        Note & note = (*p)->second;

        double newPhase = note.m_Phase + note.m_Inc;

        if ( newPhase >= m_Params.m_Quantum )
        {
            if ( m_Params.m_EchoesDeleteAtQuantum )
            {
                m_RealTimeList.erase(*p);
                continue;
            }

            if ( m_Params.m_EchoesWrapAtQuantum )
                newPhase -= m_Params.m_Quantum;
        }

        // Have we reached the target?

        if ( note.IncrementAndCheckTarget() && NoteTargetAction(*p, newPhase) )
            continue;

        // MoveNote() inserts a copy at the new location. The previous
        // map entry and its note is deleted.

        /*Note & newNote =*/ MoveNote(*p, newPhase)->second;

        // Any further processing on new note? If so, do it here.


    }
}

unsigned long RealTimeList::PhaseLength()
{
    // Lets do this with integers, return the result in thousanths of a beat. (mBeat?)
    unsigned long adjustedQuantum = lround(1000.0 * m_Params.m_Multiplier * m_Params.m_Quantum);
    unsigned long quantum = lround(1000.0 * m_Params.m_Quantum);

    // Using lowest common multiple gives us the 'adjusted' result, which needs to be
    // adjusted back to pattern time.

    return lcm(quantum, adjustedQuantum)/adjustedQuantum * quantum;

    // This method is unreliable: fmod() seems to return
    // a whole value of m_Quantum for some values
    // of adjustedQuantum, which means we don't find the
    // lowest multiple. (Rounding errors, I guess.)

//    double t = adjustedQuantum;
//    int mod = 0;
//
//    do
//    {
//        t += adjustedQuantum;
//        mod = lround(1000.0 * fmod(t, m_Params.m_Quantum));
//    } while ( mod != 0 );

//    return 0.001 * static_cast<double>(t) / m_Params.m_Multiplier;
}

void RealTimeList::Step2(Cluster & cluster, double globalPhase, double stepValue, double patternBeat)
{
//    double altPhase = fmod(m_Params.m_Multiplier * patternBeat, m_Params.m_Quantum);

    long lTemp = lround(1000.0 * m_Params.m_Multiplier * patternBeat);
    long lPhase = lTemp % lround(1000.0 * m_Params.m_Quantum);
    double phase = static_cast<double>(lPhase)/1000;

    m_LastRequestedStepValue = stepValue;
    m_LastRequestedPhase = phase;
    m_LastStepPhaseZero = lPhase == 0;

    if ( m_LastStepPhaseZero )
        DoEchoes();

    // Window is defined in the list time scale, so use the multiplier.

    double window = 4.0 * m_Params.m_Multiplier/stepValue;
    double windowStart, windowEnd;

    switch ( m_Params.m_WindowMode )
    {
    case RealTimeListParams::normal:
    case RealTimeListParams::small:

        // This doesn't make sense: window becomes 4 * mul-squared/stepValue.
        if ( m_Params.m_WindowMode == RealTimeListParams::small && abs(m_Params.m_Multiplier) < 1.0 )
            window *= m_Params.m_Multiplier;

        windowStart = phase - window/2;
        windowEnd = phase + window/2;
        break;

    case RealTimeListParams::look_ahead:
        windowStart = phase;
        windowEnd = phase + window;
        break;

    default:
        windowStart = phase - window/2;
        windowEnd = phase + window/2;
        break;
    }

    // Swap round if negative multiplier (reverse play).

    if ( windowStart > windowEnd )
    {
        double t = windowStart;
        windowStart = windowEnd;
        windowEnd = t;
    }

    // Step window may overlap either end of the original capture range (m_Params.m_Quantum), or may even
    // be bigger than the capture window, in which case we need to repeat the captured data at new locations
    // within the step window for playback. Here, N represents the capture window as it slides over the step
    // window, starting with the value of N that contains windowStart, ending with the value of N that contains
    // windowEnd. For each position of N, we grab the relevant events and map them to their playback location in
    // the playback stream, marked by globalPhase.

    for ( int N = ceil(windowStart/m_Params.m_Quantum) - 1; N < ceil(windowEnd/m_Params.m_Quantum); N++ )
    {
        double rangeOffset = N * m_Params.m_Quantum;

        // Set bounds for the collection window applied to captured data.

        double lower = max(windowStart, rangeOffset) - rangeOffset;
        double upper = min(windowEnd, rangeOffset + m_Params.m_Quantum) - rangeOffset - 0.001;

        // (Taking off 0.001 makes sure we test against the element at the top of
        // the collection winow, as upper_bound() "Returns an iterator pointing to
        // the first element whose key is considered to go *after* k.")

        // Slide the collection window over the captured data. (This doesn't wrap if we slide
        // over the end of the capture range.)

        lower += m_Params.m_LoopStart;
        upper += m_Params.m_LoopStart;

//        lower = 0.001 * lround(lower * 1000);
//        upper = 0.001 * lround(upper * 1000);
        lower = tidy_3(lower);
        upper = tidy_3(upper);

        // Protect map from scans where lower > upper. This can happen due to rounding errors
        // and/or imprecise floating point data. (If lower > upper executing the loop below
        // seems to produce race conditions and/or stack corruption.)

        if ( lower >= upper )
            continue;

        for ( map<double,Note>::iterator it = m_RealTimeList.lower_bound(lower);
                        it != m_RealTimeList.upper_bound(upper); it++ )
            cluster.Add(it->second).AdjustPhase(m_Params.m_Multiplier, phase, globalPhase, rangeOffset);
    }


}
