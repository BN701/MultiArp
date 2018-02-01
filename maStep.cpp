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

#define LINK_PLATFORM_LINUX
#include <ableton/Link.hpp>

//#include <csignal>

#include "maAlsaSequencer.h"
//#include "maAlsaSequencerQueue.h"
//#include "maCommand.h"
//#include "maCursorKeys.h"
//#include "maFeelMap.h"
#include "maListBuilder.h"
#include "maPatternStore.h"
#include "maScreen.h"
#include "maState.h"
#include "maStep.h"
//#include "maUtility.h"

using namespace std;

// Global Link instance.

extern ableton::Link g_Link;
extern chrono::microseconds g_LinkStartTime;

// Global instances.

extern CursorKeys g_CursorKeys;
extern ListBuilder g_ListBuilder;
extern PatternStore g_PatternStore;
extern AlsaSequencer g_Sequencer;
extern State g_State;

extern Display g_Display;

int gDeferStop = 0;

void do_UI_updates()
{
    // If pattern changed last step ...

    if ( g_PatternStore.PatternChanged(true) )
    {
        set_top_line();
        update_edit_panels();
        set_status(STAT_POS_2, "Pattern changed ...");
    }

    // If phase zero last step ...

    if ( g_State.PhaseIsZero() )
    {
        update_pattern_status_panel();
    }

    // Every step ...

//    update_pattern_panel();     // TODO: Revert to updating this when updates are actually made.
//    highlight_pattern_panel();  // Moves note highlight.
    update_pattern_panel();
//    g_PatternStore.TranslateTableForPlay().Diags().ResetLog();

    update_progress_bar();
    set_status_w(STAT_POS_STEP, " Beat%9.2f\n (Sec%6i:%i)",
               /*g_State.Phase(),*/
//               g_State.Beat(),
               g_PatternStore.LastRealTimeBeat(),
               g_Sequencer.ScheduleTimeSeconds(),
               g_Sequencer.ScheduleTimeNanoSeconds() / 100000000);
}

void do_phase0_updates()
{
    g_PatternStore.TranslateTableForPlay().Diags().ResetLog();

    g_State.SetCurrentStepValue(g_PatternStore.StepValue());

    if ( g_State.NewQuantumPending() )
    {
        set_status(STAT_POS_2, "New quantum value set.");
    }

    if ( g_PatternStore.TranslateTableForPlay().NewTransposePending() )
    {
        set_status(STAT_POS_2, "Transpose set.");
    }

    if ( g_PatternStore.NewPatternPending() )
    {
        set_status(STAT_POS_2, "Pattern changed.");
    }

    if ( g_State.NewRunStatePending() )
    {
        if ( g_State.RunState() )
            g_PatternStore.ResetAllPatterns();
        gDeferStop = g_State.DeferStop();
    }

    if ( g_State.PatternReset() != RESET_NONE )
    {
        switch ( g_State.PatternReset() )
        {
            case RESET_ALL :
                g_PatternStore.ResetAllPatterns();
                set_status(STAT_POS_2, "All patterns were reset.");
                break;
            case RESET_CURRENT :
                g_PatternStore.ResetCurrentPattern();
                set_status(STAT_POS_2, "Current pattern was reset.");
                break;
        }
        g_State.SetPatternReset(RESET_NONE);
    }

    set_top_line();
}


void queue_next_step(int queueId)
{
    // We're called when ALSA has played the events we scheduled last time we were here,
    // so updating position info at this point should reflect what we are hearing.

    do_UI_updates();

    // Now incrememt the step/beat and get on with scheduling the next events.

    g_State.Step(g_PatternStore.StepValueMultiplier());

    // Get time of next step from Link.

    ableton::Link::Timeline timeline = g_Link.captureAppTimeline();

    double nextBeat = g_State.Beat();

    nextBeat = g_PatternStore.FeelMapForPlay().Adjust(nextBeat);
    chrono::microseconds t_next_usec = timeline.timeAtBeat(nextBeat, g_State.Quantum());

    g_State.SetPhase(timeline.phaseAtTime(t_next_usec, g_State.Quantum()));

    if ( g_State.PhaseIsZero() )
    {
        do_phase0_updates();
        g_PatternStore.SetPhaseIsZero();
        g_ListBuilder.SetPhaseIsZero(g_State.Beat(), g_State.Quantum());
    }

    // Set next schedule time on the queue

    int64_t queue_time_usec;
    if ( g_LinkStartTime.count() < 0 )
    {
        g_LinkStartTime = t_next_usec;
        queue_time_usec = 0;
    }
    else
    {
        queue_time_usec = (t_next_usec.count() - g_LinkStartTime.count());
    }

    if ( queue_time_usec < 0 )
    {
        // Sometimes at start up link appears to go backwards, especially if
        // there's another instance of the app running. For now, just keep
        // reschedule and hope things settle down.
//        raise(SIGINT);
        queue_time_usec = 0;
    }

    g_Sequencer.SetScheduleTime(queue_time_usec);

    // Schedule an event to be fired back to our own app which prompts another
    // arpeggio to be placed in the queue.

    // TODO: We used to do this after scheduling all midi events. Have there
    //       been any noticable effects of doing it first.

    g_Sequencer.ScheduleEcho(queueId);

    // Step the Pattern Store

    Cluster nextCluster;
    TrigRepeater repeater;
    TranslateTable & translator = g_PatternStore.TranslateTableForPlay();

    if ( g_State.RunState() || gDeferStop-- > 0 )
    {
        g_PatternStore.Step(nextCluster, repeater, g_State.Phase(), g_State.LastUsedStepValue(), nextBeat);
        if ( g_ListBuilder.RealTimeRecord() )
            nextCluster += *g_ListBuilder.Step(g_State.Phase(), g_State.LastUsedStepValue());
    }

    if ( nextCluster.Empty() )
        return;

    double tempo = timeline.tempo();

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
