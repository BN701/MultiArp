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

#include "maPatternStore.h"
#include "maProgressDisplay.h"
#include "maState.h"
#include "maUtility.h"

ProgressDisplay g_ProgressBar;

ProgressDisplay::ProgressDisplay()
{
    //ctor
    m_Visible = true;
    m_DisplayObjectType = BaseUI::dot_progress_bar;
}

ProgressDisplay::~ProgressDisplay()
{
    //dtor
}

void ProgressDisplay::Update()
{
    g_State.Progress(m_Progress, m_StepWidth);

    m_Phase =  g_State.Phase() + 1;
    m_Beat = g_PatternStore.LastRealTimeBeat();
//    m_ScheduleTimeSeonds = g_Sequencer.ScheduleTimeSeconds();
//    m_ScheduleTimeNanoSeconds = g_Sequencer.ScheduleTimeNanoSeconds() / 100000000);
    SetRedraw();

#if 0
    g_TextUI.Progress(m_Progress,
                    m_StepWidth,
                    g_State.Phase() + 1,
//                    g_PatternStore.CurrentPosPatternChain() + 1,
                    0,
                    g_PatternStore.LastRealTimeBeat(),
                    g_Sequencer.ScheduleTimeSeconds(),
                    g_Sequencer.ScheduleTimeNanoSeconds() / 100000000);

#ifndef MA_BLUE
    g_CairoUI.Progress(progress,
                    stepWidth,
                    g_State.Phase() + 1,
//                    g_PatternStore.CurrentPosPatternChain() + 1,
                    0,
                    g_PatternStore.LastRealTimeBeat(),
                    g_Sequencer.ScheduleTimeSeconds(),
                    g_Sequencer.ScheduleTimeNanoSeconds() / 100000000);
#endif
#endif
}

void ProgressDisplay::SetStatus()
{
    int pos = 0;
    char buff[70];

    m_FieldPositions.clear();
    m_Highlights.clear();

    uint32_t heap_top, stack_bottom, free_mem;

    MemStats(heap_top, stack_bottom, free_mem);

    snprintf(buff, 70, " Phase %5.2f, (Mem H/S/F): %i/%i/%i", m_Phase, heap_top, stack_bottom, free_mem);

    InitStatus();
    m_Status += buff;

//    if ( ! m_FieldPositions.empty() )
//        m_Highlights.push_back(m_FieldPositions.at(m_ListGroupMenuFocus));

}
