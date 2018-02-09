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

#include <cmath>

using namespace std;

#ifdef MA_BLUE

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

#include "maCursorKeys.h"
#include "maListBuilder.h"
#include "maPatternStore.h"
#include "maScreen.h"
#include "maState.h"
#include "maStep.h"
//#include "maUtility.h"


// Global instances.

extern ListBuilder g_ListBuilder;
extern PatternStore g_PatternStore;
extern State g_State;

//extern Display g_Display;

int gDeferStop = 0;

void do_UI_updates()
{
    // If pattern changed last step ...

// Hack    if ( g_PatternStore.PatternChanged(true) )
// Hack    {
// Hack        set_top_line();
// Hack        update_edit_panels();
// Hack        set_status(STAT_POS_2, "Pattern changed ...");
// Hack    }

    // If phase zero last step ...

    if ( g_State.PhaseIsZero() )
    {
        update_pattern_status_panel();
    }

    // Every step ...

    update_pattern_panel();
    update_progress_bar();
}

void do_phase0_updates()
{
// Hack    g_PatternStore.TranslateTableForPlay().Diags().ResetLog();
// Hack
// Hack    g_State.SetCurrentStepValue(g_PatternStore.StepValue());
// Hack
// Hack    if ( g_State.NewQuantumPending() )
// Hack    {
// Hack        set_status(STAT_POS_2, "New quantum value set.");
// Hack    }
// Hack
// Hack    if ( g_PatternStore.TranslateTableForPlay().NewTransposePending() )
// Hack    {
// Hack        set_status(STAT_POS_2, "Transpose set.");
// Hack    }
// Hack
// Hack    if ( g_PatternStore.NewPatternPending() )
// Hack    {
// Hack        set_status(STAT_POS_2, "Pattern changed.");
// Hack    }
// Hack
// Hack    if ( g_State.NewRunStatePending() )
// Hack    {
// Hack        if ( g_State.RunState() )
// Hack            g_PatternStore.ResetAllPatterns();
// Hack        gDeferStop = g_State.DeferStop();
// Hack    }
// Hack
// Hack    if ( g_State.PatternReset() != RESET_NONE )
// Hack    {
// Hack        switch ( g_State.PatternReset() )
// Hack        {
// Hack            case RESET_ALL :
// Hack                g_PatternStore.ResetAllPatterns();
// Hack                set_status(STAT_POS_2, "All patterns were reset.");
// Hack                break;
// Hack            case RESET_CURRENT :
// Hack                g_PatternStore.ResetCurrentPattern();
// Hack                set_status(STAT_POS_2, "Current pattern was reset.");
// Hack                break;
// Hack        }
// Hack        g_State.SetPatternReset(RESET_NONE);
// Hack    }

    set_top_line();
}


void queue_next_step(int queueId)
{
// Hack    // We're called when ALSA has played the events we scheduled last time we were here,
// Hack    // so updating position info at this point should reflect what we are hearing.
// Hack
// Hack    do_UI_updates();
// Hack
// Hack    // Now incrememt the step/beat and get on with scheduling the next events.
// Hack
// Hack    g_State.Step(g_PatternStore.StepValueMultiplier());
// Hack
// Hack    // Get time of next step from Link.
// Hack
// Hack
// Hack    double nextBeat = g_State.Beat();
// Hack
// Hack    nextBeat = g_PatternStore.FeelMapForPlay().Adjust(nextBeat);
// Hack
// Hack#ifdef MA_BLUE
// Hack    // MA_BLUE Todo: Convert beat (phase) to schedule time
// Hack    chrono::microseconds t_next_usec;
// Hack#else
// Hack    ableton::Link::Timeline timeline = g_Link.captureAppTimeline();
// Hack    chrono::microseconds t_next_usec = timeline.timeAtBeat(nextBeat, g_State.Quantum());
// Hack
// Hack    g_State.SetPhase(timeline.phaseAtTime(t_next_usec, g_State.Quantum()));
// Hack#endif
// Hack
// Hack
// Hack    if ( g_State.PhaseIsZero() )
// Hack    {
// Hack        do_phase0_updates();
// Hack        g_PatternStore.SetPhaseIsZero();
// Hack// Hack        g_ListBuilder.SetPhaseIsZero(g_State.Beat(), g_State.Quantum());
// Hack    }
// Hack
// Hack    // Set next schedule time on the queue
// Hack
// Hack    uint64_t queue_time_usec = 0;
// Hack
// Hack#ifndef MA_BLUE
// Hack    if ( g_LinkStartTime.count() < 0 )
// Hack    {
// Hack        g_LinkStartTime = t_next_usec;
// Hack        queue_time_usec = 0;
// Hack    }
// Hack    else
// Hack    {
// Hack        queue_time_usec = (t_next_usec.count() - g_LinkStartTime.count());
// Hack    }
// Hack#endif
// Hack
// Hack    if ( queue_time_usec < 0 )
// Hack    {
// Hack        // Sometimes at start up link appears to go backwards, especially if
// Hack        // there's another instance of the app running. For now, just keep
// Hack        // reschedule and hope things settle down. This is probably our count
// Hack        // in, though I haven't thought it through properly.
// Hack//        raise(SIGINT);
// Hack        queue_time_usec = 0;
// Hack    }
// Hack
// Hack    g_Sequencer.SetScheduleTime(queue_time_usec);
// Hack
// Hack    // Schedule an event to be fired back to our own app which prompts another
// Hack    // arpeggio to be placed in the queue.
// Hack
// Hack    // TODO: We used to do this after scheduling all midi events. Have there
// Hack    //       been any noticable effects of doing it before?
// Hack
// Hack    g_Sequencer.ScheduleNextCallBack(queueId);
// Hack
// Hack    // Step the Pattern Store
// Hack
// Hack    Cluster nextCluster;
// Hack    TrigRepeater repeater;
// Hack    TranslateTable & translator = g_PatternStore.TranslateTableForPlay();
// Hack
// Hack    if ( g_State.RunState() || gDeferStop-- > 0 )
// Hack    {
// Hack        g_PatternStore.Step(nextCluster, repeater, g_State.Phase(), g_State.LastUsedStepValue(), nextBeat);
// Hack// Hack if ( g_ListBuilder.RealTimeRecord() )
// Hack// Hack     nextCluster += *g_ListBuilder.Step(g_State.Phase(), g_State.LastUsedStepValue());
// Hack    }
// Hack
// Hack    if ( nextCluster.Empty() )
// Hack        return;
// Hack
// Hack#ifdef MA_BLUE
// Hack    double tempo = 120.0;
// Hack#else
// Hack    double tempo = timeline.tempo();
// Hack#endif
// Hack
// Hack    /*
// Hack          V, Step Value, is 4 x 'steps per beat'. (This gives the familiar
// Hack          eighth, sixteenths, etc). T, tempo, is 'beats per minute'.
// Hack
// Hack          Steps per beat, v = V/4.
// Hack          Steps per minute = Tv = TV/4
// Hack          Steps per second = TV/240
// Hack          Step length in mSec = 1000*240/TV
// Hack     */
// Hack
// Hack    double stepLengthMilliSecs = 240000.0/(tempo * g_State.LastUsedStepValue());
// Hack    unsigned int duration = lround(stepLengthMilliSecs * (nextCluster.StepsTillNextNote() + g_PatternStore.GateLength()));
// Hack
// Hack    repeater.Init(tempo, stepLengthMilliSecs);
// Hack
// Hack    for ( auto note = nextCluster.m_Notes.begin(); note != nextCluster.m_Notes.end(); note++ )
// Hack    {
// Hack        int noteNumber = note->m_NoteNumber;
// Hack
// Hack        if ( noteNumber < 0 )
// Hack            continue;
// Hack
// Hack        unsigned char noteVelocity;
// Hack
// Hack        // For real time events, move the note ahead or behind
// Hack        // the phase value of the step itself. (We can't move
// Hack        // too far ahead, obviously, but there's no mechanism
// Hack        // yet for dealing with that situation if it happens.)
// Hack
// Hack        double phaseAdjust = note->Phase() - g_State.Phase();
// Hack        int64_t timeAdjust = llround(60000000.0 * phaseAdjust/tempo);
// Hack
// Hack        int64_t queue_time_adjusted = queue_time_usec + timeAdjust;
// Hack
// Hack        if ( note->m_NoteVelocity > 0 )
// Hack            noteVelocity = note->m_NoteVelocity;
// Hack        else
// Hack            noteVelocity = g_PatternStore.NoteVelocity();
// Hack
// Hack        double noteLength = note->Length();
// Hack        if ( lround(noteLength * 100) > 0 )
// Hack        {
// Hack            // Note length here is in beats. Convert to milliseconds.
// Hack            duration = lround(60000.0 * noteLength / tempo);
// Hack        }
// Hack
// Hack        int64_t queue_time_delta = 0;
// Hack        int interval = 0;
// Hack        repeater.Reset(noteVelocity);
// Hack
// Hack        do
// Hack        {
// Hack            int note = translator.TranslateUsingNoteMap(noteNumber, interval);
// Hack            g_Sequencer.SetScheduleTime(queue_time_adjusted + queue_time_delta);
// Hack            g_Sequencer.ScheduleNote(queueId, note, noteVelocity, duration);
// Hack        }
// Hack        while ( repeater.Step(queue_time_delta, interval, noteVelocity) );
// Hack    }

}

void midi_action(int queueId)
{
    snd_seq_event_t *ev;

    // static ListBuilder listBuilder;

    static int otherEvents = 0; // Just for interest.

    do
    {
        g_Sequencer.EventInput(&ev);
        switch (ev->type)
        {
        case SND_SEQ_EVENT_ECHO:
            // This is our 'tick', so schedule everything
            // that should happen next, including the
            // next tick.
            queue_next_step(queueId);
            break;

        case SND_SEQ_EVENT_NOTEON:
        case SND_SEQ_EVENT_NOTEOFF:
// Hack            if ( g_ListBuilder.HandleMidi(ev) )
// Hack            {
// Hack                // HandleMidi() only returns true in QUICK entry
// Hack                // mode, where midi input alone is used to manage
// Hack                // notelist updates.
// Hack
// Hack                g_PatternStore.UpdatePattern(g_ListBuilder.CurrentList());
// Hack                g_ListBuilder.Clear();
// Hack                set_status(STAT_POS_2, "");
// Hack                update_pattern_panel();
// Hack            }
// Hack            else /*if ( ev->type == SND_SEQ_EVENT_NOTEON )*/
// Hack            {
// Hack                show_listbuilder_status();
// Hack            }
            break;
            default:
                otherEvents += 1;
                break;
        }
#ifdef MA_BLUE
        // Todo MA_BLUE: How does event allocation work?
#else
        snd_seq_free_event(ev);
#endif
    }
    while ( g_Sequencer.EventInputPending() );
}
