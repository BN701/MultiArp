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


#include "maTrigList.h"

using namespace std;

TrigList::TrigList()
{
    //ctor
}

TrigList::~TrigList()
{
    //dtor
}

void TrigList::SetStatus()
{
    int pos = 0;
    char buff[50];

    m_FieldPositions.clear();
    m_Highlights.clear();

    m_Status = "[Trig List] ";

    for ( int i = 0; i < m_TrigItems.size(); i++ )
    {
        if ( i > 0 )
            m_Status += ' ';
        pos = m_Status.size();
        m_Status += m_TrigItems.at(i).MenuString();
        m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));
    }

    if ( !m_FieldPositions.empty() )
        m_Highlights.push_back(m_FieldPositions.at(m_PosEdit));

}

bool TrigList::HandleKey(key_type_t k)
{
    switch ( k )
    {
    case enter:
        if ( !m_TrigItems.empty() )
        {
            TrigListItem & n = m_TrigItems.at(m_PosEdit);
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
        if ( m_PosEdit < m_TrigItems.size() - 1 )
            m_PosEdit += 1;
        break;

//    case up:
//        if ( m_ReturnFocus != NULL )
//        {
//            m_ReturnFocus->HandleKey(right);
//            m_ReturnFocus->HandleKey(enter);
//        }
//        return true;
//
//    case down:
//        if ( m_ReturnFocus != NULL )
//        {
//            m_ReturnFocus->HandleKey(left);
//            m_ReturnFocus->HandleKey(enter);
//        }
//        return true;

    case ctrl_left:
        if ( !m_TrigItems.empty() )
        {
            m_TrigItems.insert(m_TrigItems.begin() + m_PosEdit, m_TrigItems.at(m_PosEdit));
        }
        break;

    case ctrl_right:
        if ( !m_TrigItems.empty() )
        {
            m_TrigItems.insert(m_TrigItems.begin() + m_PosEdit + 1, m_TrigItems.at(m_PosEdit));
            m_PosEdit += 1;
        }
        break;

    case shift_left:
        if ( m_TrigItems.empty() )
        {
            m_TrigItems.emplace_back();
            m_PosEdit = 0;
        }
        else
            m_TrigItems.insert(m_TrigItems.begin() + m_PosEdit, *(new TrigListItem()));
        break;

    case shift_delete:
        if ( !m_TrigItems.empty() )
        {
            m_TrigItems.erase(m_TrigItems.begin() + m_PosEdit);
            if ( m_PosEdit == m_TrigItems.size() )
                m_PosEdit -= 1;
        }
        break;

    case shift_right:
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

    SetStatus();

    return true;
}

string TrigList::ToStringForDisplay()
{
    string result;

    char buff[50];

    for ( int i = 0; i < m_TrigItems.size(); i++ )
    {
//        if ( i > 0 )
//            result += ' ';
        if ( i == m_Pos )
        {
            sprintf(buff, "%3i| ", m_Pos + 1);
            result += buff;
        }
        result += m_TrigItems.at(i).MenuString(9);
    }

    return result;
}

string TrigList::ToStringForDisplay2(int & offset, int & length, int width)
{
    string result;

    char buff[50];

    offset = 0;
    length = 0;

    for ( int i = 0; i < m_TrigItems.size(); i++ )
    {
//        if ( i > 0 )
//            result += ' ';
        if ( i == m_Pos )
        {
            offset = result.size();
        }
        result += m_TrigItems.at(i).MenuString(9);
        if ( i == m_Pos )
        {
            length = result.size() - offset;
        }
    }

    // Scroll left if highlight is beyond width.

    while ( offset + length > width )
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
