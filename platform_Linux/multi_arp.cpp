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


//#define _USE_MATH_DEFINES

#define LINK_PLATFORM_LINUX
#include <ableton/Link.hpp>


#include "maAlsaSequencer.h"
#include "maCommand.h"
#include "maListBuilder.h"
#include "maPatternStore.h"
#include "maScreen.h"
#include "maStep.h"

// Global Link instance.

ableton::Link g_Link(120.);
std::chrono::microseconds g_LinkStartTime(-1);

// Global instances.

AlsaSequencer g_Sequencer;
ListBuilder g_ListBuilder(&g_Link);
PatternStore g_PatternStore;

extern TextUI g_TextUI;
extern CairoUI g_CairoUI;


void sigterm_exit(int sig)
{
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

    update_pattern_status_panel();

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
            CursorKeys::key_type_t curKey = CursorKeys::no_key;
            xcb_keysym_t sym = 0;

            if ( pfd[0].revents & POLLIN )
            {
                keyDataValid = true;
                g_TextUI.KeyInput(curKey, sym);
            }
            else if ( gotXWindow && (pfd[1].revents & POLLIN) )
            {
                keep_going = g_CairoUI.PollEvents(keyDataValid, curKey, sym);
            }

            if ( keep_going && keyDataValid )
            {
                keep_going = handle_key_input(curKey, sym);
            }

            for ( int i = 2; i < npfd + 2; i++ )
            {
                if ( pfd[i].revents > 0 )
                    midi_action(queueId);
            }
        }
    }

}
