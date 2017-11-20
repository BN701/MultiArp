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

#include <algorithm>
#include <climits>
#include <cmath>
#include <unordered_map>

#include "maTrigListItem.h"
#include "maUtility.h"

using namespace std;

//
// TrigRepeater
//
//////////////////////////////////////////////////////////////


void TrigRepeater::Init(double tempo, double stepLengthMilliSecs)
{
    m_Steps = 0;

    if ( lround(m_RepeatTime * 100) != 0 )
        m_RepeatTimeMicroSecs = 1000 * 60000.0 * m_RepeatTime/tempo;
    else
        m_RepeatTimeMicroSecs = 1000 * stepLengthMilliSecs/((double) m_Repeats + 1);

}

void TrigRepeater::Reset(/*int noteNumber,*/ int noteVelocity)
{
    m_Steps = m_Repeats;
//    m_NoteNumber = noteNumber;
    m_NoteVelocity = noteVelocity;
    m_VelocityDecrement = m_NoteVelocity * (1 - m_VelocityDecay);

    for ( auto it = m_Arpeggio.begin(); it < m_Arpeggio.end(); it++ )
        it->m_Position = it == m_Arpeggio.begin() ? 1 : 0;
}

bool TrigRepeater::Step(int64_t & queue_delta, int & interval, unsigned char & noteVelocity)
{
    if ( m_Steps == 0 )
        return false;

    queue_delta += m_RepeatTimeMicroSecs;

    switch ( m_DecayMode )
    {
    case exponential:
        m_Steps -= 1;
    case exponential_unlimited:
        m_NoteVelocity *= m_VelocityDecay;
        noteVelocity = m_NoteVelocity;
        break;
    case linear:
        m_Steps -= 1;
    case linear_unlimited:
        m_NoteVelocity -= m_VelocityDecrement;
        if ( m_NoteVelocity > 0 )
            noteVelocity = m_NoteVelocity;
        else
            noteVelocity = 0;
        break;
    case off:
        m_Steps -= 1;
        break;
    default:
        return false;
    }

    bool stepNext = true;
    interval = 0;

    for ( int i = 0; i < m_Arpeggio.size(); i ++ )
    {
        ArpeggioStage & s = m_Arpeggio.at(i);
        interval += s.m_Interval * s.m_Position;

        if ( stepNext )
        {
            s.m_Position += 1;
            if ( s.m_Position == s.m_Steps )
            {
                s.m_Position = 0;
                stepNext = true;
            }
            else
                stepNext = false;
        }
    }

    return noteVelocity > 0;
}

//
// TrigListItem
//
//////////////////////////////////////////////////////////////

TrigListItem::TrigListItem()
{
    //ctor
}

TrigListItem::~TrigListItem()
{
    //dtor
}

enum tli_element_names_t
{
    tli_trig_mask,
    tli_multiplier,
    tli_skip,
    tli_mute,
    tli_repeats,
    tli_repeat_time,
    num_tli_element_names
};


unordered_map<tli_element_names_t, const char *> tli_element_names = {
    {tli_trig_mask, "Trig Mask"},
    {tli_multiplier, "Multiplier"},
    {tli_skip, "Skip"},
    {tli_mute, "Mute"},
    {tli_repeats, "Repeats"},
    {tli_repeat_time, "Repeat Time"},
    {num_tli_element_names, ""}
};


string TrigListItem::ToString()
{
    char buff[200];

    sprintf(buff, "%s %i %s %.3f %s '%s' %s '%s' %s %i %s %.3f",
            tli_element_names.at(tli_trig_mask), m_TrigMask,
            tli_element_names.at(tli_multiplier), m_Multiplier,
            tli_element_names.at(tli_skip), m_Skip ? "On" : "Off",
            tli_element_names.at(tli_mute), m_Mute ? "On" : "Off",
            tli_element_names.at(tli_repeats), m_Repeater.Repeats(),
            tli_element_names.at(tli_repeat_time), m_Repeater.RepeatTime()
            );

    return buff;
}

void TrigListItem::FromString(string s)
{
    for ( tli_element_names_t e = static_cast<tli_element_names_t>(0);
          e < num_tli_element_names;
          e = static_cast<tli_element_names_t>(static_cast<int>(e) + 1) )
    {
        string token = find_token(s, tli_element_names.at(e));

        if ( token.empty() )
            continue;

        transform(token.begin(), token.end(), token.begin(), ::tolower);

        try
        {
            switch (e)
            {
            case tli_trig_mask:
                m_TrigMask = stoi(token);
                break;
            case tli_multiplier:
                m_Multiplier = stod(token);
            case tli_skip:
                m_Skip = token == "on";
                break;
            case tli_mute:
                m_Mute = token == "on";
                break;
            case tli_repeats:
                m_Repeater.SetRepeats( stoi(token) );
                break;
            case tli_repeat_time:
                m_Repeater.SetRepeatTime( stod(token) );
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

void TrigListItem::FromSimpleString(string s)
{
    transform(s.begin(), s.end(), s.begin(), ::tolower);

    if ( s == "all" )
    {
        m_TrigMask = ULONG_MAX;
        TrigsFromMask(m_TrigMask);
        return;
    }

    m_Trigs.clear();

    while ( s.size() > 0 )
    {
        try
        {
            int i = stoi(s);
            if ( i <= 64 )
                m_Trigs.push_back(i - 1);

            size_t pos = s.find('+');
            if ( pos == string::npos )
                break;
            s.erase(0, pos + 1);
        }
        catch (...)
        {
            break;
        }
    }

    m_TrigMask = MaskFromTrigs();

}

void TrigListItem::TrigsFromMask(unsigned long trigMask)
{
    unsigned long mask = 1;
    m_Trigs.clear();

    for ( int i = 0; i < CHAR_BIT * sizeof(mask); i++ )
    {
        if ( (trigMask & mask) > 0 )
            m_Trigs.push_back(i);
        mask = mask << 1;
    }

}

unsigned long TrigListItem::MaskFromTrigs()
{
    unsigned long result = 0;

    for ( auto it = m_Trigs.begin(); it < m_Trigs.end(); it++ )
    {
        result += (unsigned long) 1 << *it;
    }

    return result;
}

string TrigListItem::TrigMaskToString()
{
    if ( m_TrigMask == 0 )
        return "None";
    else if ( m_TrigMask == ULONG_MAX )
        return "All";
    else
    {
        string result;
        char buff[20];

        bool addSpacer = false;
        unsigned long mask = 1;

        for ( int i = 0; i < CHAR_BIT * sizeof(mask); i++ )
        {
            if ( (m_TrigMask & mask) > 0 )
            {
                sprintf(buff, "%s%i", addSpacer ? "+" : "", i + 1);
                result += buff;
                addSpacer = true;
            }
            mask = mask << 1;
        }

        return result;
    }
}

string TrigListItem::MenuString(int width)
{
    char buff[200];
    string result = TrigMaskToString();

    if ( lround(m_Multiplier * 100) != 100 )
    {
        sprintf(buff, "x%.2f", m_Multiplier);
        result += buff;
    }

    if ( m_Repeater.Repeats() > 0 )
    {
        sprintf(buff, "/R%i", m_Repeater.Repeats());
        result += buff;
    }

    if ( m_Skip )
        result += ",Skip";

    if ( m_Mute )
        result += ",Mute";

    if ( width > 0 && result.size() < width )
    {
        int pad = width - result.size();
        result.insert(0, pad/2, ' ');
        result.append(pad/2 + (pad % 2), ' ');
    }
    else if (width > 4 && result.size() > width )
    {
        result = result.substr(0, width - 3);
        result += "...";
    }

    return result;
}


void TrigListItem::SetStatus()
{
    int pos = 0;
    char buff[200];

    m_FieldPositions.clear();
    m_Highlights.clear();

    m_Status = "[Trigs] ";

    pos = m_Status.size();
    m_Status += TrigMaskToString();
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += ", Mul ";
    pos = m_Status.size();
    sprintf(buff, "%.2f", m_Multiplier);
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += ", Skip ";
    pos = m_Status.size();
    if ( m_Skip )
        m_Status += "ON";
    else
        m_Status +=  "Off";
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += ", Mute ";
    pos = m_Status.size();
    if ( m_Mute )
        m_Status += "ON";
    else
        m_Status +=  "Off";
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));


    m_Status += ", Rep ";
    pos = m_Status.size();
    if ( m_Repeater.Repeats() > 0 )
    {
        sprintf(buff, "%i", m_Repeater.Repeats());
        m_Status += buff;
    }
    else
        m_Status +=  "Off";
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += ", Rep Time ";
    pos = m_Status.size();
    if ( lround(100 * m_Repeater.RepeatTime()) > 0 )
    {
        sprintf(buff, "%.2f", m_Repeater.RepeatTime());
        m_Status += buff;
    }
    else
        m_Status +=  "Off";
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Highlights.push_back(m_FieldPositions.at(m_TrigListItemFocus));

}

bool TrigListItem::HandleKey(key_type_t k)
{
    bool newTrigs = false;
    double inc = 0.1;

    switch ( k )
    {
    case enter:
    case back_space:
    case escape:
        ReturnFocus();
        break;

    case left:
        if ( m_TrigListItemFocus > 0 )
            m_TrigListItemFocus = static_cast<trig_list_item_focus_t>(m_TrigListItemFocus - 1);
        break;

    case right:
        if ( m_TrigListItemFocus < number_tlif_types - 1 )
            m_TrigListItemFocus = static_cast<trig_list_item_focus_t>(m_TrigListItemFocus + 1);
        break;

    case shift_up:
        inc = 0.01;
    case up:
        switch ( m_TrigListItemFocus )
        {
        case tlif_trigs:
            m_TrigMask += 1;
            newTrigs = true;
            break;
        case tlif_multiplier:
            m_Multiplier += inc;
            break;
        case tlif_skip:
            m_Skip = false;
            break;
        case tlif_mute:
            m_Mute = false;
            break;
        case tlif_repeats:
            m_Repeater.SetRepeats(m_Repeater.Repeats() + 1);
            break;
        case tlif_repeat_time:
            m_Repeater.SetRepeatTime(m_Repeater.RepeatTime() + inc);
            break;
        default:
            break;
        }
        break;

    case shift_down:
        inc = 0.01;
    case down:
        switch ( m_TrigListItemFocus )
        {
        case tlif_trigs:
            if ( (int) m_TrigMask >= 0 ) // m_TrigMask is unsigned, so can't be -ve. Make it an int and allow -1 equals 'all'.
            {
                m_TrigMask -= 1;
                newTrigs = true;
            }
            break;
        case tlif_multiplier:
            if ( lround( 100 * (m_Multiplier - inc)) > 0 )
                m_Multiplier -= inc;
            break;
        case tlif_skip:
            m_Skip = true;
            break;
        case tlif_mute:
            m_Mute = true;
            break;
        case tlif_repeats:
            if ( m_Repeater.Repeats() > 0 )
                m_Repeater.SetRepeats(m_Repeater.Repeats() - 1);
            break;
        case tlif_repeat_time:
            m_Repeater.SetRepeatTime(m_Repeater.RepeatTime() - inc);
            if ( m_Repeater.RepeatTime() < 0 )
                m_Repeater.SetRepeatTime(0);
            break;
//        case tlif_repeats:
//            if ( m_Repeats > 0 )
//                m_Repeats -= 1;
//            break;
//        case tlif_repeat_time:
//            m_RepeatTime -= inc;
//            if ( m_RepeatTime < 0 )
//                m_RepeatTime = 0.0;
//            break;
        default:
            break;
        }
        break;

    default:
        return false;
    }

    if ( newTrigs )
    {
        TrigsFromMask(m_TrigMask);
    }

    m_FirstField = m_TrigListItemFocus == 0;

    SetStatus();

    return true;
}
