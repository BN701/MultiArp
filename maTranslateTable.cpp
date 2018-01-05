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

#include "maTranslateTable.h"

#include <vector>
#include <string>
#include <cstring>  // For strlen
#include <unordered_map>
#include <algorithm>

#include "maNotes.h"
#include "maPatternStore.h"
#include "maUtility.h"

// These tables aren't needed at the moment, but
// could perhaps be used for some other experiments
// with strange mappings ...

const char * tt_major = {
"0,,2,,4,5,,7,,9,,11\n \
0,,2,,4,5,,7,,9,,11\n \
0,,2,,3,5,,7,,9,,10\n \
0,,2,,4,5,,7,,9,,11\n \
0,,1,,3,5,,7,,8,,10\n \
0,,2,,4,6,,7,,9,,11\n \
0,,2,,4,5,,7,,9,,11\n \
0,,2,,4,5,,7,,9,,10\n \
0,,2,,4,5,,7,,9,,11\n \
0,,2,,3,5,,7,,8,,10\n \
0,,2,,4,5,,7,,9,,11\n \
0,,1,,3,5,,6,,8,,10"
};

const char * tt_natural_minor = {
"0,,2,3,,5,,7,8,,10,\n \
0,,,,,,,,,,,\n \
0,,1,3,,5,,6,8,,10,\n \
0,,2,4,,5,,7,9,,11,\n \
0,,,,,,,,,,,\n \
0,,2,3,,5,,7,9,,10,\n \
0,,,,,,,,,,,\n \
0,,1,3,,5,,7,8,,10,\n \
0,,2,4,,6,,7,9,,11,\n \
0,,,,,,,,,,,\n \
0,,2,4,,5,,7,9,,10,\n \
0,,,,,,,,,,,"
};


const char * tt_harmonic_minor = {
"0,,2,3,,5,,7,8,,,11\n \
0,,,,,,,,,,,\n \
0,,1,3,,5,,6,9,,,10\n \
0,,2,4,,5,,8,9,,,11\n \
0,,,,,,,,,,,\n \
0,,2,3,,6,,7,9,,,10\n \
0,,,,,,,,,,,\n \
0,,1,4,,5,,7,8,,,10\n \
0,,3,4,,6,,7,9,,,11\n \
0,,,,,,,,,,,\n \
0,,,,,,,,,,,\n \
0,,1,3,,4,,6,8,,,9"
};

const char * tt_numerals[] = {"I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X", "XI", "XII"};
const char * tt_intervals[] = {"I", "IIb", "II", "IIIb", "III", "IV", "Vb", "V", "VIb", "VI",
        "VIIb", "VII", "VIII", "IXb", "IX", "Xb", "X", "XI", "XIIb", "XII"};




using namespace std;

enum tt_element_names_t
{
    tt_scale_heading,
    tt_name_scale,
    tt_name_accidentals,
    tt_name_premap,
    tt_name_root,
    tt_name_chromatic,
    tt_name_shift,
    tt_name_degrees,
    num_tt_element_names
};


unordered_map<tt_element_names_t, const char *> tt_element_names = {
    {tt_scale_heading, "Scale"},
    {tt_name_scale, "scale"},
    {tt_name_accidentals, "accidentals"},
    {tt_name_premap, "premap"},
    {tt_name_root, "root"},
    {tt_name_chromatic, "chrom"},
    {tt_name_shift, "shift"},
    {tt_name_degrees, "degrees"},
    {num_tt_element_names, ""}
};


unordered_map<scale_t, const char*>  tt_scale_strings = {
    {major, "2, 2, 1, 2, 2, 2, 1"},
    {harmonic_minor, "2, 1, 2, 2, 1, 3, 1"},
    {major_dorian, "2, 1, 2, 2, 2, 1, 2"},
    {major_phrygian, "1, 2, 2, 2, 1, 2, 2"},
    {major_lydian, "2, 2, 2, 1, 2, 2, 1"},
    {major_mixolydian, "2, 2, 1, 2, 2, 1, 2"},
    {natural_minor, "2, 1, 2, 2, 1, 2, 2"},
    {major_locrian, "1, 2, 2, 1, 2, 2, 2"},
    {major_pentatonic, "2, 2, 3, 2, 3"},
    {minor_pentatonic, "3, 2, 2, 3, 2"},
    {major_blues, "2, 1, 1, 3, 2, 3"},
    {minor_blues, "3, 2, 1, 1, 3, 1"}
};

unordered_map<scale_t, const char *> tt_scale_names = {
    {major, "Major"},
    {natural_minor, "Natural minor"},
    {harmonic_minor, "Harmonic minor"},
    {major_dorian, "Major dor"},
    {major_phrygian, "Major phryg"},
    {major_lydian, "Major lyd"},
    {major_mixolydian, "Major mixolyd"},
    {major_locrian, "Major loc"},
    {major_pentatonic, "Major pen"},
    {minor_pentatonic, "Minor pen"},
    {major_blues, "Major blues"},
    {minor_blues, "Minor blues"},
    {scroll_top, "(Scroll Top)"},
    {from_list, "From list"},
    {scale_unknown, "Undefined"}
};


scale_t tt_scale_lookup(string s)
{
    static unordered_map<string, scale_t>  map;

    // Initialize on first use.

    if ( map.size() == 0 )
        for ( scale_t m = static_cast<scale_t>(0);
              m < num_scale_types;
              m = static_cast<scale_t>(static_cast<int>(m) + 1) )
        {
            map.insert(std::make_pair(tt_scale_names.at(m), m));
        }

    return map.at(s);
}

unordered_map<string, scale_t> tt_scale_tags = // Possible tags setting ID from command input.
{
    {"user", from_list},
    {"major", major},
    {"minor", natural_minor},
    {"natural", natural_minor},
    {"n", natural_minor},
    {"harmonic", harmonic_minor},
    {"h", harmonic_minor},
    {"pentatonic major", major_pentatonic},
    {"pentatonic", minor_pentatonic},
    {"blues major", major_blues},
    {"blues", minor_blues}
};

unordered_map<accidentals_mode_t, const char *> tt_accidentals_mode_names =
{
    {accmode_upper, "upper"},
    {accmode_lower, "lower"},
    {upper_mute_if_clash, "up+mute"},
    {lower_mute_if_clash, "lo+mute"}
};

accidentals_mode_t tt_accidentals_mode_lookup(string s)
{
    static unordered_map<string, accidentals_mode_t> map;

    // Initialize on first use.

    if ( map.size() == 0 )
        for ( accidentals_mode_t m = static_cast<accidentals_mode_t>(0);
              m < num_accidentals_modes;
              m = static_cast<accidentals_mode_t>(static_cast<int>(m) + 1) )
        {
            map.insert(std::make_pair(tt_accidentals_mode_names.at(m), m));
        }

    return map.at(s);
}


unordered_map<scale_premap_mode_t, const char *> tt_premap_mode_names =
{
    {premap_off, "off"},
    {premap_mute, "mute"},
    {premap_leave, "leave"},
    {premap_upper, "upper"},
    {premap_lower, "lower"},
    {num_premap_modes, ""}
};

scale_premap_mode_t tt_premap_mode_lookup(string s)
{
    static unordered_map<string, scale_premap_mode_t> map;

    // Initialize on first use.

    if ( map.size() == 0 )
        for ( scale_premap_mode_t m = static_cast<scale_premap_mode_t>(0);
              m < num_premap_modes;
              m = static_cast<scale_premap_mode_t>(static_cast<int>(m) + 1) )
        {
            map.insert(std::make_pair(tt_premap_mode_names.at(m), m));
        }

    return map.at(s);
}

TranslateTable::TranslateTable():
    m_TranslateTableFocus(chromatic_shift)
{
    //ctor

    m_NoteMap.resize(12);

    Reset();
    SetupScaleMaps();

#if 0
    LoadTableFromString(tt_major);
    LoadTableFromString(tt_natural_minor);
    LoadTableFromString(tt_harmonic_minor);
#endif

}

TranslateTable::~TranslateTable()
{
    //dtor
}

void TranslateTable::Reset()
{
    m_Root = 60;
    m_Transpose = 0;
    m_DegreeShift = 0;
    m_DegreeShiftSemis = 0;
    m_Scale = major;
    m_ScaleDegrees = 0;
    m_AccidentalsMode = lower_mute_if_clash;
    m_PremapMode = premap_off;
    m_NewTransposePending = false;
    m_NewTranspose = 0;
}

bool TranslateTable::HandleKey(key_type_t k)
{
    int temp;
    switch ( k )
    {
    case left:
        temp = static_cast<int>(m_TranslateTableFocus) - 1;
        if ( temp >= 0 && temp < num_tt_menu_focus_modes )
            m_TranslateTableFocus = static_cast<translate_table_menu_focus_t>(temp);
        break;
    case right:
        temp = static_cast<int>(m_TranslateTableFocus) + 1;
        if ( temp >= 0 && temp < num_tt_menu_focus_modes )
            m_TranslateTableFocus = static_cast<translate_table_menu_focus_t>(temp);
        break;
    case up:
        switch ( m_TranslateTableFocus )
        {
        case chromatic_shift:
            m_Transpose += 1;
            break;
        case tonal_shift:
            m_DegreeShift += 1;
            break;
        case root:
            m_Root += 1;
            break;
        case scale:
            NextScale(-1);
            break;
        case premap:
            NextPremapMode(-1);
            break;
        case accidentals:
            NextAccidentalsMode(-1);
            break;
        default:
            break;
        }
        break;
    case down:
        switch ( m_TranslateTableFocus )
        {
        case chromatic_shift:
            m_Transpose -= 1;
            break;
        case tonal_shift:
            m_DegreeShift -= 1;
            break;
        case root:
            m_Root -= 1;
            break;
        case scale:
            NextScale(1);
            break;
        case premap:
            NextPremapMode(1);
            break;
        case accidentals:
            NextAccidentalsMode(1);
            break;
        default:
            break;
        }
        break;
    default:
        return false;
    }

    SetStatus();

    return true;
}


void TranslateTable::FromString(string s)
{
    if ( s.find(tt_element_names.at(tt_scale_heading)) == string::npos )
        throw string("TranslateTable::FromString() - Not a valid field list.");

    for ( tt_element_names_t e = static_cast<tt_element_names_t>(1);
          e < num_tt_element_names;
          e = static_cast<tt_element_names_t>(static_cast<int>(e) + 1) )
    {
        string token = find_token(s, tt_element_names.at(e));

        if ( token.empty() )
            continue;

        try
        {
            switch (e)
            {
            case tt_name_scale:
                m_Scale = tt_scale_lookup(token);
                break;
            case tt_name_accidentals:
                m_AccidentalsMode = tt_accidentals_mode_lookup(token);
                break;
            case tt_name_premap:
                m_PremapMode = tt_premap_mode_lookup(token);
                break;
            case tt_name_root:
                m_Root = stoi(token);
                break;
            case tt_name_chromatic:
                m_Transpose = stoi(token);
                break;
            case tt_name_shift:
                m_DegreeShift = stoi(token);
                break;
            case tt_name_degrees:
                m_ScaleDegrees = stoi(token);
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

}

string TranslateTable::ToString(const char * prefix)
{
    char buff[100];
    string result;

    // Should automate this bit to cycle through the tt_element_names enumeration,
    // though deciding where to trigger the line break might be an annoying piece of trivia.

    if ( prefix != NULL )
    {
        result += prefix;
        result += ' ';
    }

    sprintf(buff, "%s\\\n    %s %i %s %i %s %i %s '%s'",
            tt_element_names.at(tt_scale_heading),
            tt_element_names.at(tt_name_root), m_Root,
            tt_element_names.at(tt_name_chromatic), m_Transpose,
            tt_element_names.at(tt_name_shift), m_DegreeShift,
            tt_element_names.at(tt_name_scale), tt_scale_names.at(m_Scale)
            );

    result += buff;
//    result += "\\\n    ";

//    if ( prefix != NULL )
//    {
//        result += prefix;
//        result += ' ';
//    }

//    sprintf(buff, "%s %s %i %s '%s' %s '%s'",
//            tt_element_names.at(tt_scale_heading),
//            tt_element_names.at(tt_name_degrees), m_ScaleDegrees,
//            tt_element_names.at(tt_name_accidentals), tt_accidentals_mode_names.at(m_AccidentalsMode),
//            tt_element_names.at(tt_name_premap), tt_premap_mode_names.at(m_PremapMode)
//            );

    sprintf(buff, "\\\n    %s %i %s '%s' %s '%s'",
            tt_element_names.at(tt_name_degrees), m_ScaleDegrees,
            tt_element_names.at(tt_name_accidentals), tt_accidentals_mode_names.at(m_AccidentalsMode),
            tt_element_names.at(tt_name_premap), tt_premap_mode_names.at(m_PremapMode)
            );

    result += buff;

    return result;
}

void TranslateTable::SetNewTransposePending( int val )
{
    m_NewTransposePending = true;
    m_NewTranspose = val;
}

bool TranslateTable::NewTransposePending()
{
    if ( m_NewTransposePending )
    {
        m_Transpose = m_NewTranspose;
        m_NewTransposePending = false;
        return true;
    }
    else
        return false;
}

void TranslateTable::SetScale(scale_t val)
{
    if ( val == from_list || (val >= major && val < scroll_top) )
    {
        m_Scale = val;
        SetupScaleMaps();
    }
    else
        throw int (-1); // Change this to something meaningful if we ever want to deal with it properly.
}

bool TranslateTable::SetScale(std::string & name)
{
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    try
    {
        SetScale(tt_scale_tags.at(name));
        return true;
    }
    catch ( std::out_of_range const & e )
    {
        // Just try the next thing.
    }

    try
    {
        SetScale(static_cast<scale_t>(strtol(name.c_str(), NULL, 10) - 1));
        return true;
    }
    catch (...)
    {
        return false;
    }

}


void TranslateTable::NextScale( int dir )
{
    // Move up and down list but don't wrap.

    int t = static_cast<int>(m_Scale) + dir;

    if ( t >= 0 && t < static_cast<int>(scroll_top))
    {
        m_Scale = static_cast<scale_t>(t);
        SetupScaleMaps();
    }
}

void TranslateTable::NextPremapMode( int dir )
{
    // Move up and down list but don't wrap.

    int t = static_cast<int>(m_PremapMode) + dir;

    if ( t >= 0 && t < static_cast<int>(num_premap_modes))
    {
        m_PremapMode = static_cast<scale_premap_mode_t>(t);
    }
}


bool TranslateTable::SetScale(StepList & list)
{
    bool foundSomething = false;
    int foundNotes[12];
    for ( int i = 0; i < 12; i++ )
        foundNotes[i] = 0;

    for ( vector<Cluster>::iterator ch = list.m_Clusters.begin(); ch < list.m_Clusters.end(); ch++ )
        for ( vector<Note>::iterator note = (*ch).m_Notes.begin(); note < (*ch).m_Notes.end(); note++ )
        {
            foundNotes[(*note).m_NoteNumber % 12] = 1;
            foundSomething = true;
        }

    if ( !foundSomething )
        return false;

    m_Root = 60;

    while ( foundNotes[0] == 0 )
    {
        for ( int i = 0; i < 11; i++ )
            foundNotes[i] = foundNotes[i+1];
        foundNotes[11] = 0;
        m_Root += 1;  // Adjust root else notes in the list will be off the scale.
    }

    m_Intervals.clear();
    for ( int i = 0; i < 12; i++ )
        m_NoteMap[i] = 0;

    m_Intervals.push_back(1);

    for ( int i = 1; i < 12; i++ )
    {
        if ( foundNotes[i] != 0 )
        {
            m_Intervals.push_back(1);
            m_NoteMap[i] = m_Intervals.size() - 1;
        }
        else
            m_Intervals.back() += 1;

    }

    m_Scale = from_list;
    m_ScaleDegrees = m_Intervals.size();

    return true;
}

void TranslateTable::SetupScaleMaps()
{
    m_Intervals.clear();

    for ( int i = 0; i < 12; i++ )
        m_NoteMap[i] = 0;


    vector<string> tokens = split(tt_scale_strings.at(m_Scale), ',', true);

    int totalIntervals = 0;
    for ( unsigned i = 0; i < tokens.size(); i++ )
    {
        try
        {
            int interval = std::strtol(tokens.at(i).c_str(), NULL, 10);
            m_Intervals.push_back(interval);
            m_NoteMap[totalIntervals] = i;
            totalIntervals += interval;
            if ( totalIntervals >= 12 )
                break;
        }
        catch(...)
        {
            // Do nothing and try the next one.
        }
    }

    if ( totalIntervals < 12 )
    {
        // Increase last interval to complete the octave.
        m_Intervals.back() += 12 - totalIntervals;
    }

    m_ScaleDegrees = m_Intervals.size();
}

bool TranslateTable::SetRoot(std::string & name)
{
    int n = Note::NoteNumberFromString(name);

    if ( n >= 0 )
    {
        m_Root = n;
        return true;
    }
    else
    {
        return false;
    }
}

void TranslateTable::NextAccidentalsMode( int dir )
{
    int m = static_cast<int>(m_AccidentalsMode) + dir;

    if ( m >= 0 && m < static_cast<int>(num_accidentals_modes) )
        m_AccidentalsMode = static_cast<accidentals_mode_t>(m);
}

string TranslateTable::ShowScale()
{
    string result;
    char buff[20];
    for ( vector<int>::iterator it = m_Intervals.begin(); it < m_Intervals.end(); it++ )
    {
        sprintf(buff, "%i", (*it));
        if ( !result.empty() )
            result += ", ";
        result += buff;
    }
    return result;
}

string TranslateTable::ShowNoteMap()
{
    string result = tt_numerals[0];
    char buff[20];
    for ( int i = 1; i < 12; i++ )
    {
        result += ' ';
        if ( m_NoteMap[i] != 0 )
        {
            sprintf(buff, "%s", tt_numerals[m_NoteMap[i]]);
            result += buff;
        }
        else result += '.';
    }
    return result;
}

void TranslateTable::SetStatus()
{
    char buff[200];

    m_Status.clear();
    m_FieldPositions.clear();
    m_Highlights.clear();

    int pos = 0;
    sprintf(buff, "Chr': %2i", m_Transpose);
    m_Status += buff;
    m_FieldPositions.emplace_back(5, static_cast<int>(m_Status.size()) - 5);

    pos = m_Status.size();
    sprintf(buff, ", Tonal: %2i (%s)", m_DegreeShift, ShiftName());
    m_Status += buff;
    m_FieldPositions.emplace_back(pos + 8, m_Status.size() - pos - 8);

    pos = m_Status.size();
    sprintf(buff, " %s", RootName().c_str());
    m_Status += buff;
    m_FieldPositions.emplace_back(pos + 1, m_Status.size() - pos - 1);

    pos = m_Status.size();
    sprintf(buff, "-%s", ScaleName());
    m_Status += buff;
    m_FieldPositions.emplace_back(pos + 1, m_Status.size() - pos - 1);

    pos = m_Status.size();
    sprintf(buff, ", premap %s", PremapModeName());
    m_Status += buff;
    m_FieldPositions.emplace_back(pos + 9, m_Status.size() - pos - 9);

    pos = m_Status.size();
    sprintf(buff, ", acc's %s", AccidentalsModeName());
    m_Status += buff;
    m_FieldPositions.emplace_back(pos + 8, m_Status.size() - pos - 8);

    m_Highlights.push_back(m_FieldPositions.at(m_TranslateTableFocus));

}

string TranslateTable::RootName()
{
    return Note::NoteString(m_Root);
}

const char * TranslateTable::ScaleName()
{
    try
    {
        return tt_scale_names.at(m_Scale);
    }
    catch (...)
    {
        return "unknown";
    }
}

const char * TranslateTable::PremapModeName()
{
    try
    {
        return tt_premap_mode_names.at(m_PremapMode);
    }
    catch (...)
    {
        return "unknown";
    }
}

const char * TranslateTable::AccidentalsModeName()
{
    try
    {
        return tt_accidentals_mode_names.at(m_AccidentalsMode);
    }
    catch (...)
    {
        return "unknown";
    }
}

const char * TranslateTable::ShiftName()
{
    if ( m_ScaleDegrees == 0 )
        return tt_numerals[0];

    int i = m_DegreeShift;
    while ( i < 0 )
        i += m_ScaleDegrees;

    return tt_numerals[i % m_ScaleDegrees];
}

void TranslateTable::LoadTableFromString(const char *s)
{
    m_Tables.emplace_back();

    matrix_t & table = m_Tables.at(m_Tables.size() - 1);

    table.resize( 12, vector<int>(12, 0) );

    vector<string> rows = split(s, '\n', true);

    if ( rows.size() == 0 )
        throw std::string("Translate Table parse error: No rows found.");

    for ( vector<string>::size_type row = 0; row < rows.size(); row++ )
    {
        vector<string> values = split(rows.at(row).c_str(), ',', true);

        if ( values.size() == 0 )
            throw string("Translate Table parse error: No values found in row.");

        for ( vector<string>::size_type col = 0; col < values.size(); col++ )
//#ifdef DEBUG
        {
            const char* s = values.at(col).c_str();//    if ( dir > 0 )
            int val = strtol(s, NULL, 10);
            table.at(row).at(col) = val;
        }
//#else
//            table.at(row).at(col) = strtol(values.at(col).c_str(), NULL, 10);
//#endif


    }

}


bool TranslateTable::CanTranslate(int scale, int translation)
{

    int transIndex = translation;

    while ( transIndex >= 12 )
        transIndex -= 12;

    while ( transIndex < 0 )//    if ( dir > 0 )
        transIndex += 12;

    if ( transIndex == 0 )
        return true;

    // Check the first row of the table to see if it's
    // been initialized for this transpose index.

    int t = m_Tables.at(scale).at(0).at(transIndex);
    return  t > 0;
}

void TranslateTable::FindNextTranslation(int dir)
{
    int t = m_DegreeShiftSemis;

    do
    {
        t += dir;
        if ( CanTranslate(m_Scale, t) )
            break;
    } while ( true );

    m_DegreeShiftSemis = t;
    m_DegreeShift += dir;
}

int TranslateTable::TranslateUsingTable(int root, int scale, int translation, int note)
{
    int noteIndex = note - root;
    int octave = 0; // Zero means note is in the same octave as the root.
    int transIndex = translation;

    // Only one of these loops will apply.

    while ( noteIndex >= 12 )
    {
        noteIndex -= 12;
        octave += 12;
    }

    while ( noteIndex < 0 )
    {
        octave -= 12;
        noteIndex += 12;
    }

    int octaveShift = 0;

    while ( transIndex >= 12 )
    {
        transIndex -= 12;
        octaveShift += 12;
    }

    while ( transIndex < 0 )
    {
        transIndex += 12;
        octaveShift -= 12;
    }

    return root + octave + octaveShift + noteIndex + m_Tables.at(scale).at(noteIndex).at(transIndex);
}

int TranslateTable::ShiftSum(int start, int degreeShift)
{
    int shift = 0;

    if ( m_ScaleDegrees == 0 )
        return shift;

    for ( int i = start; i < start + degreeShift; i++ )
        shift += m_Intervals[i % m_ScaleDegrees];

    return shift;
}

/*
    MapPair()

    Return minor or major (first or second semitone)
    of a consecutive pair, whichever one is set in the
    destination scale.  Seems to work for 2nds, 3rds, 6ths
    and 7ths.
  */

int MapPair(int note, vector<int> map, int start)
{
#if 1
    // Is this a simpler to follow method?

    if ( map.at(start) != 0 && map.at(start + 1) != 0 )
        return note;
    if ( map.at(start) != 0 )
        return start;
    else if ( map.at(start + 1) != 0 )
        return start + 1;
    else
        return -1;
#else
    if ( map[note] != 0 )
        return note;

    int n = note - start;

    if ( n == 0 && map[start + 1] != 0 )
        return note + 1;
    else if ( n == 1 && map[start] != 0 )
        return note - 1;
    else
        return -1;
#endif
}

int TranslateTable::PreMapScale(int note)
{
    switch ( note )
    {
        // Sevenths
        case 11:
        case 10:
            return MapPair(note, m_NoteMap, 10);

        // Sixths
        case 9:
        case 8:
            return MapPair(note, m_NoteMap, 8);

        // Fifths
        case 7:
            if ( m_NoteMap[7] != 0 )
                return 7;
            else if ( m_NoteMap[6] != 0)
                return 6;
            else
                return -1;

        // Dimished fifth/augmented fourth
        case 6:
            if ( m_NoteMap[5] == 0 && m_NoteMap[6] != 0 && m_NoteMap[7] != 0 ) // Lydian.
                return 7;
            else if ( m_NoteMap[5] != 0 && m_NoteMap[6] != 0 && m_NoteMap[7] == 0 ) // Locrian.
                return 5;
            else
                switch ( m_PremapMode )
                {
                    case premap_mute:
                        return -1;
                    case premap_leave:
                        return note;
                    case premap_upper:
                        return note + 1;
                    case premap_lower:
                        return note - 1;
                    default:
                        return note;
                }
            break;

        // Fourths
        case 5:
            if ( m_NoteMap[5] != 0 )
                return 5;
            else if ( m_NoteMap[6] != 0 )
                return 6;
            else
                return -1;

        // Thirds
        case 4:
        case 3:
            return MapPair(note, m_NoteMap, 3);

        // Seconds
        case 2:
        case 1:
            return MapPair(note, m_NoteMap, 1);

        // Root (and anything else, for safety.
        default:
            return note;
    }
}

void TranslateDiags::ResetLog()
{
    m_Log = "                 In Octave   Premap   Degree/Actual  Trns  Out\n";
    m_Entries = 0;
}

const char * Interval(int n)
{
    if ( n >= 0 )
        return tt_intervals[n % 12];
    else
        return "-";
}

string TranslateDiags::UpdateLog()
{
    char buff[200];
    string result;

    m_InOutPairs.emplace_back(m_NoteIn, m_NoteOut);

    //                          In   Octave   Premap Degree/Actual Chro Final
    sprintf(buff, " %3s %-10.10s %3s  %2i+%-2i %4s->%-4s %2i / %-2i = %-4s %3i    %-4s %-12s",
        Note::NoteString(m_Root).c_str(),
        tt_scale_names.at(m_Scale),
        Note::NoteString(m_NoteIn).c_str(),
        m_Octave,
        m_OctaveShift,
        Interval(m_Degree),
        Interval(m_DegreeAfterPremap),
        m_DegreeShift,
        m_ModalShift,
        Interval(m_DegreeAfterShift),
        m_Transpose,
        Note::NoteString(m_NoteOut).c_str(),
        m_InScale ? "" : "Accidental");

    m_Log += buff;
    m_Log += '\n';
    m_Entries += 1;

    return buff;
}

#if 1

// With diagnositics.

int TranslateTable::TranslateUsingNoteMap(int note, int degreeShiftOverride)
{
    m_Diags.Reset();
    m_Diags.m_Root = m_Root;
    m_Diags.m_NoteIn = note;
    m_Diags.m_Transpose = m_Transpose;
    m_Diags.m_Scale = m_Scale;
    m_Diags.m_PremapMode = m_PremapMode;
    m_Diags.m_AccidentalsMode = m_AccidentalsMode;

    // Make note a value relative to root.
    note -= m_Root;

    // Normalize note within single octave so that
    // m_Root + octave + note = original note value.

    int octave = 0; // Zero means note is in the same octave as the root.

    // (Only one of these loops will apply.)

    while ( note >= 12 )
    {
        note -= 12;
        octave += 12;
    }

    while ( note < 0 )
    {
        note += 12;
        octave -= 12;
    }

    m_Diags.m_Octave = octave/12;
    m_Diags.m_Degree = note;

    if ( m_PremapMode > premap_off )
    {
        note = PreMapScale(note);
        if ( note < 0 )
        {
            m_Diags.m_DegreeAfterPremap = note;
            m_Diags.UpdateLog();
            return -1;
        }
    }

    m_Diags.m_DegreeAfterPremap = note;

    // Similarly, normalise degree shift to single octave.

    int octaveShift = 0;
    int degreeShift = m_DegreeShift + degreeShiftOverride;

    while ( degreeShift >= m_ScaleDegrees )
    {
        degreeShift -= m_ScaleDegrees;
        octaveShift += 12;
    }

    while ( degreeShift < 0 )
    {
        degreeShift += m_ScaleDegrees;
        octaveShift -= 12;
    }

    m_Diags.m_OctaveShift = octaveShift/12;
    m_Diags.m_DegreeShift = degreeShift;

    /*
        m_NoteMap give us the degree, I, II, III, etc., of the note we're
        processing. This is the index, zero based, in m_Intervals.

        For the major scale ...

        m_NoteMap looks like this: [ 0, 0, 1, 0, 2, 3, 0, 4, 0, 5, 0, 6 ]
        m_Intervals looks liks this: [ 2, 2, 1, 2, 2, 2, 1 ]

        So, shift is the cumulative number of semitones we have to move the note up.
     */

    int shift = 0;

    // int degree = m_NoteMap[note];

    if ( note == 0 || m_NoteMap[note] > 0 )
    {
        m_Diags.m_InScale = true;
        shift = ShiftSum(m_NoteMap[note], degreeShift);
#if 0
        int degreeIndex = degree - 1;

        for ( int i = degreeIndex; i < degreeIndex + degreeShift; i++ )
            shift += m_Intervals[i % m_ScaleDegrees];
#endif
    }
    else
    {
        m_Diags.m_InScale = false;

        // Accidental (not in the scale).

        // int d = 0;
        int nLower = note;
        int nUpper = note;

        while ( nLower > 0 )
        {
            if ( m_NoteMap[nLower] != 0 )
                break;
            nLower -= 1;
        }

        while ( nUpper < 12 )
        {
            if ( m_NoteMap[nUpper] != 0 )
                break;
            nUpper += 1;
        }

        int dLower = m_NoteMap[nLower];
        int dUpper = m_NoteMap[nUpper % 12];

        int sLower = ShiftSum(dLower, degreeShift);
        int sUpper = ShiftSum(dUpper, degreeShift);

        bool mute = false;

        switch ( m_AccidentalsMode )
        {
            case accmode_upper:
                shift = sUpper;
                break;
            case accmode_lower:
                shift = sLower;
                break;
            case upper_mute_if_clash:
                if ( note + sUpper > nLower + sLower )
                    shift = sUpper;
                else
                    mute = true;
            case lower_mute_if_clash:
                if ( note + sLower < nUpper + sUpper )
                    shift = sLower;
                else
                    mute = true;
            default:
                break;
        }

        if ( mute )
        {
            m_Diags.UpdateLog();
            return -1;
        }
#if 0
        // Find next free accidental slot.

        while ( d < degreeShift )
        {
            shift += 1;
            if ( m_NoteMap[++n % 12] == 0 )
                d += 1;
        }
#endif
    }

    m_Diags.m_ModalShift = shift;
    m_Diags.m_DegreeAfterShift = note + shift;
    m_Diags.m_NoteOut = m_Root + octave + note + octaveShift + shift + m_Transpose;

    m_Diags.UpdateLog();

    return m_Root + octave + note + octaveShift + shift + m_Transpose;
}
#endif

#if 0

int TranslateTable::TranslateUsingNoteMap(int note, int degreeShiftOverride)
{
    // Make note a value relative to root.
    note -= m_Root;

    // Normalize note within single octave so that
    // m_Root + octave + note = original note value.

    int octave = 0; // Zero means note is in the same octave as the root.

    // (Only one of these loops will apply.)

    while ( note >= 12 )
    {
        note -= 12;
        octave += 12;
    }

    while ( note < 0 )
    {
        note += 12;
        octave -= 12;
    }

    if ( m_PremapMode > premap_off )
    {
        note = PreMapScale(note);
        if ( note < 0 )
            return -1;
    }

    // Similarly, normalise degree shift to single octave.

    int octaveShift = 0;
    int degreeShift = degreeShiftOverride == 0 ? m_DegreeShift : degreeShiftOverride;

    while ( degreeShift >= m_ScaleDegrees )
    {
        degreeShift -= m_ScaleDegrees;
        octaveShift += 12;
    }

    while ( degreeShift < 0 )
    {
        degreeShift += m_ScaleDegrees;
        octaveShift -= 12;
    }

    /*
        m_NoteMap give us the degree, I, II, III, etc., of the note we're
        processing. This is the index, zero based, in m_Intervals.

        For the major scale ...

        m_NoteMap looks like this: [ 0, 0, 1, 0, 2, 3, 0, 4, 0, 5, 0, 6 ]
        m_Intervals looks liks this: [ 2, 2, 1, 2, 2, 2, 1 ]

        So, shift is the cumulative number of semitones we have to move the note up.
     */

    int shift = 0;

    // int degree = m_NoteMap[note];

    if ( note == 0 || m_NoteMap[note] > 0 )
    {
        shift = ShiftSum(m_NoteMap[note], degreeShift);
#if 0
        int degreeIndex = degree - 1;

        for ( int i = degreeIndex; i < degreeIndex + degreeShift; i++ )
            shift += m_Intervals[i % m_ScaleDegrees];
#endif
    }
    else
    {
        // Accidental (not in the scale).

        // int d = 0;
        int nLower = note;
        int nUpper = note;

        while ( nLower > 0 )
        {
            if ( m_NoteMap[nLower] != 0 )
                break;
            nLower -= 1;
        }

        while ( nUpper < 12 )
        {
            if ( m_NoteMap[nUpper] != 0 )
                break;
            nUpper += 1;
        }

        int dLower = m_NoteMap[nLower];
        int dUpper = m_NoteMap[nUpper % 12];

        int sLower = ShiftSum(dLower, degreeShift);
        int sUpper = ShiftSum(dUpper, degreeShift);

        switch ( m_AccidentalsMode )
        {
            case accmode_upper:
                shift = sUpper;
                break;
            case accmode_lower:
                shift = sLower;
                break;
            case upper_mute_if_clash:
                if ( note + sUpper > nLower + sLower )
                    shift = sUpper;
                else
                    return -1;
            case lower_mute_if_clash:
                if ( note + sLower < nUpper + sUpper )
                    shift = sLower;
                else
                    return -1;
            default:
                break;
        }

#if 0
        // Find next free accidental slot.

        while ( d < degreeShift )
        {
            shift += 1;
            if ( m_NoteMap[++n % 12] == 0 )
                d += 1;
        }
#endif
    }

    return m_Root + octave + note + octaveShift + shift + m_Transpose;
}
#endif

int TranslateTable::Translate(int note)
{
    // return TranslateUsingTable(m_Root, m_Scale, m_DegreeShiftSemis, note);
    return TranslateUsingNoteMap(note);
}

int TranslateTable::Translate2(int note, int interval)
{
    return TranslateUsingNoteMap(note, interval);
}



