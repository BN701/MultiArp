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

#ifndef TEENSYDUINO

#include "maAlsaSequencerQueue.h"

AlsaSequencerQueue::AlsaSequencerQueue():
    m_SeqHandle(NULL),
    m_QueueId(-1)
{
}

int AlsaSequencerQueue::Create(snd_seq_t * h, int pool_size)
{
    // We do this here rather than in the constructor because
    // these items are kept in a pre-allocated vector<> and maybe
    // copied and destroyed at times we can't control.

    m_SeqHandle = h;
    m_QueueId = snd_seq_alloc_queue(m_SeqHandle);

    /*
        From the original example notes by Matthias Nagorni:

        "This allocates a queue and sets its buffer size. The
        buffer size is passed as the number of events. While
        gaining experience with queues it might be helpful to
        have a look at /proc/asound/seq/queues."

        But what are the explicit rules for determining buffer
        size, and why then the "(seq_len << 1) + 4"? That's a
        lot of room for error.

        Plus how do we handle our case, where the number of
        notes varies from step to step. Set a max chord size
        and make sure we adhere to that when queueing each step?
     */

    // TODO: Clarify need for this bit. The program runs without it, though
    //       it may be running less efficiently.

    // snd_seq_set_client_pool_output(seq_handle, (seq_len<<1) + 4);

#if 0
    // Experiment to increase queue resolution.

    // According to this, https://www.alsa-project.org/alsa-doc/alsa-lib/seq.html, there
    // are actually two queues: one for events scheduled with ticks, the other for real time
    // events. Working with Link, we're scheduling real time events.
    //
    // I think changing the PPQ only affects events scheduled with ticks. Certainly,
    // reducing PPQ something very low has no effect on our own timing. (To check that
    // these calls are working, use "cat queues" in /proc/asound/seq/.)

    snd_seq_queue_tempo_t *tempo;
    snd_seq_queue_tempo_alloca(&tempo);
    snd_seq_queue_tempo_set_tempo(tempo, 1000000); // 60 BPM
    snd_seq_queue_tempo_set_ppq(tempo, 12); // 960 is what we get with Reaper midi export.
    snd_seq_set_queue_tempo(m_SeqHandle, m_QueueId, tempo);
#endif

    return m_QueueId;
}

AlsaSequencerQueue::~AlsaSequencerQueue()
{
    // TODO: Do we need to clear the queue?

    // clear_queue();
    // sleep(2);
    snd_seq_stop_queue(m_SeqHandle, m_QueueId, NULL);
    snd_seq_free_queue(m_SeqHandle, m_QueueId);
}

bool AlsaSequencerQueue::Start()
{
    snd_seq_start_queue(m_SeqHandle, m_QueueId, NULL);
    snd_seq_drain_output(m_SeqHandle); // I still don't know what this does (can't find any docs for it)

    // TODO: How do we check for success here?

    return true;
}


#endif // TEENSYDUINO
