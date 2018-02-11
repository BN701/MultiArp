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

#include "maSequencer.h"

#include <cstring>

Sequencer::Sequencer()
{
    //ctor
}

Sequencer::~Sequencer()
{
    //dtor
}

uint64_t AlsaTimeStampToMicros(snd_seq_timestamp & t)
{
    return static_cast<uint64_t>(t.time.tv_sec) * 1000000 + t.time.tv_nsec/1000;
}

uint64_t EventTimeToMicros(snd_seq_event_t & ev)
{
    return AlsaTimeStampToMicros(ev.time);
}

snd_seq_event_t * Sequencer::GetEvent(uint64_t t)
{
    if ( !m_EventQueue.empty() )
    {
        snd_seq_event_t * ev = & m_EventQueue.front();

        if ( EventTimeToMicros(*ev) <= t )
            return ev;
    }

    return NULL;
}


void Sequencer::PopEvent()
{
    m_EventQueue.pop_front();


}

void Sequencer::SetScheduleTime(uint64_t t)
{
    m_NextScheduleTimeMicros = t;
    m_NextScheduleTime.tv_sec = t / 1000000;
    m_NextScheduleTime.tv_nsec = (t % 1000000) * 1000;
}

bool Sequencer::ScheduleEvent(snd_seq_event_t & ev)
{
    uint64_t t = EventTimeToMicros(ev);

    if ( t == 0 )
    {
        t = m_NextScheduleTimeMicros;
        memcpy(&ev.time, &m_NextScheduleTime, sizeof(m_NextScheduleTime));
    }

    if ( m_EventQueue.empty() || t <= m_tQueueHead )
    {
        m_tQueueHead = t;
        m_EventQueue.emplace_front(ev);
    }
//    else if ( t >= m_tQueueTail )
//    {
//        m_tQueueTail = t;
//        m_EventQueue.emplace_after(m_EventQueue.end(), ev);
//    }
    else
    {
        // Find the place to insert the event.

        auto lastIt = m_EventQueue.begin();
        for ( auto it = m_EventQueue.begin(); it != m_EventQueue.end(); it++ )
        {
            if ( t < EventTimeToMicros(*it) )
                break;
            lastIt = it;
        }
        if ( lastIt != m_EventQueue.end() )
            m_EventQueue.emplace_after(lastIt, ev);
    }

    return true;
}

bool Sequencer::ScheduleNote(int queueId, uint8_t note, uint8_t vel, uint16_t len)
{
    snd_seq_event_t ev = {0};

    /// Schedule note-on.

    ev.type = SND_SEQ_EVENT_NOTEON;
    ev.data.note.channel = m_MidiChannel;
    ev.data.note.note = note;
    ev.data.note.velocity = vel;

    if ( !ScheduleEvent(ev) )
        return false;

    /// Schedule note-off.

    // Note length is already supplied in milliseconds. Event time
    // will have been filled in by the previous call to ScheduleEvent(),
    // so we update it in place. ScheduleEvent() will use the event's
    // time in preference to m_NextScheduleTime.

    uint64_t note_off_u = static_cast<uint64_t>(ev.time.time.tv_sec) * 1000000 + ev.time.time.tv_nsec/1000 + static_cast<uint64_t>(len) * 1000;
    ev.time.time.tv_sec = note_off_u / 1000000;
    ev.time.time.tv_nsec = (note_off_u % 1000000) * 1000;

    return ScheduleEvent(ev);
}

bool Sequencer::ScheduleNextCallBack(int queueId)
{
    snd_seq_event_t ev = {0};
    ev.type = SND_SEQ_EVENT_ECHO;
    return ScheduleEvent(ev);
}
