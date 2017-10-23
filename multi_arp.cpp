//
//    MultiArp - Another step in the Great Midi Adventure!
//    Copyright (C) 2017  Barry Neilsen
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
//
//    ******************************************************************
//    * Apologies for the alarming mix of coding styles. I'm an ad-hoc *
//    * coder and I've been away from the game for several years.      *
//    * BN, Oct 2017                                                   *
//    ******************************************************************
//
//    Starting from miniArp.c by Matthias Nagorni, a great place to get
//    going with the ALSA Sequencer.
//


//#define _USE_MATH_DEFINES

#define LINK_PLATFORM_LINUX
#include <ableton/Link.hpp>
//#include <unordered_map>

#include "maAlsaSequencer.h"
#include "maAlsaSequencerQueue.h"
#include "maCommand.h"
#include "maCursorKeys.h"
#include "maFeelMap.h"
#include "maListBuilder.h"
#include "maPatternStore.h"
#include "maScreen.h"
#include "maState.h"
#include "maUtility.h"

using namespace std;

// Global Link instance.

ableton::Link g_Link(120.);
chrono::microseconds g_LinkStartTime(-1);

// Global instances.

AlsaSequencer g_Sequencer;
ListBuilder g_ListBuilder(g_Link);
PatternStore g_PatternStore;
State g_State;


extern Display gDisplay;

int gDeferStop = 0;

void do_phase0_updates()
{
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

    if ( g_PatternStore.PatternChanged(true) )
    {
        update_pattern_panel();
        set_top_line();
    }

    highlight_pattern_panel();  // Moves note highlight.

    set_status_w(STAT_POS_STEP, "Beat: %6.2f, phase: %4.2f, queue: %4i:%02i seconds",
               g_State.Beat(),
               g_State.Phase(),
               g_Sequencer.ScheduleTimeSeconds(),
               g_Sequencer.ScheduleTimeNanoSeconds() / 10000000);

    // There may have been a pattern change (especially if chaining is active).

    set_status_w(STAT_POS_PATTERN, g_PatternStore.PatternStatus().c_str());

    // Now incrememt the step/beat and get on with scheduling the next events.

    g_State.Step();

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

    g_Sequencer.SetScheduleTime(queue_time_usec);

    // Step the Pattern Store

    Chord * nextChord = NULL;

    if ( g_State.RunState() || gDeferStop-- > 0 )
    {
        // nextChord = g_PatternStore.Step();
        nextChord = g_ListBuilder.Step(g_State.Phase(), g_State.CurrentStepValue());
    }

    if ( nextChord != NULL )
    {
        double tempo = timeline.tempo();

        /*
              V, Step Value, is 4 x 'steps per beat'. (This gives the familiar
              eighth, sixteenths, etc). T, tempo, is 'beats per minute'.

              Steps per beat, v = V/4.
              Steps per minute = Tv = TV/4
              Steps per second = TV/240
              Step length in mSec = 1000*240/TV
         */

        double stepLengthMilliSecs = 240000.0/(tempo * g_State.CurrentStepValue());
        double duration = stepLengthMilliSecs * (nextChord->StepsTillNextNote() + g_PatternStore.GateLength());

        // TODO: Calculate duration for real time note capture, which is returned in beat values.

        for ( unsigned int i = 0; i < nextChord->m_Notes.size(); i++ )
        {
            Note & note = nextChord->m_Notes[i];

            int noteNumber = note.m_NoteNumber;

            if ( noteNumber < 0 )
                continue;

            noteNumber = g_PatternStore.TranslateTableForPlay().Translate(noteNumber);

            if ( noteNumber < 0 )
                continue;

            unsigned char noteVelocity;

            if ( note.m_NoteVelocity > 0 )
                noteVelocity = note.m_NoteVelocity;
            else
                noteVelocity = g_PatternStore.NoteVelocity();

            double noteLength = note.Length();
            if ( noteLength > 0 )
            {
                // Note duration here is in beats. Convert to milliseconds.
                // mSec/beat = 60000/tempo
                duration = 60000.0 * noteLength / tempo;
            }

            g_Sequencer.ScheduleNote(queueId, noteNumber, noteVelocity, duration);
        }
    }

    // Schedule an event to be fired back to our own app which prompts another
    // arpeggio to be placed in the queue.

    g_Sequencer.ScheduleEcho(queueId);

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
            if ( g_ListBuilder.HandleMidi(ev) )
            {
                // HandleMidi() only returns true in QUICK entry
                // mode, where midi input alone is used to manage
                // notelist updates.

                g_PatternStore.UpdatePattern(g_ListBuilder.CurrentList());
                g_ListBuilder.Clear();
                set_status(STAT_POS_2, "");
                update_pattern_panel();
            }
            else /*if ( ev->type == SND_SEQ_EVENT_NOTEON )*/
            {
                show_listbuilder_status();
            }
            break;
            default:
                otherEvents += 1;
                break;
        }
        snd_seq_free_event(ev);
    }
    while ( g_Sequencer.EventInputPending() );
}

enum global_element_names_t {
     global_heading,
     global_name_midi_channel,
     global_name_link_quantum,
     number_global_element_names
};


unordered_map<global_element_names_t, const char *> global_element_names = {
    {global_heading, "Global"},
    {global_name_midi_channel, "Midi Channel"},
    {global_name_link_quantum, "Link Quantum"}
};

string globals_to_string()
{
    string result = "<< ";
    result += global_element_names.at(global_heading);
    result += " >>\n\n";

    char buff[200];

    sprintf(buff, "%s %i\n", global_element_names.at(global_name_midi_channel), g_Sequencer.MidiChannel() + 1);
    result += buff;
    sprintf(buff, "%s %.2f\n", global_element_names.at(global_name_link_quantum), g_State.Quantum());
    result += buff;

    result += '\n';

    return result;
}


void load_from_string(string s, int & created, int & updated )
{
    created = 0;
    updated = 0;

    vector<string> errors;
    vector<string> rows = split(s.c_str(), '\n');

    // paste_target_t target = paste_pattern;
    // g_PatternStore.SetPasteTargetPatterns();

    for ( vector<string>::iterator i = rows.begin(); i != rows.end(); i++ )
    {
        try
        {
            if ( g_PatternStore.LoadFromString(*i, created, updated) ) // Give Pattern Store a chance to see the header.
                continue;

            // Handle global fields here (for the time being).

            for ( global_element_names_t e = static_cast<global_element_names_t>(1);
                  e < number_global_element_names;
                  e = static_cast<global_element_names_t>(static_cast<int>(e) + 1) )
            {
                string token = find_token(*i, global_element_names.at(e));

                if ( token.empty() )
                    continue;

                try
                {
                    switch (e)
                    {
                    case global_name_midi_channel:
                        g_Sequencer.SetMidiChannel(stoi(token) - 1);
                        break;
                    case global_name_link_quantum:
                        g_State.SetNewQuantumPending(stod(token));
                        break;
                    default:
                        break;
                    }
                }
                catch(invalid_argument ex)
                {

                }
                catch(out_of_range ex)
                {
                }
            }
        }
        catch (invalid_argument e)
        {
        }
        catch (string error )
        {
            errors.push_back(error);
        }
    }

    if ( !errors.empty() )
        throw string("Pattern parse error: At least one row contained errors.");

}

unordered_map<int, CursorKeys::key_type_t> g_CursorKeyMap =
{
    {KEY_DOWN, CursorKeys::down},
    {KEY_UP, CursorKeys::up},
    {KEY_LEFT, CursorKeys::left},
    {KEY_RIGHT, CursorKeys::right}
};


bool key_input_action()
{
    bool result = true;

    static string commandString;

    int c = getch();

    switch (c)
    {
    case 1: // Ctrl-A
        move(COMMAND_HOME);
        clrtoeol();
        refresh();
        copy_clipboard(globals_to_string() + g_PatternStore.ToString());
        set_status(STAT_POS_2, "All Data copied to clipboard ...");
        break;
        break;
    case 3:  // Ctrl-C, Copy
        move(COMMAND_HOME);
        clrtoeol();
        refresh();
        copy_clipboard(g_PatternStore.EditPatternToString());
        set_status(STAT_POS_2, "Edit Pattern copied to clipboard ...");
        break;

    case 22: // Ctrl-V, Paste
        move(COMMAND_HOME);
        clrtoeol();
        refresh();
        try
        {
            int created, updates;
            load_from_string(get_clipboard(), created, updates);
            set_status(STAT_POS_2, "Paste: %i updates, %i new patterns created.", updates, created);
        }
        catch (string errorMessage)
        {
            set_status(STAT_POS_2, "%s", errorMessage.c_str());
        }
        set_status_w(STAT_POS_PATTERN, g_PatternStore.PatternStatus().c_str());
        update_pattern_panel();
        break;

    case 10: // Enter (Ctl-J *and* Ctl-M will fire this one.)
        if ( !commandString.empty() )
        {
            result = do_command(commandString/*.c_str()*/);
            commandString.clear();
        }
        else if ( g_ListBuilder.HandleKeybInput(c) )
        {
            g_PatternStore.UpdatePattern(g_ListBuilder.CurrentList());
            g_ListBuilder.Clear();
            update_pattern_panel();
            set_status(STAT_POS_2, "");
        }
        move(COMMAND_HOME);
        clrtoeol();
        break;

    case 32: // Space bar.
        if ( commandString.empty() )
        {
            if ( g_ListBuilder.HandleKeybInput(c) )
                show_listbuilder_status();
            move(COMMAND_HOME);
        }
        else
            commandString += c;
        break;

    case KEY_SPGUP:
    case KEY_SPGDOWN:
    case KEY_APGUP:
    case KEY_APGDOWN:
    case KEY_CDOWN:
    case KEY_CUP:
    case KEY_CLEFT:
    case KEY_CRIGHT:
    case KEY_SDOWN:
    case KEY_SUP:
    case KEY_SLEFT:
    case KEY_SRIGHT:
    case KEY_CSLEFT:
    case KEY_CSRIGHT:
    case KEY_CSUP:
    case KEY_CSDOWN:
        break;

    case KEY_DOWN:
    case KEY_UP:
    case KEY_LEFT:
    case KEY_RIGHT:
        g_PatternStore.RouteKey(g_CursorKeyMap.at(c));
        show_status_after_navigation();
        break;

    case KEY_BACKSPACE: // 263
        if ( commandString.size() > 0 )
            commandString.pop_back();
        else if ( g_ListBuilder.HandleKeybInput(c) )
            show_listbuilder_status();
        move(COMMAND_HOME + commandString.size());
        clrtoeol(); // Assuming the cursor has been put back to correct location.
        break;

    default:
        if ( true )
        {
            set_status(STAT_POS_2, "Key: %i", c);
        }
        else if ( commandString.size() == 0 )
        {
            set_status(STAT_POS_2, "");
        }
        if ( c > 31 && c < 127 )
            commandString += c;
        break;
    }

    refresh();

    return result;
}

void sigterm_exit(int sig)
{

    // TODO: Do a proper cleanup of the Sequencer resources, i.e. queues?
    endwin();
    exit(0);
}


int main(int argc, char *argv[])
{
    g_PatternStore.SetFocus();
    do_command_line(argc, argv);

    // Initialize ...

    g_Link.enable(true); // Start peer-to-peer interactions.

    int queueIndex = g_Sequencer.CreateQueue();
    int queueId = g_Sequencer.Queue(queueIndex).GetQueueId();

    set_status_w(STAT_POS_PATTERN, g_PatternStore.PatternStatus().c_str());

    // Start the queue.

    g_Sequencer.Queue(queueIndex).Start();

    set_top_line();

    // Termination handlers.

    signal(SIGINT, sigterm_exit);
    signal(SIGTERM, sigterm_exit);

    /*
    * Set up polling ...
    *
    * Obtain number of descriptors
    * Allocate an appropriately sized array of struct pollfd (alloca puts them on the stack)
    * Fill in the array.
    *
    */

    int npfd = g_Sequencer.GetFileDescriptorCount();
    struct pollfd *pfd = (struct pollfd *)alloca((npfd + 1) * sizeof(struct pollfd));
    pfd[0].fd = 0;
    pfd[0].events = POLLIN;
    g_Sequencer.GetFileDescriptors(pfd + 1, npfd);

    // Queue first events

    queue_next_step(queueId);

    // Polling loop

    bool keep_going = true;

    while (keep_going)
    {
        if (poll(pfd, npfd + 1, 10000) > 0)
        {
            if (pfd[0].revents & POLLIN)
                keep_going = key_input_action();
            for (int i = 1; i < npfd + 1; i++)
            {
                if (pfd[i].revents > 0)
                    midi_action(queueId);
            }
        }
    }

}