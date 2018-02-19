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


#include "maCommand.h"
#include "maListBuilder.h"
#include "maPatternStore.h"
#include "maScreen.h"
#include "maState.h"
#include "maStep.h"

//
// Use of MA_BLUE in this file is to create a PC platform
// that can be used to test and debug MA_BLUE configurations
// away from the embedded platform. This includes things
// like no-exception handling, alternative UI elements.
//
// This file is never run on the embedded target itself.
//
/////////////////////////////////////////////////////////////

#if defined(MA_BLUE)

#include <chrono>
#include <poll.h>
#include <thread>

#include "maSequencer.h"
Sequencer g_Sequencer;
ListBuilder g_ListBuilder;

#if defined(MA_BLUE_PC)
// Direct midi IO with ALSA ...
// (This should be independent of
// and ALSA sequencer activity
// in the main PC build.)

#include "maAlsaTestSupport.h"

#endif

#else

#define LINK_PLATFORM_LINUX
#include <ableton/Link.hpp>

#include "maAlsaSequencer.h"

// Global Link instance.

ableton::Link g_Link(120.);
std::chrono::microseconds g_LinkStartTime(-1);

AlsaSequencer g_Sequencer;
ListBuilder g_ListBuilder(&g_Link);

#endif

using namespace std;

// Global instances.

PatternStore g_PatternStore;

AnsiUI g_TextUI;
#ifndef MA_BLUE
CairoUI g_CairoUI(true);
#endif

State g_State;
//PatternStore g_PatternStore;

// Static instances of empty items
// for use by member functions that
// return references when no patterns
// or other items have been created.
//
// (And having them here
// seems to ensure that they're
// initialized after the other static
// data they depend on.)

StepList StepList::EmptyList;
RealTimeList RealTimeList::EmptyList;
Pattern Pattern::EmptyPattern;

void sigterm_exit(int sig)
{
    exit(0);
}

int main(int argc, char *argv[])
{
    // Initialize ...

    // Termination handlers.

//    signal(SIGINT, sigterm_exit);
//    signal(SIGINT, SIG_IGN);

#if !defined(MA_BLUE)// || defined(MA_BLUE_PC)
    signal(SIGTERM, sigterm_exit);
#endif

   g_PatternStore.SetFocus();

#if defined(MA_BLUE)

#if defined(MA_BLUE_PC)
    init_alsa_test_support();
#endif

    set_top_line();

    // Poll for keyboard input to start with.

    int npfd = alsa_midi_test_support_GetFileDescriptorCount();
    struct pollfd *pfd = (struct pollfd *)alloca((npfd + 1) * sizeof(struct pollfd));
    pfd[0].fd = 0;  // stdin
    pfd[0].events = POLLIN;
    alsa_midi_test_support_GetFileDescriptors(pfd + 1, npfd);

    // Queue first events

    queue_next_step(0);

    // Polling loop

    auto start = chrono::high_resolution_clock::now();

    bool keep_going = true;

    int loopCount = 0;
    while ( keep_going )
    {
        loopCount++;

        auto elapsed = chrono::high_resolution_clock::now() - start;
        uint64_t microseconds = chrono::duration_cast<chrono::microseconds>(elapsed).count();

        bool callStep = false;
//        bool gotEvent = false;
        while ( snd_seq_event_t * ev = g_Sequencer.GetEvent(microseconds) )
        {
//            gotEvent = true;
            switch (ev->type)
            {
                case SND_SEQ_EVENT_ECHO:
                    // This is our 'tick', so schedule everything
                    // that should happen next, including the
                    // next tick.
//                    queue_next_step(0);
//                    fprintf(stderr, "%12i - Tick, in loop, deferring ...\n", loopCount);
                    callStep = true;
                    break;
                case SND_SEQ_EVENT_NOTEON:
//                    fprintf(stderr, "%12i - Note on.\n", loopCount);
                    alsa_midi_write_event(ev);
                    break;
                case SND_SEQ_EVENT_NOTEOFF:
//                    fprintf(stderr, "%12i - Note off.\n", loopCount);
                    alsa_midi_write_event(ev);
                    break;
            }
            g_Sequencer.PopEvent();
        }

        if ( callStep )
        {
            queue_next_step(0);
#if 0
            fprintf(stderr, "%12i - Tick, deferred, pre-step.\n", loopCount);
            for ( auto it = g_Sequencer.EventQueue().begin(); it != g_Sequencer.EventQueue().end(); it++ )
            {
                const char *evName;
                switch (it->type)
                {
                    case SND_SEQ_EVENT_NOTEON:
                        evName = "Note On";
                        break;
                    case SND_SEQ_EVENT_NOTEOFF:
                        evName = "Note Off";
                        break;
                    case SND_SEQ_EVENT_ECHO:
                        evName = "Echo";
                        break;
                }
                fprintf(stderr, "\t\tQueue: %s %i:%i\n", evName, it->time.time.tv_sec, it->time.time.tv_nsec);
            }
            fprintf(stderr, "%12i - Tick, deferred, post-step.\n", loopCount);
#endif

        }

//        if ( gotEvent )
//            fprintf(stderr, "\n");

        if ( poll(pfd, 2, 1) > 0 )  // Short time out, 1 msec, still longer than MCU loop
        {
            bool keyDataValid = false;
            BaseUI::key_command_t key;

            if ( pfd[0].revents & POLLIN )
            {
                keyDataValid = true;
                key = g_TextUI.KeyInput();
            }
//            else if ( gotXWindow && (pfd[1].revents & POLLIN) )
//            {
//                keep_going = g_CairoUI.PollEvents(keyDataValid, key);
//            }

            if ( keep_going && keyDataValid )
            {
                keep_going = handle_key_input(key);
            }

            for ( int i = 1; i < npfd + 1; i++ )
            {
                if ( pfd[i].revents > 0 )
                {
                    do
                    {
                        snd_seq_event_t *ev;
                        alsa_midi_read_input(& ev);
                        ev->time.time.tv_sec = microseconds/1000000;
                        ev->time.time.tv_nsec = (microseconds % 1000000) * 1000;
                        handle_midi_event(ev);
                        alsa_midi_free_event(ev);
                    }
                    while ( alsa_midi_input_pending() );
                }
            }
        }

        // Pop this in here so we don't drive the CPU hot by fast looping.
//        std::this_thread::sleep_for(chrono::milliseconds(1));
    }

#else

    do_command_line(argc, argv);

    g_Link.enable(true); // Start peer-to-peer interactions.


    int queueIndex = g_Sequencer.CreateQueue();
    int queueId = g_Sequencer.Queue(queueIndex).GetQueueId();

    update_pattern_status_panel();

    // Start the queue.

    g_Sequencer.Queue(queueIndex).Start();

    set_top_line();

    /*
    * Set up polling ...
    *
    * Obtain number of descriptors
    * Allocate an appropriately sized array of struct pollfd (alloca puts them on the stack)
    * Fill in the array.
    *
    */

    int npfd = g_Sequencer.GetFileDescriptorCount();
    struct pollfd *pfd = (struct pollfd *)alloca((npfd + 2) * sizeof(struct pollfd));
    pfd[0].fd = 0;  // stdin
    pfd[0].events = POLLIN;
    pfd[1].fd = g_CairoUI.GetFileDescriptor();
    pfd[1].events = POLLIN;
    g_Sequencer.GetFileDescriptors(pfd + 2, npfd);

    bool gotXWindow = pfd[1].fd != 0;

    // Queue first events

    queue_next_step(queueId);

    // Polling loop

    bool keep_going = true;

    while ( keep_going )
    {
        if ( poll(pfd, npfd + 2, 10000) > 0 )
        {
            bool keyDataValid = false;
            BaseUI::key_command_t key;
            // CursorKeys::key_type_t curKey = CursorKeys::no_key;
            // xcb_keysym_t sym = 0;

            if ( pfd[0].revents & POLLIN )
            {
                keyDataValid = true;
                key = g_TextUI.KeyInput();
            }
            else if ( gotXWindow && (pfd[1].revents & POLLIN) )
            {
                keep_going = g_CairoUI.PollEvents(keyDataValid, key);
            }

            if ( keep_going && keyDataValid )
            {
                keep_going = handle_key_input(key);
            }

            for ( int i = 2; i < npfd + 2; i++ )
            {
                if ( pfd[i].revents > 0 )
                    read_midi_ALSA(queueId);
            }
        }
    }
#endif

}