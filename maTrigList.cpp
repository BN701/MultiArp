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

#if defined(MA_BLUE)
#include <cstdio>
#endif

#include "maTrigList.h"
#include "maUtility.h"

using namespace std;

TrigList::TrigList()
{
    //ctor
   m_Help = "S-L/R: add trig stage, C-L/R: copy trig stage, S-Del: delete ";
}

TrigList::~TrigList()
{
    //dtor
}

void TrigList::SetStatus()
{
    int pos = 0;
//    char buff[50];

    m_FieldPositions.clear();
    m_Highlights.clear();

    m_Status = "[Trig List] ";

//    for ( int i = 0; i < m_TrigItems.size(); i++ )
    for ( auto it = m_TrigItems.begin(); it < m_TrigItems.end(); it++ )
    {
        if ( it > m_TrigItems.begin() )
            m_Status += ' ';
        pos = m_Status.size();
//        m_Status += m_TrigItems.at(i).MenuString();
        m_Status += it->MenuString();
        m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));
    }

    if ( !m_FieldPositions.empty() )
        m_Highlights.push_back(m_FieldPositions.at(m_PosEdit));

}

bool TrigList::HandleKey(BaseUI::key_command_t k)
{
    switch ( k )
    {
    case BaseUI::key_return:
        if ( !m_TrigItems.empty() )
        {
            TrigListItem & n = m_TrigItems.at(m_PosEdit);
            n.SetItemID(m_PosEdit + 1);
            n.SetFocus();
            n.SetStatus();
            n.SetReturnFocus(this);
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
        if ( m_PosEdit < m_TrigItems.size() - 1 )
            m_PosEdit += 1;
        break;

    case BaseUI::key_ctrl_left:
        if ( !m_TrigItems.empty() )
        {
            m_TrigItems.insert(m_TrigItems.begin() + m_PosEdit, m_TrigItems.at(m_PosEdit));
        }
        break;

    case BaseUI::key_ctrl_right:
        if ( !m_TrigItems.empty() )
        {
            m_TrigItems.insert(m_TrigItems.begin() + m_PosEdit + 1, m_TrigItems.at(m_PosEdit));
            m_PosEdit += 1;
        }
        break;

    case BaseUI::key_shift_left:
        if ( m_TrigItems.empty() )
        {
            m_TrigItems.emplace_back();
            m_PosEdit = 0;
        }
        else
            m_TrigItems.insert(m_TrigItems.begin() + m_PosEdit, *(new TrigListItem()));
        break;

    case BaseUI::key_shift_delete:
        if ( !m_TrigItems.empty() )
        {
            m_TrigItems.erase(m_TrigItems.begin() + m_PosEdit);
            if ( m_PosEdit == m_TrigItems.size() )
                m_PosEdit -= 1;
        }
        break;

    case BaseUI::key_shift_right:
        if ( m_TrigItems.empty() )
        {
            m_TrigItems.emplace_back();
            m_PosEdit = 0;
        }
        else
        {
            m_TrigItems.insert(m_TrigItems.begin() + m_PosEdit + 1, *(new TrigListItem()));
            m_PosEdit += 1;
        }
        break;

    default:
        return false;
    }

    m_FirstField = m_PosEdit == 0;

//    SetStatus();

    return true;
}

string TrigList::ToString(const char * prefix)
{
    string result;

    if ( prefix != NULL )
    {
        result += prefix;
        result += ' ';
    }

    result += "Trigs ";

    for (auto it = m_TrigItems.begin(); it != m_TrigItems.end(); )
    {
        result += " \\\n    ";
        result += it->ToString();
        if ( ++it < m_TrigItems.end() )
            result += ", ";

    }

    return result;
}

void TrigList::FromString(string s)
{
//    if ( s.find("Trigs") == string::npos )
//        throw string("TrigList::FromString() - Not a valid trig list.");

    vector<string> tokens = split(s.c_str(), ',');

    if ( tokens.size() == 0 )
        return;

    m_TrigItems.clear();

    for ( auto it = tokens.begin(); it != tokens.end(); it++ )
    {
        m_TrigItems.emplace_back();
        m_TrigItems.back().FromString(*it);
    }
}

void TrigList::AddArpeggio(vector<string>::iterator token, vector<string>::iterator finish)
{
//    vector<string> tokens = split(s.c_str(), ' ');

    // TODO: Currently just a aplaceholder to set up some test data.

//    if ( m_TrigItems.empty() )
        return;

    m_TrigItems.front().Repeater().AddArpStage(2, 16);
//    m_TrigItems.front().Repeater().AddArpStage(1, 2);
//    m_TrigItems.front().Repeater().AddArpStage(-2, 4);
}


void TrigList::FromSimpleString(vector<string>::iterator token, vector<string>::iterator finish)
{
//    vector<string> tokens = split(s.c_str(), ' ');

//    if ( tokens.size() <= 1 )
//        throw string("TrigList::FromSimpleString() - Nothing found.");

    m_TrigItems.clear();

    for ( ; token != finish; token++ )
    {
        m_TrigItems.emplace_back();
        m_TrigItems.back().FromSimpleString(*token);
    }
}


string TrigList::ToStringForDisplay()
{
    string result;

    char buff[50];

    for ( unsigned i = 0; i < m_TrigItems.size(); i++ )
    {
//        if ( i > 0 )
//            result += ' ';
        if ( i == m_Pos )
        {
#if defined(MA_BLUE) && !defined(MA_BLUE_PC)
            snprintf(buff, 50, "%3u| ", m_Pos + 1);
#else
            snprintf(buff, 50, "%3lu| ", m_Pos + 1);
#endif
            result += buff;
        }
        result += m_TrigItems.at(i).MenuString(9);
    }

    return result;
}

string TrigList::ToStringForDisplay2(int & offset, int & length, unsigned displayWidth)
{
    string result;

    unsigned itemWidth = 9;
    unsigned itemsPerRow = displayWidth/itemWidth;

//    char buff[50];

    offset = 0;
    length = 0;

    for ( unsigned i = 0; i < m_TrigItems.size(); i++ )
    {
        if ( i == m_Pos )
            offset = result.size();

        result += m_TrigItems.at(i).MenuString(9);

        if ( i == m_Pos )
            length = result.size() - offset;
    }

    // Scroll left if highlight is beyond displayWidth.

    while ( static_cast<unsigned>(offset + length) > displayWidth )
    {
        int scroll = itemsPerRow * itemWidth;
        result.erase(0, scroll);
        offset -= scroll;
    }

    // Truncate if line itself goes beyond width.

    if ( result.size() > displayWidth )
    {
        result = result.substr(0, displayWidth);
    }

    return result;
}

TrigListItem * TrigList::Step()
{
    if ( ! m_TrigItems.empty() )
    {
        m_Pos += 1;
        if ( m_Pos >= m_TrigItems.size() )
            m_Pos = 0;
        return & m_TrigItems.at(m_Pos);
    }
    else
        return NULL;
}
