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

using namespace std;

#ifdef MA_BLUE

#include <math.h>
#include "maSequencer.h"
extern Sequencer g_Sequencer;

#else

#define LINK_PLATFORM_LINUX
#include <ableton/Link.hpp>

// Global Link instance.

extern ableton::Link g_Link;
extern chrono::microseconds g_LinkStartTime;

#include "platform_Linux/maAlsaSequencer.h"
extern AlsaSequencer g_Sequencer;

#endif // MA_BLUE

//
//  ListGroup
//
///////////////////////////////////////////////////////////

int ListGroup::m_ListGroupCounter = 0;
map<int, ListGroup*> ListGroup::m_ListGroupsLookup;

ListGroup::ListGroup(list_group_type type):
    m_Type(type)
{
    m_ListGroupsLookup.insert(m_ListGroupsLookup.end(), pair<int, ListGroup*>(m_ListGroupID, this) );
}

ListGroup::ListGroup(ListGroup & lg):
    m_Type(lg.m_Type),
    m_MidiChannel(lg.m_MidiChannel),
    m_CurrentStepValue(lg.m_CurrentStepValue),
    m_Quantum(lg.m_Quantum)
{
}

ListGroup::~ListGroup()
{
    auto it = m_ListGroupsLookup.find(m_ListGroupID);

    if ( it != m_ListGroupsLookup.end() && it->second == this )
        m_ListGroupsLookup.erase(m_ListGroupID);
}

void ListGroup::ResetPosition()
{
}


const char * g_LGTypeNames[] = {"Sequence", "Looper"};

void ListGroup::SetStatus()
{
    int pos = 0;
    char buff[50];

    m_FieldPositions.clear();
    m_Highlights.clear();

    if ( m_GotFocus )
        snprintf(buff, 50, "[%s] -", g_LGTypeNames[m_Type]);
    else
        snprintf(buff, 50, " %s  -", g_LGTypeNames[m_Type]);

    InitStatus();
    m_Status += buff;

    m_Status += " Midi: ";
    pos = m_Status.size();
    snprintf(buff, 50, "%02i", m_MidiChannel + 1);
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += " Step:";
    pos = m_Status.size();
    snprintf(buff, 50, "%05.2f", m_CurrentStepValue);
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += " (Q:";
    pos = m_Status.size();
    snprintf(buff, 50, "%05.2f", m_Quantum);
    m_Status += buff;
    m_Status += ")";
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += m_Progress;

    if ( m_GotFocus && !m_FieldPositions.empty() )
        m_Highlights.push_back(m_FieldPositions.at(m_ListGroupMenuFocus));
}

bool ListGroup::HandleKey(BaseUI::key_command_t k)
{
    int temp = 0;
    double inc = 0.01;

    switch ( k )
    {
    case BaseUI::key_return:
        break;

    case BaseUI::key_backspace:
    case BaseUI::key_escape:
        ReturnFocus();
        return true;

    case BaseUI::key_ctrl_up:
        if ( m_MenuList != NULL )
            m_MenuList->DownCursorPos();
        return true;

    case BaseUI::key_ctrl_down:
        if ( m_MenuList != NULL )
            m_MenuList->UpCursorPos();
        return true;

    case BaseUI::key_left:
        temp = static_cast<int>(m_ListGroupMenuFocus) - 1;
        if ( temp >= 0 && temp < num_listgroup_params_menu_focus_modes )
            m_ListGroupMenuFocus = static_cast<listgroup_params_menu_focus_t>(temp);
        break;

    case BaseUI::key_right:
        temp = static_cast<int>(m_ListGroupMenuFocus) + 1;
        if ( temp >= 0 && temp < num_listgroup_params_menu_focus_modes )
            m_ListGroupMenuFocus = static_cast<listgroup_params_menu_focus_t>(temp);
        break;

    case BaseUI::key_shift_up:
        inc = 1;
    case BaseUI::key_up:
        switch ( m_ListGroupMenuFocus )
        {
        case lgp_midi_channel:
            if ( m_MidiChannel + 1 < 16 )
                m_MidiChannel += 1;
            break;
        case lgp_step_value:
            m_CurrentStepValue += inc;
            break;
        case lgp_quantum:
            m_Quantum += inc;
            break;
        default:
            break;
        }
        break;

    case BaseUI::key_shift_down:
        inc = 1;
    case BaseUI::key_down:
        switch ( m_ListGroupMenuFocus )
        {
        case lgp_midi_channel:
            if ( m_MidiChannel - 1 >= 0 )
                m_MidiChannel -= 1;
            break;
        case lgp_step_value:
            if ( m_CurrentStepValue - inc > 0 )
                m_CurrentStepValue -= inc;
            break;
        case lgp_quantum:
            if ( m_Quantum - inc > 0 )
                m_Quantum -= inc;
            break;
        default:
            break;
        }
        break;

    default:
        return false;
    }

    SetRedraw();
    return true;
}

// Static callback routing lookup.

bool ListGroup::Step(int listGroupID, int queueId)
{
    auto pos = m_ListGroupsLookup.find(listGroupID);
    if ( pos == m_ListGroupsLookup.end() )
        return false;
    pos->second->Step(queueId);
    return true;
}

void ListGroup::Step(int queueId)
{
    // In the main loop, this is where we update progress indicators.
    // With the ItemMenu deferred drawing mechanism this item
    // is placed on the redraw queue but nothing is actually displayed until
    // later. If values are about to be changed, we should store specific
    // progress data now. (I don't think it matters if the SetRedraw() call
    // happens now or at the end of this function. It'll be processed when
    // this has all completed.)

#if 1
    snprintf(m_Progress, 20, " %5.2f", m_Phase + 1);
#else
    snprintf(m_Progress, 20, " %5.2f - %.2f", m_Phase + 1, m_Beat);
#endif
    SetRedraw();

    // Now incrememt the step/beat and get on with scheduling the next events.

//->    g_State.Step(g_PatternStore.StepValueMultiplier());
//    m_LastUsedStepValue = m_CurrentStepValue;
    double stepValueMultiplier = 1.0;
    double beatInc = 4.0 * stepValueMultiplier / m_CurrentStepValue;
    m_Beat += beatInc;
#if defined(MA_BLUE)
    m_TimeLineMicros += beatInc * 60000000 / m_Tempo;
#endif

    // Get time of next step from Link.


    double nextBeatStrict = m_Beat;    // This is absolute beat value since the clock started,

//->    double nextBeatSwung = g_PatternStore.FeelMapForPlay().Adjust(nextBeatStrict);
        double nextBeatSwung = nextBeatStrict;

#ifdef MA_BLUE
   // MA_BLUE Todo: Convert beat (phase) to schedule time
//   chrono::microseconds t_next_usec(llround(nextBeat * 60000000/120));

    g_State.SetPhase(fmod(nextBeatStrict, g_State.Quantum()));

#else
    ableton::Link::Timeline timeline = g_Link.captureAppTimeline();
    chrono::microseconds t_next_usec = timeline.timeAtBeat(nextBeatSwung, m_Quantum);

    m_Phase = timeline.phaseAtTime(t_next_usec, m_Quantum);
#endif


//    if ( g_State.PhaseIsZero() )
//    {
//       do_phase0_updates();
//       g_PatternStore.SetPhaseIsZero();
//       g_ListBuilder.SetPhaseIsZero(g_State.Beat(), g_State.Quantum());
//    }

    // Set next schedule time on the queue

    uint64_t queue_time_usec = 0;

#ifdef MA_BLUE
//    queue_time_usec = llround(nextBeat * 60000000/120);
    queue_time_usec = g_State.TimeLineMicros();
#else
    if ( g_LinkStartTime.count() < 0 )
    {
       g_LinkStartTime = t_next_usec;
       queue_time_usec = 0;
    }
    else
    {
       queue_time_usec = (t_next_usec.count() - g_LinkStartTime.count());
    }
#endif

    if ( queue_time_usec < 0 )
    {
       // Sometimes at start up link appears to go backwards, especially if
       // there's another instance of the app running. For now, just keep
       // reschedule and hope things settle down. This is probably our count
       // in, though I haven't thought it through properly.
    //        raise(SIGINT);
       queue_time_usec = 0;
    }

    g_Sequencer.SetScheduleTime(queue_time_usec);

    // Schedule an event to be fired back to our own app which prompts another
    // arpeggio to be placed in the queue.

    // TODO: We used to do this after scheduling all midi events. Have there
    //       been any noticable effects of doing it before?

    g_Sequencer.ScheduleNextCallBack(queueId, m_ListGroupID);

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
    m.Add(this /*, true*/);   // Add (& select?)
    for ( auto it = m_StepListSet.begin(); it != m_StepListSet.end(); it++ )
        m.Add(&*it);
}

void StepListGroup::Step(int queueId)
{
    ListGroup::Step(queueId);

    Cluster nextCluster;
    TrigRepeater repeater;
    TranslateTable & translator = g_PatternStore.TranslateTableForPlay();

    if ( g_State.RunState() || gDeferStop-- > 0 )
    {
        g_PatternStore.Step(nextCluster, repeater, g_State.Phase(), g_State.LastUsedStepValue(), nextBeatSwung);
        if ( g_ListBuilder.RealTimeRecord() )
            nextCluster += *g_ListBuilder.Step(g_State.Phase(), g_State.LastUsedStepValue());
    }

    if ( nextCluster.Empty() )
       return;

#ifdef MA_BLUE
    double tempo = g_State.Tempo();
#else
    double tempo = timeline.tempo();
#endif

    /*
         V, Step Value, is 4 x 'steps per beat'. (This gives the familiar
         eighth, sixteenths, etc). T, tempo, is 'beats per minute'.

         Steps per beat, v = V/4.
         Steps per minute = Tv = TV/4
         Steps per second = TV/240
         Step length in mSec = 1000*240/TV
    */

    double stepLengthMilliSecs = 240000.0/(tempo * g_State.LastUsedStepValue());
    unsigned int duration = lround(stepLengthMilliSecs * (nextCluster.StepsTillNextNote() + g_PatternStore.GateLength()));

    repeater.Init(tempo, stepLengthMilliSecs);

    for ( auto note = nextCluster.m_Notes.begin(); note != nextCluster.m_Notes.end(); note++ )
    {
       int noteNumber = note->m_NoteNumber;

       if ( noteNumber < 0 )
           continue;

       unsigned char noteVelocity;

       // For real time events, move the note ahead or behind
       // the phase value of the step itself. (We can't move
       // too far ahead, obviously, but there's no mechanism
       // yet for dealing with that situation if it happens.)

       double phaseAdjust = note->Phase() - g_State.Phase();
       int64_t timeAdjust = llround(60000000.0 * phaseAdjust/tempo);

       int64_t queue_time_adjusted = queue_time_usec + timeAdjust;

       if ( note->m_NoteVelocity > 0 )
           noteVelocity = note->m_NoteVelocity;
       else
           noteVelocity = g_PatternStore.NoteVelocity();

       double noteLength = note->Length();
       if ( lround(noteLength * 100) > 0 )
       {
           // Note length here is in beats. Convert to milliseconds.
           duration = lround(60000.0 * noteLength / tempo);
       }

       int64_t queue_time_delta = 0;
       int interval = 0;
       repeater.Reset(noteVelocity);

       do
       {
           int note = translator.TranslateUsingNoteMap(noteNumber, interval);
           g_Sequencer.SetScheduleTime(queue_time_adjusted + queue_time_delta);
           g_Sequencer.ScheduleNote(queueId, note, noteVelocity, duration);
       }
       while ( repeater.Step(queue_time_delta, interval, noteVelocity) );
    }

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
    m.Add(this /*, true*/);   // Add (& select?)
    for ( auto it = m_RealTimeSet.begin(); it != m_RealTimeSet.end(); it++ )
        m.Add(&*it);
}


void RTListGroup::Step(int queueId)
{
    ListGroup::Step(queueId);
}



