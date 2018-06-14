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
#include "maUtility.h"

#include <cstdio>
#include <cstring>

#if defined(MA_BLUE)
Sequencer g_Sequencer;
#endif

Sequencer::Sequencer()
{
    //ctor
}

Sequencer::~Sequencer()
{
    //dtor
}

snd_seq_event_t * Sequencer::GetEvent(uint64_t t)
{
    if ( m_EventQueue.empty() )
        return NULL;

    if ( EventTimeToMicros(m_EventQueue.front()) <= t )
        return & m_EventQueue.front();
    else
        return NULL;
}


void Sequencer::PopEvent()
{
    m_EventQueue.pop_front();
    if ( !m_EventQueue.empty() )
        m_tQueueHead = EventTimeToMicros(m_EventQueue.front());
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
        m_EventQueue.push_front(ev);
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
            m_EventQueue.insert_after(lastIt, ev);
    }

    return true;
}

bool Sequencer::ScheduleNote(uint8_t note, uint8_t vel, uint16_t len, uint8_t midiChannel)
{
    snd_seq_event_t ev = {0};

    /// Schedule note-on.

    ev.type = SND_SEQ_EVENT_NOTEON;
    if ( midiChannel < 16 )
        ev.data.note.channel = midiChannel;
    else
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
    ev.type = SND_SEQ_EVENT_NOTEOFF;
    ev.time.time.tv_sec = note_off_u / 1000000;
    ev.time.time.tv_nsec = (note_off_u % 1000000) * 1000;
    ev.data.note.velocity = 0;

    return ScheduleEvent(ev);
}

bool Sequencer::ScheduleNextCallBack(int raw0, int raw1, int raw2)
{
    snd_seq_event_t ev = {0};
    ev.type = SND_SEQ_EVENT_ECHO;
    ev.data.raw32.d[0] = raw0;
    ev.data.raw32.d[1] = raw1;
    ev.data.raw32.d[2] = raw2;
   return ScheduleEvent(ev);
}
