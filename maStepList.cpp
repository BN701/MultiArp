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

//string StepList::ToStringForDisplay(int & offset, int & length)
//{
//    string result;
//
//    char buff[50];
//
//    offset = 0;
//    length = 0;
//
//    for ( unsigned i = 0; i < m_Clusters.size(); i++ )
//    {
//        if ( i > 0 )
//            result += ' ';
//        if ( i == m_NowPlayingPos )
//        {
//#if defined(MA_BLUE)
//            const char * format = "%3u| ";
//#else
//            const char * format = "%3lu| ";
//#endif
//            snprintf(buff, 50, format, m_NowPlayingPos + 1);
//            result += buff;
//        }
//        if ( i == 0 )
//            offset = result.size();
//
//        result += m_Clusters.at(i).ToString(false);
//
//        if ( i == 0 )
//            length = result.size() - offset;
//    }
//
//    return result;
//}
//
//string StepList::ToStringForDisplay2(int & offset, int & length, unsigned width)
//{
//    string result;
//
//    offset = 0;
//    length = 0;
//
//    for ( unsigned i = 0; i < m_Clusters.size(); i++ )
//    {
//        if ( i > 0 )
//            result += ' ';
//        if ( i == m_NowPlayingPos )
//        {
//            offset = result.size();
//        }
//        result += m_Clusters.at(i).ToString(false);
//        if ( i == m_NowPlayingPos )
//        {
//            length = result.size() - offset;
//        }
//    }
//
//    // Scroll left if highlight is beyond width.
//
//    while ( static_cast<unsigned>(offset + length) > width )
//    {
//        int scroll = 3 * width / 4;
//        result.erase(0, scroll + 3);
//        result.insert(0, "...");
//        offset -= scroll;
//    }
//
//    // Truncate if line itself goes beyond width.
//
//    if ( result.size() > width )
//    {
//        result = result.substr(0, width - 4);
//        result += "... ";
//    }
//
//    return result;
//}

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

#if 0
    if ( m_GotFocus )
        snprintf(buff, 200, "[List %lu] ", m_NowPlayingPos + 1);
    else
        snprintf(buff, 200, " List %lu  ", m_NowPlayingPos + 1);
#else
    if ( m_GotFocus )
        snprintf(buff, 200, "[List %i] ", m_ItemID);
    else
        snprintf(buff, 200, " List %i  ", m_ItemID);
#endif
    InitStatus();
    m_Status += buff;

    for ( unsigned i = 0; i < m_Clusters.size(); i++ )
    {
        if ( i > 0 )
            m_Status += ",";
        if ( i == m_NowPlayingPos )
            m_Status += '>';
        else
            m_Status += ' ';
        pos = m_Status.size();
        Cluster & c = m_Clusters.at(i);
        if ( !c.Empty() )
            m_Status += c.ToString(false);
        else
            m_Status += "~";
        m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));
    }

    if ( m_GotFocus && !m_FieldPositions.empty() )
        m_Highlights.push_back(m_FieldPositions.at(m_PosEdit));
}

bool StepList::HandleKey(BaseUI::key_command_t k)
{
    switch ( k )
    {
    case BaseUI::key_cmd_enter:
        if ( ! m_Clusters.empty() )
        {
            Cluster & c = m_Clusters.at(m_PosEdit);
            c.SetItemID(m_PosEdit + 1);
//            c.SetFocus();
            c.SetDisplayIndent(m_MenuListIndent + 2);
            c.SetVisible(m_Visible);
            m_MenuListPtr->InsertAfter(m_PosInMenuList, & c, true);  // This selects it, too.
            c.SetReturnFocus(this);                         // Override return focus.
//            MenuInsert(m_PosInMenuList, & c);
        }
        break;

    case BaseUI::key_cmd_back:
        ReturnFocus();
        break;

    case BaseUI::key_cmd_left:
        if ( m_PosEdit > 0 )
            m_PosEdit -= 1;
        break;

    case BaseUI::key_cmd_right:
        if ( m_PosEdit < m_Clusters.size() - 1 )
            m_PosEdit += 1;
        break;

    case BaseUI::key_cmd_up:
        if ( m_MenuListPtr != NULL )
            m_MenuListPtr->DownCursorPos();
        return true;

    case BaseUI::key_cmd_down:
        if ( m_MenuListPtr != NULL )
            m_MenuListPtr->UpCursorPos();
        return true;

    case BaseUI::key_cmd_inc:
    case BaseUI::key_cmd_dec:
    case BaseUI::key_cmd_inc_2:
    case BaseUI::key_cmd_dec_2:
        if ( !m_Clusters.empty() )
        {
            m_Clusters.at(m_PosEdit).HandleKey(k);
        }
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

    case BaseUI::key_cmd_insert_right:
        InsertRight();
        break;

    case BaseUI::key_cmd_delete:
        DeleteStep();
        break;

    default:
        return false;
    }

    m_FirstField = m_PosEdit == 0;

    SetRedraw();

    return true;
}

void StepList::InsertLeft()
{
    if ( m_Clusters.empty() )
    {
        m_Clusters.emplace_back();
        m_PosEdit = 0;
    }
    else
        m_Clusters.insert(m_Clusters.begin() + m_PosEdit, *(new Cluster()));
}

void StepList::InsertRight()
{
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
}

void StepList::CopyLeft()
{
    if ( !m_Clusters.empty() )
    {
        m_Clusters.insert(m_Clusters.begin() + m_PosEdit, m_Clusters.at(m_PosEdit));
    }
}

void StepList::CopyRight()
{
    if ( !m_Clusters.empty() )
    {
        m_Clusters.insert(m_Clusters.begin() + m_PosEdit + 1, m_Clusters.at(m_PosEdit));
        m_PosEdit += 1;
    }
}

void StepList::DeleteStep()
{
    if ( !m_Clusters.empty() )
    {
        m_Clusters.erase(m_Clusters.begin() + m_PosEdit);
        if ( m_PosEdit == m_Clusters.size() )
            m_PosEdit -= 1;
    }
}

Cluster * StepList::Step()
{
    if ( m_Clusters.empty() )
        return NULL;

//    m_LastRequestedPos = m_Pos;
    Cluster *pCluster = & m_Clusters[m_PosSL++];

    // Look ahead for rests.

    if ( !pCluster->IsRest() )
    {
        vector<int>::size_type p = m_PosSL;
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

    if ( m_PosSL >= m_Clusters.size() )
    {
        m_Complete = true;
        m_PosSL = 0;
    }
    else
    {
        m_Complete = false;
    }

//    SetRedraw();
    return pCluster;
}

