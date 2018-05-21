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


#include "maAlsaSequencer.h"

//#if !defined(MA_BLUE)
AlsaSequencer g_Sequencer;
//#endif

AlsaSequencer::AlsaSequencer():
    m_MidiChannel(0)
{
    // We don't want the queueList reallocating space and calling
    // destructors, so reserve the space here and prevent the limit
    // being exceeded in CreateQueue().

    m_QueueList.reserve(MAX_QUEUES);

    if (snd_seq_open(&m_SeqHandle, "default", SND_SEQ_OPEN_DUPLEX, 0) < 0)
    {
        fprintf(stderr, "Error opening ALSA sequencer.\n");
        exit(1);
    }
    snd_seq_set_client_name(m_SeqHandle, "MultiArp");

    if ((m_PortOutId = snd_seq_create_simple_port(m_SeqHandle, "MultiArp-out",
                      SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ,
                      SND_SEQ_PORT_TYPE_APPLICATION)) < 0)
    {
        fprintf(stderr, "Error creating sequencer port.\n");
    }

    if ((m_PortInId = snd_seq_create_simple_port(m_SeqHandle, "MultiArp-in",
                     SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
                     SND_SEQ_PORT_TYPE_APPLICATION)) < 0)
    {
        fprintf(stderr, "Error creating sequencer port.\n");
        exit(1);
    }

}

AlsaSequencer::~AlsaSequencer()
{

}

int AlsaSequencer::CreateQueue()
{
    if ( m_QueueList.size() >= MAX_QUEUES )
        return -1;

    m_QueueList.emplace_back();
    m_QueueList.back().Create(m_SeqHandle, 0);
    return m_QueueList.size() - 1;
}

void AlsaSequencer::SetScheduleTime(uint64_t t)
{
    m_NextScheduleTime.tv_sec = t / 1000000;
    m_NextScheduleTime.tv_nsec = (t % 1000000) * 1000;
}

bool AlsaSequencer::ScheduleNote(unsigned char note, unsigned char vel, unsigned int len, uint8_t midiChannel)
{
    snd_seq_event_t ev;

    // Macro: fills event record with NULLS
    snd_seq_ev_clear(&ev);

    // Macro: fills in event record with note data, ch, key, vel, dur. Also sets event type to Note

    if ( midiChannel < 16 )
        snd_seq_ev_set_note(&ev, midiChannel, note, vel, len);
    else
        snd_seq_ev_set_note(&ev, m_MidiChannel, note, vel, len);

    // Macro: schedules the event in MIDI tick mode
    // snd_seq_ev_schedule_tick(&ev, queue_id,  0, tick);
    snd_seq_ev_schedule_real(&ev, m_QueueId,  0, &m_NextScheduleTime);

    // Macro: sets the source port id number on the event record
    snd_seq_ev_set_source(&ev, m_PortOutId);

    // Macro: sets the destination as the subscribers (and sets dest port to UNKNOWN)
    snd_seq_ev_set_subs(&ev);

    // Macro: send the event to the queue, maybe? There's no description for this call in
    // the header file, and the implementation is hidden.
    snd_seq_event_output_direct(m_SeqHandle, &ev);

    return true;

}


bool AlsaSequencer::ScheduleNextCallBack(int raw0, int raw1, int raw2)
{
    snd_seq_event_t ev;

    // Schedule an event to be fired back to our own app which prompts another
    // arpeggio to be placed in the queue.

    snd_seq_ev_clear(&ev);
    ev.type = SND_SEQ_EVENT_ECHO;
    ev.data.raw32.d[0] = raw0;
    ev.data.raw32.d[1] = raw1;
    ev.data.raw32.d[2] = raw2;
    snd_seq_ev_schedule_real(&ev, m_QueueId,  0, &m_NextScheduleTime);
    snd_seq_ev_set_dest(&ev, snd_seq_client_id(m_SeqHandle), m_PortInId);
    snd_seq_event_output_direct(m_SeqHandle, &ev);

    return true;
}

