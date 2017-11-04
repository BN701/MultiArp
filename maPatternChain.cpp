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

#include <stdexcept>

#include "maPatternChain.h"
#include "maUtility.h"

using namespace std;


PatternChain::PatternChain()
{
    //ctor
}

PatternChain::~PatternChain()
{
    //dtor
}

string PatternChain::ToStringForDisplay(int firstRow, int rows)
{
    if ( m_Chain.empty() )
        return "Pattern Chain is empty.";

    string result;

    for ( int row = firstRow; row < firstRow + rows; row++ )
    {
        if ( 4 * row >= m_Chain.size() )
            break;

        if ( ! result.empty() )
            result +=  '\n';

        char buff[20];
        sprintf(buff, "%02i - ", 4 * row + 1);
        result += buff;

        for ( int i = 0; i < 4; i++ )
        {
            int pos = 4 * row + i;
            if ( pos >= m_Chain.size() )
                break;
            result += m_Chain.at(pos).ToStringForDisplay();
        }
    }

    return result;
}

void PatternChain::FromSimpleString(string s)
{
    // Scan past first token, which should be 'chain', though we don't check.

    vector<string> tokens = split(s.c_str(), ' ');

    if ( tokens.size() == 1 )
        throw string("Pattern Chain parse error: nothing entered.");

    m_Chain.clear();

    for ( vector<string>::iterator it = tokens.begin() + 1; it < tokens.end(); it++ )
    {
        try
        {
            size_t pos;
            int pattern = stoi(*it, &pos) - 1;

            if ( pattern < 0 )
                continue;

            int repeats = 1;
            if ( pos < (*it).size() )
            {
                pos = (*it).find('x');
                if ( pos != string::npos )
                    repeats = stoi((*it).substr(pos + 1));
            }

            m_Chain.emplace_back();
            m_Chain.back().SetPattern(pattern);
            m_Chain.back().SetRepeats(repeats - 1);
        }
        catch ( invalid_argument )
        {
            // Do nothing and carry on with next token.
        }

    }
}

void PatternChain::FromString(string s)
{
    // Scan past first token, which should be 'chain', though we don't check.

    vector<string> tokens = split(s.c_str(), ' ');

    if ( tokens.size() == 1 )
        throw string("Pattern Chain parse error: nothing entered.");

    m_Chain.clear();
//    m_PosPatternChain = 0;

    for ( vector<string>::iterator it = tokens.begin() + 1; it < tokens.end(); it++ )
    {
        try
        {
            m_Chain.emplace_back();
            m_Chain.back().FromString(*it);
        }
        catch ( invalid_argument )
        {
            // Do nothing and carry on with next token.
        }

    }
}

string PatternChain::ToString()
{
    if ( m_Chain.empty() )
        return "Pattern Chain empty.";

    string result = "Chain";

    for ( vector<ChainLink>::iterator it = m_Chain.begin(); it != m_Chain.end(); it++ )
    {
        result += ' ';
        result += (*it).ToString();
    }

    return result;

}


