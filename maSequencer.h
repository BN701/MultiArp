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

#ifndef MASEQUENCER_H
#define MASEQUENCER_H

#include <cstdint>
#include <chrono>
#include <forward_list>

#if defined(MA_BLUE) && !defined(MA_BLUE_PC)
#include "alsa_types.h"
#else
#include <alsa/asoundlib.h>
#endif

class Sequencer
{
    public:
        Sequencer();
        virtual ~Sequencer();

        void SetScheduleTime(uint64_t t); // Use a one off call to set time to be used by all subsequent calls to schedule events.

        unsigned int ScheduleTimeSeconds()
        {
            return m_NextScheduleTime.tv_sec;
        }

        unsigned int ScheduleTimeNanoSeconds()
        {
            return m_NextScheduleTime.tv_nsec;
        }


        bool ScheduleNote(int queueId, uint8_t note, uint8_t vel, uint16_t len);
        bool ScheduleNextCallBack(int queueId);

        snd_seq_event_t * GetEvent(uint64_t);
        void PopEvent();

//
//        snd_seq_t * Handle()
//        {
//            return m_SeqHandle;
//        }
//
        bool EventInputPending()
        {
//            return snd_seq_event_input_pending(m_SeqHandle, 0) > 0;
            return false;
        }

        void EventInput(snd_seq_event_t **ev)
        {
//            snd_seq_event_input(m_SeqHandle, ev);
        }

//        int CreateQueue();
//
//        AlsaSequencerQueue & Queue(int queueIndex)
//        {
//            return m_QueueList.at(queueIndex);
//        }
//
//        int GetFileDescriptorCount()
//        {
//            return snd_seq_poll_descriptors_count(m_SeqHandle, POLLIN);
//        }
//
//        bool GetFileDescriptors(struct pollfd * pfd, int npfd)
//        {
//            snd_seq_poll_descriptors(m_SeqHandle, pfd, npfd, POLLIN);
//            return true; // TODO: How to check for success. (Or should we just be throwing exceptions?)
//        }

        unsigned char MidiChannel()
        {
            return m_MidiChannel;
        }

        void SetMidiChannel( uint8_t c )
        {
            m_MidiChannel = c;
        }

//        void SetClientName(const char * name)
//        {
//            snd_seq_set_client_name(m_SeqHandle, name);
//        }

        std::forward_list<snd_seq_event_t> & EventQueue() { return m_EventQueue; }
    private:

//        snd_seq_t *m_SeqHandle;

//        std::vector<AlsaSequencerQueue> m_QueueList;

        uint64_t m_NextScheduleTimeMicros = 0;
        snd_seq_real_time_t m_NextScheduleTime;

        uint8_t m_MidiChannel;
//        int m_PortInId;
//        int m_PortOutId;

        bool ScheduleEvent(snd_seq_event_t & ev);

        std::forward_list<snd_seq_event_t> m_EventQueue;
        uint64_t m_tQueueHead = static_cast<uint64_t>(-1);
        uint64_t m_tQueueTail = 0;
//        bool m_InsertFirst;
//        bool m_InsertLast = true;
};

#endif // MASEQUENCER_H
