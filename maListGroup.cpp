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

//
//  ListGroup
//
///////////////////////////////////////////////////////////

void ListGroup::ResetPosition()
{
}

const char * g_LGTypeNames[] = {"Sequencer", "Looper"};

void ListGroup::SetStatus()
{
    int pos = 0;
    char buff[50];

    m_FieldPositions.clear();
    m_Highlights.clear();

    if ( m_GotFocus )
        snprintf(buff, 50, "[%s %i]", g_LGTypeNames[m_Type], m_ItemID);
    else
        snprintf(buff, 50, " %s %i ", g_LGTypeNames[m_Type], m_ItemID);

    InitStatus();
    m_Status += buff;

    m_Status += " Midi: ";
    pos = m_Status.size();
    snprintf(buff, 50, "%02i", m_MidiChannel);
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += " Step: ";
    pos = m_Status.size();
    snprintf(buff, 50, "%05.2f", m_Step);
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

//    if ( !m_FieldPositions.empty() )
//        m_Highlights.push_back(m_FieldPositions.at(m_PosEdit));
}

bool ListGroup::HandleKey(BaseUI::key_command_t k)
{
    switch ( k )
    {
    case BaseUI::key_return:
        break;

    case BaseUI::key_backspace:
    case BaseUI::key_escape:
        ReturnFocus();
        break;

    case BaseUI::key_left:
        break;

    case BaseUI::key_right:
        break;

    case BaseUI::key_ctrl_up:
        if ( m_MenuList != NULL )
            m_MenuList->DownCursorPos();
        return true;

    case BaseUI::key_ctrl_down:
        if ( m_MenuList != NULL )
            m_MenuList->UpCursorPos();
//        if ( m_ReturnFocus != NULL )
//        {
//            m_ReturnFocus->HandleKey(k);
////            m_ReturnFocus->HandleKey(BaseUI::key_return);
//        }
        return true;

    case BaseUI::key_ctrl_left:
        break;

    case BaseUI::key_ctrl_right:
        break;

    case BaseUI::key_shift_left:
        break;

    case BaseUI::key_shift_right:
        break;

    case BaseUI::key_shift_delete:
        break;

    default:
        return false;
    }

    return true;
}

//
//  StepListGroup
//
////////////////////////////////////////////////////////////////////

StepListGroup::StepListGroup():
    ListGroup(lgtype_step)
{
    m_DisplayObjectType = BaseUI::dot_step_list_group;
    m_PopUpMenuID = C_MENU_ID_SEQUENCE;
}

StepListGroup::StepListGroup(ListGroup * g):
    ListGroup(*g)
{
    m_StepListSet = dynamic_cast<StepListGroup*>(g)->m_StepListSet;
}

//StepListPtr StepListGroup::NewStepList()
StepList * StepListGroup::NewStepList()
{
    m_StepListSet.emplace_back();
//    m_StepListSet.emplace_back(new StepList());
    return & m_StepListSet.back();
}

void StepListGroup::AddToMenuList(MenuList & m)
{
    m.Add(this /*, true*/);   // Insert (& select?)
    for ( auto it = m_StepListSet.begin(); it != m_StepListSet.end(); it++ )
        m.Add(&*it);
}

//
// RTListGroup
//
////////////////////////////////////////////////////////////////////

RTListGroup::RTListGroup():
    ListGroup(lgtype_realtime)
{
    m_DisplayObjectType = BaseUI::dot_rt_list_group;
    m_PopUpMenuID = C_MENU_ID_LOOP;
}

RTListGroup::RTListGroup(ListGroup * g):
    ListGroup(*g)
{

}

void RTListGroup::AddToMenuList(MenuList & m)
{
    m.Add(this /*, true*/);   // Insert (& select?)
    for ( auto it = m_RealTimeSet.begin(); it != m_RealTimeSet.end(); it++ )
        m.Add(&*it);
}




