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
#include <cerrno>
#include <cstdlib>
#include <cstdio>
#endif

#include <unordered_map>

#include "maPatternChain.h"
#include "maPatternStore.h"
#include "maUtility.h"

using namespace std;


PatternChain::PatternChain(PatternStore * store):
    m_PatternStore(store)
{
    //ctor
    m_PopUpMenuID = C_MENU_ID_PATTERN_CHAIN;
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
        snprintf(buff, 20, "%02i - ", 4 * row + 1);
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

//void PatternChain::FromSimpleString(string s)
//{
//    // Scan past first token, which should be 'chain', though we don't check.
//
//    vector<string> tokens = split(s.c_str(), ' ');
//
//    if ( tokens.size() == 1 )
//#ifdef MA_BLUE
//        return;
//#else
//        throw string("Pattern Chain parse error: nothing entered.");
//#endif
//
//    std::vector<ChainLink> newChain;
//
//    for ( vector<string>::iterator it = tokens.begin() + 1; it < tokens.end(); it++ )
//    {
//#ifndef MA_BLUE
//        try
//        {
//#endif
//            size_t pos;
//#if defined(MA_BLUE)
//            const char *startPtr = it->c_str();
//            char *endPtr;
//            int pattern = strtol(startPtr, &endPtr, 0) - 1;
//            if ( errno != 0 )
//                continue;
//            pos = endPtr - startPtr;
//#else
//            int pattern = stoi(*it, &pos) - 1;
//#endif
//            if ( pattern < 0 )
//                continue;
//
//            int repeats = 1;
//            if ( pos < (*it).size() )
//            {
//                pos = (*it).find('x');
//                if ( pos != string::npos )
//                    repeats = strtol((*it).substr(pos + 1).c_str(), NULL, 0);
//            }
//
//            newChain.emplace_back();
//            newChain.back().SetPattern(pattern);
//            newChain.back().SetRepeats(repeats - 1);
//#ifndef MA_BLUE
//        }
//        catch ( invalid_argument )
//        {
//            // Do nothing and carry on with next token.
//        }
//#endif
//    }
//
//    if ( !newChain.empty() )
//    {
//        m_Chain = newChain;
//        m_PosEdit = 0;
//    }
//}

void PatternChain::FromString(string s)
{
    // Scan past first token, which should be 'chain', though we don't check.

    vector<string> tokens = split(s.c_str(), ' ');

    if ( tokens.size() == 1 )
#ifdef MA_BLUE
        return;
#else
        throw string("Pattern Chain parse error: nothing entered.");
#endif

    m_Chain.clear();
    m_PosEdit = 0;

    for ( vector<string>::iterator it = tokens.begin() + 1; it < tokens.end(); it++ )
    {
#ifndef MA_BLUE
        try
        {
#endif
            m_Chain.emplace_back();
            m_Chain.back().FromString(*it);
#ifndef MA_BLUE
        }
        catch ( invalid_argument )
        {
            // Do nothing and carry on with next token.
        }
#endif
    }
}

void PatternChain::New()
{
    if ( m_PosEdit >= m_Chain.size() )
    {
        m_Chain.emplace_back(m_PatternStore);
        m_PosEdit = m_Chain.size() - 1;
    }
    else
    {
        m_Chain.insert(m_Chain.begin() + m_PosEdit, ChainLink(m_PatternStore) );
    }
    // Todo: This is going to break when the pattern chain vector reallocates.
    m_Chain[m_PosEdit].SetParent(this);

    m_MenuFocus = static_cast<pattern_chain_menu_focus_t>(m_PosEdit + num_pc_menu_items);

    for ( auto it = m_Chain.begin(); it != m_Chain.end(); it++ )
        if ( it->Jump() >= static_cast<int>(m_PosEdit) )
            it->SetJump(it->Jump() + 1 );

//    SetStatus();
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

//    SetStatus();
}

#if defined(MA_BLUE)
unordered_map<int, const char *> pc_mode_names =
#else
unordered_map<PatternChain::pattern_chain_mode_t, const char *> pc_mode_names =
#endif
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

void PatternChain::SetMode(pattern_chain_mode_t val)
{
    if ( m_PatternChainMode != val )
    {
        m_PatternChainMode = val;
        SetRedraw();
    }
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

    InitStatus();
    if ( m_GotFocus )
        m_Status += "[Chain]";
    else
        m_Status += " Chain ";
    m_FieldPositions.clear();
    m_Highlights.clear();

    if ( m_Chain.empty() )
    {
        m_Status += " Empty";
        return;
    }

    m_Status += " ";
    pos = m_Status.size();
    m_Status += pc_mode_names.at(m_PatternChainMode);
    m_FieldPositions.emplace_back(pos, m_Status.size() - pos);


    for ( unsigned i = 0; i < m_Chain.size(); i++ )
    {
        if ( m_PosPlay == i )
            m_Status += '>';
        else
            m_Status += ' ';
        pos = m_Status.size();
//        snprintf(buff, 20, "%i:", i + 1);
//        m_Status += buff;
        m_Status += m_Chain[i].ToStringForDisplay(false, 1);
        m_FieldPositions.emplace_back(pos, m_Status.size() - pos);
    }

    if ( m_GotFocus && !m_Chain.empty() )
        m_Highlights.push_back(m_FieldPositions.at(m_MenuFocus));
}

void PatternChain::SetFocus() noexcept
{
    ItemMenu::SetFocus();
    if ( !m_Chain.empty() && m_PosEdit >= 0 && m_PosEdit < m_Chain.size() )
    {
        m_PatternStore->SetEditPos(m_Chain[m_PosEdit].m_PatternHash);
    }
}

bool PatternChain::HandleKey(BaseUI::key_command_t k)
{
    switch ( k )
    {
    case BaseUI::key_cmd_enter:
        if ( m_MenuFocus < num_pc_menu_items )
        {
            // Only one option at the moment.

            // Just toggle mode to quantum (or off). If turning
            // on, also make sure everything else is turned off.

            if ( m_PatternChainMode == off )
                m_PatternStore->SetActivePatternChain(this);
            else
                m_PatternStore->SetActivePatternChain(NULL);
        }
        else if ( m_PosEdit >= 0 && m_PosEdit < m_Chain.size() )
        {
            ChainLink & l = m_Chain[m_PosEdit];
            l.SetItemID(m_PosEdit + 1);
            l.SetDisplayIndent(m_MenuListIndent + 2);
            l.SetVisible(m_Visible);
            m_MenuListPtr->InsertAfter(m_PosInMenuList, & l, true);  // This selects it, too.
            l.SetParent(this);       // Specific pointer to PatternChain.
            l.SetReturnFocus(this);  // Generic return pointer to ItemMenu object.
        }
        break;
    case BaseUI::key_cmd_left:
        if ( m_MenuFocus > 0 )
        {
            m_MenuFocus = static_cast<pattern_chain_menu_focus_t>(m_MenuFocus - 1);
            m_PosEdit = max(m_MenuFocus - num_pc_menu_items, 0);
            m_PatternStore->SetEditPos(m_Chain[m_PosEdit].m_PatternHash);
        }
        break;
    case BaseUI::key_cmd_right:
        if ( m_MenuFocus < num_pc_menu_items + m_Chain.size() - 1 )
        {
            m_MenuFocus = static_cast<pattern_chain_menu_focus_t>(m_MenuFocus + 1);
            m_PosEdit = max(m_MenuFocus - num_pc_menu_items, 0);
            m_PatternStore->SetEditPos(m_Chain[m_PosEdit].m_PatternHash);
        }
        break;

    case BaseUI::key_cmd_back:
        if ( m_MenuListPtr != NULL )
        {
            auto pos = m_MenuListPtr->ReverseFind(BaseUI::dot_pattern_store);
            m_MenuListPtr->Select(pos);
        }
        break;

    case BaseUI::key_cmd_copy_left:
    case BaseUI::key_cmd_copy_right:
    case BaseUI::key_cmd_move_left:
    case BaseUI::key_cmd_move_right:
    case BaseUI::key_cmd_move_up:
    case BaseUI::key_cmd_move_down:
    case BaseUI::key_cmd_shift_left:
    case BaseUI::key_cmd_shift_right:
    case BaseUI::key_cmd_undo:
        break;

    case BaseUI::key_cmd_inc:
    case BaseUI::key_cmd_dec:
    case BaseUI::key_cmd_inc_2:
    case BaseUI::key_cmd_dec_2:
        if ( !m_Chain.empty() )
        {
            m_Chain[m_PosEdit].HandleKey(k);
        }
        break;

    case BaseUI::key_cmd_up:
        m_MenuListPtr->DownCursorPos();
        return true;

    case BaseUI::key_cmd_down:
        m_MenuListPtr->UpCursorPos();
        return true;

    case BaseUI::key_cmd_insert_left:
    case BaseUI::key_cmd_insert_right:
        switch (m_MenuFocus)
        {
        case mode:
            if ( !m_Chain.empty() )
                break;
        default:
            if ( k == BaseUI::key_cmd_insert_right)
                m_PosEdit += 1;
            New();
            break;
        }
        break;

    case BaseUI::key_cmd_delete:
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

    SetRedraw();

    return true;
}

void PatternChain::SetJumpOverride(ChainLink * l)
{
    m_Chain[m_PosPlay].ClearRemaining();
    m_PatternStore->StopCurrentPlayPattern();
    m_JumpOverride = l - & m_Chain[0];  // This works as long as m_Chain is a vector.
}

int PatternChain::JumpOverride()
{
    int t = m_JumpOverride;
    m_JumpOverride = -1;
    return t;
}

void PatternChain::Clear()
{
    m_Chain.clear();
    m_PosEdit = 0;
//            SetStatus();
}

