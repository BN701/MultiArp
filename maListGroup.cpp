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


void ListGroup::ResetPosition()
{
}

const char * g_LGTypeNames[] = {"Step", "Real Time"};

void ListGroup::SetStatus()
{
    int pos = 0;
    char buff[50];

    m_FieldPositions.clear();
    m_Highlights.clear();

    snprintf(buff, 50, "%s %i", g_LGTypeNames[m_Type], m_ItemID);
    m_Status = buff;

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

StepListPtr StepListGroup::NewStepList()
{
    m_StepListSet.emplace_back(new StepList());
    return m_StepListSet.back();
//    m_PosEdit = m_StepListSet.size() - 1;

}
