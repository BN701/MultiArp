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

#ifndef ALSASEQUENCER_H
#define ALSASEQUENCER_H

#include "maAlsaSequencerQueue.h"

struct AlsaSequencerQueue;

#include <cstdint>
#include <vector>


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

    bool ScheduleNote(int queueId, unsigned char note, unsigned char vel, unsigned int len);
    bool ScheduleNextCallBack(int queueId);

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
