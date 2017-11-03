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

#include "maChainLink.h"
#include "maUtility.h"

using namespace std;

ChainLink::ChainLink()
{
    //ctor
}

ChainLink::~ChainLink()
{
    //dtor
}

int ChainLink::Remaining()
{
    // This bit makes us loop indefinitely.

    if ( m_Repeats < 0 )
        return 1;

    // Initialize remaining loop count.

    if ( m_Remaining == -1 )
        m_Remaining = m_Repeats;

    return m_Remaining--;
}

string ChainLink::ToStringForDisplay(int width)
{
    string result;

    char buff[50];

    sprintf(buff, "%02i", m_Pattern + 1);
    result += buff;

    if ( m_Remaining >= 0 )
    {
        sprintf(buff, " x%02i", m_Remaining + 1);
        result += buff;
    }
    else if ( m_Repeats > 0 )
    {
        sprintf(buff, " x%02i", m_Repeats + 1);
        result += buff;
    }

    if ( m_Jump >= 0 )
    {
        sprintf(buff, " >%02i", m_Jump + 1);
        result += buff;
    }

    bool odd = true;
    while ( result.size() < width )
    {
        if ( odd )
            result.insert(0, 1, ' ');
        else
            result += ' ';
        odd = !odd;
    }

    return result;
}


string ChainLink::ToString()
{
    char buff[50];

    sprintf(buff, "%i/%i/%i", m_Pattern, m_Repeats, m_Jump);

    return buff;
}

void ChainLink::FromString(string & s)
{
    vector<string> tokens = split(s.c_str(), '/');

    if ( tokens.size() != 3 )
        throw string("Pattern Chain parse error: nothing entered.");

    m_Pattern = stoi(tokens.at(0));
    m_Repeats = stoi(tokens.at(1));
    m_Jump = stoi(tokens.at(2));
}
