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
#include <unordered_map>

#include "maPatternChain.h"
#include "maUtility.h"

using namespace std;


PatternChain::PatternChain()
{
    //ctor
    m_Help = "S-Left/Right: new item before/after cursor, S-Del: delete current item";
}

PatternChain::~PatternChain()
{
    //dtor
}

string PatternChain::ToStringForDisplay(unsigned firstRow, unsigned rows)
{
    if ( m_Chain.empty() )
        return "Pattern Chain is empty.";

    string result;

    for ( unsigned row = firstRow; row < firstRow + rows; row++ )
    {
        if ( 4 * row >= m_Chain.size() )
            break;

        if ( ! result.empty() )
            result +=  '\n';

        char buff[20];
        sprintf(buff, "%02i - ", 4 * row + 1);
        result += buff;

        for ( unsigned i = 0; i < 4; i++ )
        {
            unsigned pos = 4 * row + i;
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

    std::vector<ChainLink> newChain;

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

            newChain.emplace_back();
            newChain.back().SetPattern(pattern);
            newChain.back().SetRepeats(repeats - 1);
        }
        catch ( invalid_argument )
        {
            // Do nothing and carry on with next token.
        }

    }

    if ( !newChain.empty() )
    {
        m_Chain = newChain;
        m_PosEdit = 0;
    }
}

void PatternChain::FromString(string s)
{
    // Scan past first token, which should be 'chain', though we don't check.

    vector<string> tokens = split(s.c_str(), ' ');

    if ( tokens.size() == 1 )
        throw string("Pattern Chain parse error: nothing entered.");

    m_Chain.clear();
    m_PosEdit = 0;

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

void PatternChain::New()
{
    if ( m_PosEdit >= m_Chain.size() )
    {
        m_Chain.emplace_back();
        m_PosEdit = m_Chain.size() - 1;
    }
    else
    {
        m_Chain.insert(m_Chain.begin() + m_PosEdit, ChainLink() );
    }

    m_MenuFocus = static_cast<pattern_chain_menu_focus_t>(m_PosEdit + num_pc_menu_items);

    for ( auto it = m_Chain.begin(); it != m_Chain.end(); it++ )
        if ( it->Jump() >= static_cast<int>(m_PosEdit) )
            it->SetJump(it->Jump() + 1 );

    SetStatus();
}

void PatternChain::Delete()
{
    m_Chain.erase(m_Chain.begin() + m_PosEdit);

    for ( auto it = m_Chain.begin(); it != m_Chain.end(); it++ )
        if ( it->Jump() == static_cast<int>(m_PosEdit) )
            it->SetJump(-1);
        else if ( it->Jump() > static_cast<int>(m_PosEdit) )
            it->SetJump(it->Jump() - 1 );

    if ( m_PosEdit >= m_Chain.size() )
    {
        m_PosEdit = m_Chain.size() - 1;
        // Menu focus will also be at max, so reset it.
        m_MenuFocus = static_cast<pattern_chain_menu_focus_t>(m_MenuFocus - 1);
    }
    if ( m_PosPlay >= m_Chain.size() )
        m_PosPlay = m_Chain.size() - 1;

    SetStatus();
}


unordered_map<PatternChain::pattern_chain_mode_t, const char *> pc_mode_names =
{
    {PatternChain::off, "off"},
    {PatternChain::natural, "natural"},
    {PatternChain::quantum, "quantum"},
};

PatternChain::pattern_chain_mode_t pc_pattern_chain_mode_lookup(string s)
{
    static unordered_map<string, PatternChain::pattern_chain_mode_t> map;

    // Initialize on first use.

    if ( map.size() == 0 )
        for ( PatternChain::pattern_chain_mode_t m = static_cast<PatternChain::pattern_chain_mode_t>(0);
              m < PatternChain::num_pattern_chain_modes;
              m = static_cast<PatternChain::pattern_chain_mode_t>(static_cast<int>(m) + 1) )
        {
            map.insert(std::make_pair(pc_mode_names.at(m), m));
        }

    return map.at(s);
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

void PatternChain::NextPatternChainMode( int dir )
{
    int m = static_cast<int>(m_PatternChainMode) + dir;

    if ( m >= 0 && m < static_cast<int>(num_pattern_chain_modes) )
        m_PatternChainMode = static_cast<pattern_chain_mode_t>(m);
}

void PatternChain::SetStatus()
{
    int pos = 0;
    char buff[20];

    m_Status = "[Chain]";
    m_FieldPositions.clear();
    m_Highlights.clear();

    if ( m_Chain.empty() )
    {
        m_Status += " Empty";
        return;
    }

    m_Status += " Mode: ";
    pos = m_Status.size();
    m_Status += pc_mode_names.at(m_PatternChainMode);
    m_FieldPositions.emplace_back(pos, m_Status.size() - pos);


    for ( unsigned i = 0; i < m_Chain.size(); i++ )
    {
        m_Status += " ";
        pos = m_Status.size();
        sprintf(buff, "%i:", i + 1);
        m_Status += buff;
        m_Status += m_Chain.at(i).ToStringForDisplay(true, 1);
        m_FieldPositions.emplace_back(pos, m_Status.size() - pos);
    }

    if ( ! m_Chain.empty() )
        m_Highlights.push_back(m_FieldPositions.at(m_MenuFocus));
}

bool PatternChain::HandleKey(key_type_t k)
{
    switch ( k )
    {
    case enter:
        if ( m_MenuFocus >= num_pc_menu_items && m_PosEdit < m_Chain.size() )
        {
            ChainLink & link = m_Chain.at(m_PosEdit);
            link.SetItemID(m_PosEdit + 1);
            link.SetFocus();
            link.SetStatus();
            link.SetParent(this);       // Specific pointer to PatternChain.
            link.SetReturnFocus(this);  // Generic return pointer to CursorKeys object.
        }
        break;
    case left:
        if ( m_MenuFocus > 0 )
        {
            m_MenuFocus = static_cast<pattern_chain_menu_focus_t>(m_MenuFocus - 1);
            m_PosEdit = max(m_MenuFocus - num_pc_menu_items, 0);
        }
//        if ( m_PosEdit > 0 )
//            m_PosEdit -= 1;
        break;
    case right:
        if ( m_MenuFocus < num_pc_menu_items + m_Chain.size() - 1 )
        {
            m_MenuFocus = static_cast<pattern_chain_menu_focus_t>(m_MenuFocus + 1);
            m_PosEdit = max(m_MenuFocus - num_pc_menu_items, 0);
        }
//        if ( m_PosEdit < m_Chain.size() - 1 )
//            m_PosEdit += 1;
        break;
    case up:
        switch (m_MenuFocus)
        {
        case mode:
            NextPatternChainMode(-1);
            break;
        default:
            break;
        }
        break;

    case down:
        switch (m_MenuFocus)
        {
        case mode:
            NextPatternChainMode(1);
            break;
        default:
            break;
        }
        break;


    case shift_right:
    case shift_left:
        switch (m_MenuFocus)
        {
        case mode:
            if ( !m_Chain.empty() )
                break;
        default:
            if ( k == shift_right)
                m_PosEdit += 1;
            New();
            break;
        }
        break;

    case shift_delete:
        switch (m_MenuFocus)
        {
        case mode:
            break;
        default:
            Delete();
            break;
        }
        break;

    default:
        return false;
    }

    m_FirstField = m_MenuFocus == 0;

    SetStatus();

    return true;
}


