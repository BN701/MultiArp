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
#include "maPatternStore.h"
#include "maUtility.h"


using namespace std;

ChainLink::ChainLink(PatternStore * store):
    m_PatternStore(store)
{
    //ctor

    m_PatternLabel = m_PatternStore->CurrentEditPatternID();
    m_PatternHash = m_PatternStore->CurrentEditPatternHash();
}

ChainLink::~ChainLink()
{
    //dtor
}

void ChainLink::SetPattern(std::string label, int hash)
{
    m_PatternLabel = label;
    m_PatternHash = hash;
}

int ChainLink::Remaining()
{
    // This bit makes us loop indefinitely.

    if ( m_Repeats < 0 )
        return 1;

//    m_Parent->SetRedraw();  // Update the pattern chain display.

    // Initialize remaining loop count.

    if ( m_Remaining == -1 )
        m_Remaining = m_Repeats;

    return m_Remaining--;
}

bool ChainLink::RemainingIsOne()
{
    if ( m_Repeats < 0 )
        return false;
    else if ( m_Remaining == -1 )
        return m_Repeats == 0;
    else
        return m_Remaining == 0;
}

string ChainLink::ToStringForDisplay(bool forMenu, unsigned width)
{
    string result;

    char buff[50];

    snprintf(buff, 50, "%s", m_PatternLabel.c_str());
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
            snprintf(buff, 50, "x%02i", m_Remaining + 1);
            result += buff;
        }
        else if ( m_Repeats > 0 )
        {
            snprintf(buff, 50, "x%02i", m_Repeats + 1);
            result += buff;
        }
        else if ( m_Repeats < 0 )
            result += "-Hold";

//        if ( m_Repeats >= 0 && m_Jump >= 0 )
//        {
//            snprintf(buff, 50, " >%02i", m_Jump + 1);
//            result += buff;
//        }
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

    snprintf(buff, 50, "%s/%i/%i/%i", m_PatternLabel.c_str(), m_PatternHash, m_Repeats, m_Jump);

    return buff;
}

void ChainLink::FromString(string & s)
{
    vector<string> tokens = split(s.c_str(), '/');

    if ( tokens.size() != 4 )
        return;

    m_PatternLabel = tokens[0];
    m_PatternHash = stoi(tokens[1]);
    m_Repeats = stoi(tokens[2]);
    m_Jump = stoi(tokens[3]);
}

void ChainLink::SetStatus()
{
    int pos = 0;
    char buff[50];

    m_FieldPositions.clear();
    m_Highlights.clear();

//    if ( m_GotFocus )
//        snprintf(buff, 50, "[Chain Slot %02i] ", m_ItemID);
//    else
//        snprintf(buff, 50, " Chain Slot %02i  ", m_ItemID);

    InitStatus();
//    m_Status += buff;

//    m_Status += " ";
    pos = m_Status.size();
    snprintf(buff, 50, "%s", m_PatternLabel.c_str());
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, m_Status.size() - pos);

    m_Status += ", Repeat ";
    pos = m_Status.size();
    if ( m_Repeats >= 0 )
    {
        snprintf(buff, 50, "x %i", m_Repeats + 1);
        m_Status += buff;
    }
    else
        m_Status += "(hold)";
    m_FieldPositions.emplace_back(pos, m_Status.size() - pos);

//    m_Status += ", Jump ";
//    pos = m_Status.size();
//    if ( m_Jump >= 0 )
//    {
//        snprintf(buff, 50, "%i", m_Jump + 1);
//        m_Status += buff;
//    }
//    else
//        m_Status += "(off)";
//    m_FieldPositions.emplace_back(pos, m_Status.size() - pos);

    m_Status += ", ";
    pos = m_Status.size();
    m_Status += "Jump Here";
    m_FieldPositions.emplace_back(pos, m_Status.size() - pos);

    m_Highlights.push_back(m_FieldPositions.at(m_PosEdit));
}


bool ChainLink::HandleKey(BaseUI::key_command_t k)
{
    switch ( k )
    {
    case BaseUI::key_cmd_enter:
        if ( m_PosEdit == 2 /*&& m_Parent != NULL*/ )
        {
//            m_Parent->[m_Parent->PosPlay()].ClearRemaining();
            m_PatternStore->SetJumpOverride(this);
        }
        break;

    case BaseUI::key_cmd_back:
        if ( m_MenuListPtr != NULL )
        {
            m_MenuListPtr->m_Container->SetRedraw();
            m_MenuListPtr->DownCursorPos();
            m_MenuListPtr->Remove(m_PosInMenuList);
        }
        break;

    case BaseUI::key_cmd_left:
        if ( m_PosEdit > 0 )
            m_PosEdit -= 1;
        break;

    case BaseUI::key_cmd_right:
        if ( m_PosEdit < 2 )
            m_PosEdit += 1;
        break;

    case BaseUI::key_cmd_inc:
        switch ( m_PosEdit )
        {
        case 0:     // Pattern
            m_PatternStore->DownEditPos();
            m_PatternLabel = m_PatternStore->CurrentEditPatternID();
            m_PatternHash = m_PatternStore->CurrentEditPatternHash();
            break;
        case 1:     // Repeats
            m_Repeats += 1;
            break;
//        case 2:     // Jump
//            m_Jump += 1;
//            break;
        default:
            break;
        }
        break;

    case BaseUI::key_cmd_dec:
        switch ( m_PosEdit )
        {
        case 0:     // Pattern
            m_PatternStore->UpEditPos();
            m_PatternLabel = m_PatternStore->CurrentEditPatternID();
            m_PatternHash = m_PatternStore->CurrentEditPatternHash();
            break;
        case 1:     // Repeats
            if ( m_Repeats > -1 )
                m_Repeats -= 1;
            break;
//        case 2:     // Jump
//            if ( m_Jump > -1 )
//                m_Jump -= 1;
//            break;
        default:
            break;
        }
        break;
    default:
        return false;
    }

    m_FirstField = m_PosEdit == 0;

    SetRedraw();

    return true;
}

