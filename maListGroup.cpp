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

//#include "maNotes.h"
#include "maListGroup.h"
#include "maPattern.h"
//#include "maRealTimeList.h"
//#include "maStepList.h"
#include "maState.h"
#include "maTranslateTable.h"
#include "maUtility.h"

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

extern State g_State;

//
//  ListGroup
//
///////////////////////////////////////////////////////////

int ListGroup::m_ListGroupCounter = 0;
map<int, ListGroup*> ListGroup::m_ListGroupsLookup;

ListGroup::ListGroup(Pattern & p, list_group_type type):
    m_Parent(p),
    m_Type(type)
{
    m_ListGroupsLookup.insert(m_ListGroupsLookup.end(), pair<int, ListGroup*>(m_ListGroupID, this) );
}

ListGroup::ListGroup(ListGroup & lg):
    ItemMenu(lg),
    m_Parent(lg.m_Parent),
    m_Type(lg.m_Type),
    m_MidiChannel(lg.m_MidiChannel),
    m_CurrentStepValue(lg.m_CurrentStepValue),
    m_Quantum(lg.m_Quantum)
{
    m_ListGroupsLookup.insert(m_ListGroupsLookup.end(), pair<int, ListGroup*>(m_ListGroupID, this) );
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
        snprintf(buff, 50, "[%s %i] -", g_LGTypeNames[m_Type], m_ListGroupID);
    else
        snprintf(buff, 50, " %s %i  -", g_LGTypeNames[m_Type], m_ListGroupID);

    InitStatus();
    m_Status += buff;

    m_Status += " Midi: ";
    pos = m_Status.size();
    snprintf(buff, 50, "%02i", m_MidiChannel + 1);
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += " Step:";
    pos = m_Status.size();
    snprintf(buff, 50, "%05.2f", m_StepEdit);
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    if ( !equals(m_CurrentStepValue, m_StepEdit) )
    {
        if ( !equals(m_CurrentStepValue, m_StepPending) )
            snprintf(buff, 50, ">%05.2f>%05.2f", m_StepPending, m_CurrentStepValue);
        else
            snprintf(buff, 50, ">%05.2f", m_CurrentStepValue);
        m_Status += buff;
    }


    m_Status += " Q:";
    pos = m_Status.size();
    snprintf(buff, 50, "%05.2f", m_QuantumEdit);
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    if ( !equals(m_Quantum, m_QuantumEdit) )
    {
        if ( !equals(m_Quantum, m_QuantumPending) )
            snprintf(buff, 50, ">%05.2f>%05.2f", m_QuantumPending, m_Quantum);
        else
            snprintf(buff, 50, ">%05.2f", m_Quantum);
        m_Status += buff;
    }

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
        switch ( m_ListGroupMenuFocus )
        {
        case lgp_midi_channel:
            break;
        case lgp_step_value:
//            m_CurrentStepValue += inc;
            m_StepPending = m_StepEdit;
            break;
        case lgp_quantum:
            m_QuantumPending = m_QuantumEdit;
            break;
        default:
            break;
        }
        break;

    case BaseUI::key_backspace:
    case BaseUI::key_escape:
        if ( ! equals(m_CurrentStepValue, m_StepEdit) )
        {
            m_StepEdit = m_CurrentStepValue;
            m_StepPending = m_CurrentStepValue;
        }
        else if ( ! equals(m_Quantum, m_QuantumEdit) )
        {
            m_QuantumEdit = m_Quantum;
            m_QuantumPending = m_Quantum;
        }
        else
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
//            m_CurrentStepValue += inc;
            m_StepEdit += inc;
            break;
        case lgp_quantum:
            m_QuantumEdit += inc;
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
//            if ( m_CurrentStepValue - inc > 0 )
//                m_CurrentStepValue -= inc;
            if ( m_StepEdit - inc > 0 )
                m_StepEdit -= inc;
            break;
        case lgp_quantum:
            if ( m_QuantumEdit - inc > 1 )
                m_QuantumEdit -= inc;
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
    snprintf(m_Progress, 20, " - %5.2f", m_Phase + 1);
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
    m_NextBeatSwung = nextBeatStrict;

#ifdef MA_BLUE
   // MA_BLUE Todo: Convert beat (phase) to schedule time
//   chrono::microseconds t_next_usec(llround(nextBeat * 60000000/120));

    g_State.SetPhase(fmod(nextBeatStrict, g_State.Quantum()));

#else
    ableton::Link::Timeline timeline = g_Link.captureAppTimeline();
    chrono::microseconds t_next_usec = timeline.timeAtBeat(m_NextBeatSwung, m_Quantum);

    m_Phase = timeline.phaseAtTime(t_next_usec, m_Quantum);
#endif

    if ( equals(m_Phase, 0) )
    {
        m_CurrentStepValue = m_StepPending;
        m_Quantum = m_QuantumPending;
    }

//    if ( g_State.PhaseIsZero() )
//    {
//       do_phase0_updates();
//       g_PatternStore.SetPhaseIsZero();
//       g_ListBuilder.SetPhaseIsZero(g_State.Beat(), g_State.Quantum());
//    }

    // Set next schedule time on the queue

//    uint64_t queue_time_usec = 0;

#ifdef MA_BLUE
//    m_QueueTimeUsec = llround(nextBeat * 60000000/120);
    m_QueueTimeUsec = g_State.TimeLineMicros();
    m_Tempo = g_State.Tempo();
#else
    if ( g_LinkStartTime.count() < 0 )
    {
       g_LinkStartTime = t_next_usec;
       m_QueueTimeUsec = 0;
    }
    else
    {
       m_QueueTimeUsec = (t_next_usec.count() - g_LinkStartTime.count());
    }

    m_Tempo = timeline.tempo();
#endif


    if ( m_QueueTimeUsec < 0 )
    {
       // Sometimes at start up link appears to go backwards, especially if
       // there's another instance of the app running. For now, just keep
       // reschedule and hope things settle down. This is probably our count
       // in, though I haven't thought it through properly.
    //        raise(SIGINT);
       m_QueueTimeUsec = 0;
    }

    g_Sequencer.SetScheduleTime(m_QueueTimeUsec);

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

StepListGroup::StepListGroup(Pattern & p):
    ListGroup(p, lgtype_step)
{
    m_DisplayObjectType = BaseUI::dot_step_list_group;
    m_PopUpMenuID = C_MENU_ID_SEQUENCE;
}

StepListGroup::StepListGroup(StepListGroup * g):
    ListGroup(*g)
{
    m_StepListSet = dynamic_cast<StepListGroup*>(g)->m_StepListSet;
}

StepList * StepListGroup::NewStepList()
{
    m_StepListSet.emplace_back();
    m_StepListSet.back().SetItemID(m_StepListSet.size() - 1);
    return & m_StepListSet.back();
}

void StepListGroup::CopyList(StepList * pItem, MenuList & menu)
{
    if ( pItem != NULL )
    {
        // Get list position.

        int pos = pItem->ItemID();

        // Remove references to this item and those beyond it.
        // (Inserting an item in a vector shuffles everything up
        // so pointers, etc. to those items become invalid.)

        menu_list_cursor_t menuInsertPos;
        for ( auto it = m_StepListSet.begin() + pos; it != m_StepListSet.end(); it++ )
        {
            menuInsertPos = menu.Remove(it->MenuPos()/*, false*/);
            m_RedrawList.remove(&*it);
        }

        // Insert a copy.

        m_StepListSet.insert(m_StepListSet.begin() + pos, *pItem);

        // Renumber any lists starting at the copied item and put them back in
        // the menu list.

        int itemId = pos;
        for ( auto it = m_StepListSet.begin() + pos; it != m_StepListSet.end(); it++ )
        {
            it->SetItemID(itemId++);
            menu.Insert(menuInsertPos, &*it);
        }

        menu.Select(m_StepListSet[pos].MenuPos());

        // Renumber lists in the deferred update queue.

        for ( auto it = m_DeferredUpdates.begin(); it != m_DeferredUpdates.end(); it++ )
            if ( it->list_id > pos )
                it->list_id++;
    }
}

void StepListGroup::DeleteList(StepList * pItem, MenuList & menu)
{
    if ( pItem != NULL )
    {
        // Move cursor out of the way.

        menu.Select(m_PosInMenuList);

        // Get list position and then delete the list.

        int pos = pItem->ItemID();

        // Remove references to this item and those beyond it.
        // (Erasing an item in a vector shuffles everything down
        // so pointers, etc. to those items become invalid.)

        for ( auto it = m_StepListSet.begin() + pos; it != m_StepListSet.end(); it++ )
        {
            menu.Remove(it->MenuPos()/*, false*/);
            m_RedrawList.remove(&*it);
        }

        // Erase the list.

        m_StepListSet.erase(m_StepListSet.begin() + pos);

        // Renumber any lists beyond deleted item and put them back in
        // the menu list.

        auto menuPos = m_PosInMenuList;
        for ( int i = 0; i < pos; i++ )
            menuPos++;
        for ( auto it = m_StepListSet.begin() + pos; it != m_StepListSet.end(); it++ )
        {
            it->SetItemID(it->ItemID() - 1);
            menuPos = menu.InsertAfter(menuPos, &*it);
        }

        // Reselect: Cursor stays at same position, or moves up if end of list.

        if ( pos < m_StepListSet.size() )
            menu.Select((m_StepListSet.begin() + pos)->MenuPos());
        else if ( ! m_StepListSet.empty() )
            menu.Select(m_StepListSet.back().MenuPos());


        // Renumber lists in the deferred update queue (and remove the deleted list, if present).

        for ( auto it = m_DeferredUpdates.begin(); it != m_DeferredUpdates.end(); it++ )
            if ( it->list_id == pos )
                m_DeferredUpdates.erase(it);
            else if ( it->list_id > pos )
                it->list_id--;
    }
}

void StepListGroup::MoveListUp(StepList * pItem, MenuList & menu)
{
    MoveList(pItem, menu, true);
}

void StepListGroup::MoveListDown(StepList * pItem, MenuList & menu)
{
    MoveList(pItem, menu, false);
}

void StepListGroup::MoveList(StepList * pItem, MenuList & menu, bool up)
{
    if ( pItem != NULL )
    {
        // Get list position and then delete the list.

        int pos = pItem->ItemID();

        if ( up )
        {
            if ( pos == 0 )
                return;
        }
        else
        {
            if ( pos == m_StepListSet.size() - 1 )
                return;
            pos++;
        }

        // Swap two items. The process is the same regardless
        // of which one was selected.

        m_RedrawList.remove(&m_StepListSet[pos]);
        m_RedrawList.remove(&m_StepListSet[pos - 1]);

        auto menuInsertPos = menu.Remove(m_StepListSet[pos - 1].MenuPos()/*, false*/);
        menu.Remove(m_StepListSet[pos].MenuPos()/*, false*/);

        StepList temp = m_StepListSet[pos];
        m_StepListSet[pos] = m_StepListSet[pos - 1];
        m_StepListSet[pos].SetItemID(pos);

        m_StepListSet[pos - 1] = temp;
        m_StepListSet[pos - 1].SetItemID(pos - 1);

        menu.Insert(menuInsertPos, &m_StepListSet[pos - 1]);
        menu.Insert(menuInsertPos, &m_StepListSet[pos]);

        if ( up )
            menu.Select(m_StepListSet[pos - 1].MenuPos());
        else
            menu.Select(m_StepListSet[pos].MenuPos());
    }
}

void StepListGroup::AddToMenuList(MenuList & m)
{
    m.Add(this /*, true*/);   // Add (& select?)
    for ( auto it = m_StepListSet.begin(); it != m_StepListSet.end(); it++ )
        m.Add(&*it);
}

void StepListGroup::InsertListsIntoMenu(menu_list_cursor_t before)
{
    if ( m_MenuList == NULL )
        return;

    for ( auto it = m_StepListSet.begin(); it != m_StepListSet.end(); it++ )
        m_MenuList->Insert(before, &*it);
}

void StepListGroup::RemoveListsFromMenu()
{
    if ( m_MenuList == NULL )
        return;

    for ( auto it = m_StepListSet.begin(); it != m_StepListSet.end(); it++ )
        m_MenuList->Remove(it->MenuPos());
}

void StepListGroup::StepTheLists(Cluster & cluster, TrigRepeater & repeater,
    double & stepValueMultiplier, double phase, double stepValue, double globalBeat)
{
    if ( ! m_StepListSet.empty() )
    {
        unsigned loopCheck = 0;

        for ( auto it = m_StepListSet.begin(); it != m_StepListSet.end(); it++ )
            it->SetNowPlayingPos(-1);

        for ( auto it = m_DeferredUpdates.begin(); it != m_DeferredUpdates.end(); it++ )
        {
            update_pair & u = m_DeferredUpdates.front();
            if ( u.list_id < m_StepListSet.size() )
            {
                StepList & l = m_StepListSet[u.list_id];
                l.SetNowPlayingPos(u.list_pos);
            }
        }

        m_DeferredUpdates.clear();

        if ( m_TrigList.Empty() )
        {
            StepList * l = NULL;

            while ( loopCheck < m_StepListSet.size() )
            {
                if ( m_Pos >= m_StepListSet.size() )
                    m_Pos = 0;

                l = & m_StepListSet[m_Pos];

                m_Pos++;

                if ( ! l->Empty() )
                    break;

                loopCheck++;
            }

            update_pair u;
            u.list_id = m_Pos;
            u.list_pos = l->m_Pos;
            m_DeferredUpdates.push_back(u);

            Cluster * result = l->Step();

            if ( result != NULL )
            {
                cluster.SetStepsTillNextNote(result->StepsTillNextNote());
                cluster += *result;
            }

        }
//        else while ( loopCheck < m_TrigList.Size() ) // Loop only if skipping entries.
//        {
//            TrigListItem * trigItem = m_TrigList.Step();
//
//            if ( trigItem->Skip() )
//            {
//                loopCheck += 1;
//                continue;
//            }
//
//            if ( trigItem->Mute() )
//                break;
//
//            stepValueMultiplier = trigItem->Multiplier();
//
//            repeater = trigItem->Repeater();
//
//            for ( vector<int>::iterator it = trigItem->Trigs().begin(); it < trigItem->Trigs().end(); it++ )
//            {
//                // Just ignore list number if list doesn't exist.
//
//                if ( static_cast<unsigned>(*it) < m_StepListSet.size() )
//                {
//                    m_LastRequestedPos = *it;
//
//                    Cluster * result = m_StepListSet.at(*it).Step();
//
//                    if ( result != NULL )
//                    {
//                        cluster.SetStepsTillNextNote(result->StepsTillNextNote());
//                        cluster += *result;
//                    }
//                }
//            }
//
//            break;
//        }

//        if ( m_StepListSet.size() == 1 && ! m_DeferredRedrawList.empty() )
//        {
//            m_DeferredRedrawList.front()->SetRedraw();
//            m_DeferredRedrawList.pop();
//        }
    }

    // All notes collected so far are from step lists, so they won't have any
    // phase start times set. Fix that now ...

    cluster.SetPhaseAllNotes(phase);

}

void StepListGroup::Step(int queueId)
{
//    uint64_t queueTimeUsec = 0;
//    double nextBeatSwung;

    ListGroup::Step(queueId);

    Cluster nextCluster;
    TrigRepeater repeater;
    TranslateTable & translator = m_Parent.m_TranslateTable;
    double stepValueMultiplier; // Todo: work out how to use this from here (it's filled in
                                // from the TrigList via StepTheLists() ...

    if ( g_State.RunState() /*|| g_DeferStop-- > 0*/ )
    {
        StepTheLists(nextCluster, repeater, stepValueMultiplier, m_Phase, m_CurrentStepValue, m_NextBeatSwung);
//        if ( g_ListBuilder.RealTimeRecord() )
//            nextCluster += *g_ListBuilder.Step(g_State.Phase(), g_State.LastUsedStepValue());
    }

    if ( nextCluster.Empty() )
       return;

    /*
         V, Step Value, is 4 x 'steps per beat'. (This gives the familiar
         eighth, sixteenths, etc). T, tempo, is 'beats per minute'.

         Steps per beat, v = V/4.
         Steps per minute = Tv = TV/4
         Steps per second = TV/240
         Step length in mSec = 1000*240/TV
    */

    double stepLengthMilliSecs = 240000.0/(m_Tempo * m_CurrentStepValue);
    unsigned int duration = lround(stepLengthMilliSecs * (nextCluster.StepsTillNextNote() + m_Parent.m_Gate));

    repeater.Init(m_Tempo, stepLengthMilliSecs);

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

       double phaseAdjust = note->Phase() - m_Phase;
       int64_t timeAdjust = llround(60000000.0 * phaseAdjust/m_Tempo);

       int64_t queue_time_adjusted = m_QueueTimeUsec + timeAdjust;

       if ( note->m_NoteVelocity > 0 )
           noteVelocity = note->m_NoteVelocity;
       else
           noteVelocity = m_Parent.m_Velocity;

       double noteLength = note->Length();
       if ( lround(noteLength * 100) > 0 )
       {
           // Note length here is in beats. Convert to milliseconds.
           duration = lround(60000.0 * noteLength / m_Tempo);
       }

       int64_t queue_time_delta = 0;
       int interval = 0;
       repeater.Reset(noteVelocity);

       do
       {
           int note = translator.TranslateUsingNoteMap(noteNumber, interval);
           g_Sequencer.SetScheduleTime(queue_time_adjusted + queue_time_delta);
           g_Sequencer.ScheduleNote(queueId, note, noteVelocity, duration, m_MidiChannel);
       }
       while ( repeater.Step(queue_time_delta, interval, noteVelocity) );
    }

}

//
// RTListGroup
//
////////////////////////////////////////////////////////////////////

RTListGroup::RTListGroup(Pattern & p):
    ListGroup(p, lgtype_realtime)
{
    m_DisplayObjectType = BaseUI::dot_rt_list_group;
    m_PopUpMenuID = C_MENU_ID_LOOP;
}

RTListGroup::RTListGroup(RTListGroup * g):
    ListGroup(*g)
{

}

void RTListGroup::AddToMenuList(MenuList & m)
{
    m.Add(this /*, true*/);   // Add (& select?)
    for ( auto it = m_RealTimeSet.begin(); it != m_RealTimeSet.end(); it++ )
        m.Add(&*it);
}

void RTListGroup::InsertListsIntoMenu(menu_list_cursor_t before)
{

}

void RTListGroup::RemoveListsFromMenu()
{

}


void RTListGroup::Step(int queueId)
{
    ListGroup::Step(queueId);
}



