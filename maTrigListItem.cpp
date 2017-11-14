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

#include <climits>
#include <cmath>

#include "maTrigListItem.h"

using namespace std;

TrigListItem::TrigListItem()
{
    //ctor
}

TrigListItem::~TrigListItem()
{
    //dtor
}

string TrigListItem::ToString()
{
    string result = "TrigListItem";

    return result;
}

string TrigListItem::TrigMaskToString()
{
    if ( m_TrigMask != 0 )
    {
        string result;
        char buff[20];

        bool addSpacer = false;
        unsigned int mask = 1;

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
    else
        return "None";

}

string TrigListItem::MenuString()
{
    char buff[200];
    string result = TrigMaskToString();

    if ( lround(m_Multiplier * 100) != 100 )
    {
        sprintf(buff, "x%.2f", m_Multiplier);
        result += buff;
    }

    if ( m_Repeats > 0 )
    {
        sprintf(buff, "/R%i", m_Repeats);
        result += buff;
    }

    if ( m_Skip )
        result += ",Skip ";

    if ( m_Mute )
        result += ",Mute ";

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
//    if ( !m_Trigs.empty() )
//    {
//        for ( int i = 0; i < m_Trigs.size(); i++)
//        {
//            if ( i > 0 )
//                m_Status += '/';
//            sprintf(buff, "%i", m_Trigs.at(i));
//            m_Status += buff;
//        }
//    }
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
    if ( m_Repeats > 0 )
    {
        sprintf(buff, "%i", m_Repeats);
        m_Status += buff;
    }
    else
        m_Status +=  "Off";
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += ", Rep Time ";
    pos = m_Status.size();
    if ( lround(100 * m_RepeatTime) > 0 )
    {
        sprintf(buff, "%.2f", m_RepeatTime);
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
            m_Repeats += 1;
            break;
        case tlif_repeat_time:
            m_RepeatTime += inc;
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
            if ( m_TrigMask > 0 )
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
            if ( m_Repeats > 0 )
                m_Repeats -= 1;
            break;
        case tlif_repeat_time:
//            if ( lround( 100 * (m_RepeatTime - inc) ) >= 0 )
            m_RepeatTime -= inc;
            if ( m_RepeatTime < 0 )
                m_RepeatTime = 0.0;
            break;
        default:
            break;
        }
        break;

    default:
        return false;
    }

    if ( newTrigs )
    {
        unsigned int mask = 1;
        m_Trigs.clear();

        for ( int i = 0; i < CHAR_BIT * sizeof(mask); i++ )
        {
            if ( (m_TrigMask & mask) > 0 )
                m_Trigs.push_back(i);
            mask = mask << 1;
        }
    }

    m_FirstField = m_TrigListItemFocus == 0;

    SetStatus();

    return true;
}
