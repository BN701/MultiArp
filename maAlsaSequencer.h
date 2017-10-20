#ifndef ALSASEQUENCER_H
#define ALSASEQUENCER_H

#include "maAlsaSequencerQueue.h"

struct AlsaSequencerQueue;

#include <cstdint>
#include <vector>
//#include <alsa/asoundlib.h>


#define MAX_QUEUES 10

class AlsaSequencer
{
public:
    /** Default constructor */
    AlsaSequencer();
    /** Default destructor */
    ~AlsaSequencer();

    void SetScheduleTime(uint64_t t); // Use a one off call to set time to be used by all subsequent calls to schedule events.

    unsigned int ScheduleTimeSeconds()
    {
        return m_NextScheduleTime.tv_sec;
    }

    unsigned int ScheduleTimeNanoSeconds()
    {
        return m_NextScheduleTime.tv_nsec;
    }

    bool ScheduleNote(int queueId, int note, int vel, int len);
    bool ScheduleEcho(int queueId);

    snd_seq_t * Handle()
    {
        return m_SeqHandle;
    }

    bool EventInputPending()
    {
        return snd_seq_event_input_pending(m_SeqHandle, 0) > 0;
    }

    void EventInput(snd_seq_event_t **ev)
    {
        snd_seq_event_input(m_SeqHandle, ev);
    }

    int CreateQueue();

    AlsaSequencerQueue & Queue(int queueIndex)
    {
        return m_QueueList.at(queueIndex);
    }

    int GetFileDescriptorCount()
    {
        return snd_seq_poll_descriptors_count(m_SeqHandle, POLLIN);
    }

    bool GetFileDescriptors(struct pollfd * pfd, int npfd)
    {
        snd_seq_poll_descriptors(m_SeqHandle, pfd, npfd, POLLIN);
        return true; // TODO: How to check for success. (Or should we just be throwing exceptions?)
    }

    unsigned char MidiChannel()
    {
        return m_MidiChannel;
    }

    void SetMidiChannel( unsigned char c )
    {
        m_MidiChannel = c;
    }

    void SetClientName(const char * name)
    {
        snd_seq_set_client_name(m_SeqHandle, name);
    }

protected:

    snd_seq_t *m_SeqHandle;

    std::vector<AlsaSequencerQueue> m_QueueList;

    snd_seq_real_time_t m_NextScheduleTime;

    unsigned char m_MidiChannel;
    int m_PortInId;
    int m_PortOutId;

private:
};

#endif // ALSASEQUENCER_H
