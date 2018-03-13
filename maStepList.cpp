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

#include "maListGroup.h"
#include "maRealTimeList.h"
#include "maStepList.h"


#include <algorithm>
#include <cmath>
#include <unordered_map>

#if defined(MA_BLUE)
#include <cstdio>
#endif

#include "maUtility.h"

using namespace std;


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
#if defined(MA_BLUE)
            const char * format = "%3u| ";
#else
            const char * format = "%3lu| ";
#endif
            snprintf(buff, 50, format, m_LastRequestedPos + 1);
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

bool StepList::StepListFromString(string s)
{
    vector<string> chordStrings = split(s.c_str(), ',', true);

    if ( chordStrings.size() == 0 )
#ifdef MA_BLUE
        return false;
#else
        throw string("Note List parse error: nothing found.");
#endif

    Clear();

    for ( vector<string>::iterator it = chordStrings.begin(); it != chordStrings.end(); it++ )
    {
        Cluster chord;
        if ( chord.ClusterFromString(*it) )
            m_Clusters.push_back(chord);
    }

    return !m_Clusters.empty();
}

void StepList::SetStatus()
{
    int pos = 0;
    char buff[200];

    m_FieldPositions.clear();
    m_Highlights.clear();

    snprintf(buff, 200, "[Step List %i] ", m_ItemID);
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

bool StepList::HandleKey(BaseUI::key_command_t k)
{
    switch ( k )
    {
    case BaseUI::key_return:
        if ( ! m_Clusters.empty() )
        {
            Cluster & c = m_Clusters.at(m_PosEdit);
            c.SetItemID(m_PosEdit + 1);
            c.SetFocus();
            c.SetStatus();
            c.SetReturnFocus(this);
        }
        break;

    case BaseUI::key_backspace:
    case BaseUI::key_escape:
        ReturnFocus();
        break;

    case BaseUI::key_left:
        if ( m_PosEdit > 0 )
            m_PosEdit -= 1;
        break;

    case BaseUI::key_right:
        if ( m_PosEdit < m_Clusters.size() - 1 )
            m_PosEdit += 1;
        break;

    case BaseUI::key_up:
    case BaseUI::key_down:
        if ( m_ReturnFocus != NULL )
        {
            m_ReturnFocus->HandleKey(k);
            m_ReturnFocus->HandleKey(BaseUI::key_return);
        }
        return true;

    case BaseUI::key_ctrl_left:
        if ( !m_Clusters.empty() )
        {
            m_Clusters.insert(m_Clusters.begin() + m_PosEdit, m_Clusters.at(m_PosEdit));
        }
        break;

    case BaseUI::key_ctrl_right:
        if ( !m_Clusters.empty() )
        {
            m_Clusters.insert(m_Clusters.begin() + m_PosEdit + 1, m_Clusters.at(m_PosEdit));
            m_PosEdit += 1;
        }
        break;

    case BaseUI::key_shift_left:
        if ( m_Clusters.empty() )
        {
            m_Clusters.emplace_back();
            m_PosEdit = 0;
        }
        else
            m_Clusters.insert(m_Clusters.begin() + m_PosEdit, *(new Cluster()));
        break;

    case BaseUI::key_shift_right:
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

    case BaseUI::key_shift_delete:
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


