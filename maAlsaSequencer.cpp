#include "maAlsaSequencer.h"

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

bool AlsaSequencer::ScheduleNote(int queueId, int note, int vel, int len)
{

    snd_seq_event_t ev;


    // Macro: fills event record with NULLS
    snd_seq_ev_clear(&ev);

    // Macro: fills in event record with note data, ch, key, vel, dur. Also sets event type to Note
    snd_seq_ev_set_note(&ev, m_MidiChannel, note, vel, len);

    // Macro: schedules the event in MIDI tick mode
    // snd_seq_ev_schedule_tick(&ev, queue_id,  0, tick);
    snd_seq_ev_schedule_real(&ev, queueId,  0, &m_NextScheduleTime);

    // Macro: sets the source port id number on the event record
    snd_seq_ev_set_source(&ev, m_PortOutId);

    // Macro: sets the destination as the subscribers (and sets dest port to UNKNOWN)
    snd_seq_ev_set_subs(&ev);

    // Macro: send the event to the queue, maybe? There's no description for this call in
    // the header file, and the implementation is hidden.
    snd_seq_event_output_direct(m_SeqHandle, &ev);

    return true;

}


bool AlsaSequencer::ScheduleEcho(int queueId)
{
    snd_seq_event_t ev;

    // Schedule an event to be fired back to our own app which prompts another
    // arpeggio to be placed in the queue.

    snd_seq_ev_clear(&ev);
    ev.type = SND_SEQ_EVENT_ECHO;
    snd_seq_ev_schedule_real(&ev, queueId,  0, &m_NextScheduleTime);
    snd_seq_ev_set_dest(&ev, snd_seq_client_id(m_SeqHandle), m_PortInId);
    snd_seq_event_output_direct(m_SeqHandle, &ev);

    return true;
}
