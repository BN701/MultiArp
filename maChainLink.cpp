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

#include <cstdio>
#include <cstdlib>

#include "maChainLink.h"
#include "maPatternChain.h"
#include "maUtility.h"


using namespace std;

ChainLink::ChainLink()
{
    //ctor
    m_Help = "In PC mode, enter number on command line to jump to stage.";
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

string ChainLink::ToStringForDisplay(bool forMenu, unsigned width)
{
    string result;

    char buff[50];

    snprintf(buff, 50, "%02i", m_Pattern + 1);
    result += buff;

    // Show nothing if we just play once (m_Repeats == 1).

    if ( forMenu )
    {
        if ( m_Repeats > 0 )
        {
            snprintf(buff, 50, "x%02i", m_Repeats + 1);
            result += buff;
        }
        else if ( m_Repeats < 0 )
            result += " Hold";

        if ( m_Repeats >= 0 && m_Jump >= 0 )
        {
            snprintf(buff, 50, ">%02i", m_Jump + 1);
            result += buff;
        }
    }
    else
    {
        if ( m_Remaining >= 0 )
        {
            snprintf(buff, 50, " x%02i", m_Remaining + 1);
            result += buff;
        }
        else if ( m_Repeats > 0 )
        {
            snprintf(buff, 50, " x%02i", m_Repeats + 1);
            result += buff;
        }
        else if ( m_Repeats < 0 )
            result += " H";

        if ( m_Repeats >= 0 && m_Jump >= 0 )
        {
            snprintf(buff, 50, " >%02i", m_Jump + 1);
            result += buff;
        }
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

    snprintf(buff, 50, "%i/%i/%i", m_Pattern, m_Repeats, m_Jump);

    return buff;
}

void ChainLink::FromString(string & s)
{
    vector<string> tokens = split(s.c_str(), '/');

#ifdef MA_BLUE
    if ( tokens.size() != 3 )
        return;

    m_Pattern = strtol(tokens.at(0).c_str(), NULL, 0);
    m_Repeats = strtol(tokens.at(1).c_str(), NULL, 0);
    m_Jump = strtol(tokens.at(2).c_str(), NULL, 0);

    // Check 'errno' for proper handling.
#else
    if ( tokens.size() != 3 )
        throw string("Pattern Chain parse error: nothing entered.");

    m_Pattern = stoi(tokens.at(0));
    m_Repeats = stoi(tokens.at(1));
    m_Jump = stoi(tokens.at(2));
#endif
}

void ChainLink::SetStatus()
{
    int pos = 0;
    char buff[50];

    m_FieldPositions.clear();
    m_Highlights.clear();

    snprintf(buff, 50, "[Chain Slot %02i] ", m_ItemID);
    m_Status = buff;

    m_Status += "Pattern ";
    pos = m_Status.size();
    snprintf(buff, 50, "%i", m_Pattern + 1);
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, m_Status.size() - pos);

    m_Status += ", Play ";
    pos = m_Status.size();
    if ( m_Repeats >= 0 )
    {
        snprintf(buff, 50, "x %i", m_Repeats + 1);
        m_Status += buff;
    }
    else
        m_Status += "(hold)";
    m_FieldPositions.emplace_back(pos, m_Status.size() - pos);

    m_Status += ", Jump ";
    pos = m_Status.size();
    if ( m_Jump >= 0 )
    {
        snprintf(buff, 50, "%i", m_Jump + 1);
        m_Status += buff;
    }
    else
        m_Status += "(off)";
    m_FieldPositions.emplace_back(pos, m_Status.size() - pos);

    m_Status += ' ';
    pos = m_Status.size();
    m_Status += "Jump Here";
    m_FieldPositions.emplace_back(pos, m_Status.size() - pos);

    m_Highlights.push_back(m_FieldPositions.at(m_PosEdit));
}

//bool ChainLink::HandleKey(key_type_t k)
bool ChainLink::HandleKey(BaseUI::key_command_t k)
{
    switch ( k )
    {
//    case enter:
    case BaseUI::key_return:
        if ( m_PosEdit == 3 && m_Parent != NULL )
        {
            m_Parent->at(m_Parent->PosPlay()).ClearRemaining();
            m_Parent->SetJumpOverride(m_ItemID - 1);
        }
        else
            ReturnFocus();
        break;

//    case back_space:
    case BaseUI::key_backspace:
        ReturnFocus();
        break;

//    case left:
    case BaseUI::key_left:
        if ( m_PosEdit > 0 )
            m_PosEdit -= 1;
        break;
//    case right:
    case BaseUI::key_right:
        if ( m_PosEdit < 3 )
            m_PosEdit += 1;
        break;
//    case up:
    case BaseUI::key_up:
        switch ( m_PosEdit )
        {
        case 0:     // Pattern
            m_Pattern += 1;
            m_FollowUp = update_pattern_browser;
            break;
        case 1:     // Repeats
            m_Repeats += 1;
            break;
        case 2:     // Jump
            m_Jump += 1;
            break;
        default:
            break;
        }
        break;
//    case down:
    case BaseUI::key_down:
        switch ( m_PosEdit )
        {
        case 0:     // Pattern
            if ( m_Pattern > 0 )
            {
                m_Pattern -= 1;
                m_FollowUp = update_pattern_browser;
            }
            break;
        case 1:     // Repeats
            if ( m_Repeats > -1 )
                m_Repeats -= 1;
            break;
        case 2:     // Jump
            if ( m_Jump > -1 )
                m_Jump -= 1;
            break;
        default:
            break;
        }
        break;
    default:
        return false;
    }

    m_FirstField = m_PosEdit == 0;

    SetStatus();

    return true;
}

