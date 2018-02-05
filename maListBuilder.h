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

#ifndef LISTBUILDER_H
#define LISTBUILDER_H

#ifdef MA_BLUE

#include "alsa_types.h"
//// We don't have alsa, but lets see if we can use their types:
//typedef unsigned char snd_seq_event_type_t;
//
//typedef struct snd_seq_real_time {
//	unsigned int tv_sec;	/* seconds */
//	unsigned int tv_nsec;	/* nanoseconds */
//} snd_seq_real_time_t;
//
//typedef unsigned int snd_seq_tick_time_t;	/* midi ticks */
//
//union snd_seq_timestamp {
//	snd_seq_tick_time_t tick;
//	struct snd_seq_real_time time;
//};
//
//struct snd_seq_addr {
//	unsigned char client;	/**< Client number:         0..255, 255 = broadcast to all clients */
//	unsigned char port;	/**< Port within client:    0..255, 255 = broadcast to all ports */
//};
//
//struct snd_seq_ev_note {
//	unsigned char channel;
//	unsigned char note;
//	unsigned char velocity;
//	unsigned char off_velocity;	/* only for SNDRV_SEQ_EVENT_NOTE */
//	unsigned int duration;		/* only for SNDRV_SEQ_EVENT_NOTE */
//};
//
//typedef struct snd_seq_event {
//	snd_seq_event_type_t type;	/* event type */
//	unsigned char flags;		/* event flags */
//	char tag;
//
//	unsigned char queue;		/* schedule queue */
//	union snd_seq_timestamp time;	/* schedule time */
//
//
//	struct snd_seq_addr source;	/* source address */
//	struct snd_seq_addr dest;	/* destination address */
//
//	union {				/* event data... */
//		struct snd_seq_ev_note note;
////		struct snd_seq_ev_ctrl control;
////		struct snd_seq_ev_raw8 raw8;
////		struct snd_seq_ev_raw32 raw32;
////		struct snd_seq_ev_ext ext;
////		struct snd_seq_ev_queue_control queue;
////		union snd_seq_timestamp time;
////		struct snd_seq_addr addr;
////		struct snd_seq_connect connect;
////		struct snd_seq_result result;
////		struct snd_seq_ev_quote quote;
//	} data;
//} snd_seq_event_t;
//
//#define SND_SEQ_EVENT_NOTEON    6
//#define SND_SEQ_EVENT_NOTEOFF   7

#else
#define LINK_PLATFORM_LINUX
#include <ableton/Link.hpp>

#include <alsa/asoundlib.h>

#endif // MA_BLUE


#include "maPatternStore.h"

enum midi_input_modes_t
{
    MIDI_INPUT_OFF = 0,
    MIDI_INPUT_QUICK,
    MIDI_INPUT_STEP,
    MIDI_INPUT_REAL_TIME,
    MIDI_INPUT_FILE
};

class ListBuilder
{
    public:
        ListBuilder() {};
#ifndef MA_BLUE
        ListBuilder(ableton::Link * linkInstance);
#endif // MA_BLUE
        virtual ~ListBuilder();

        bool HandleKeybInput(int c);

        StepList & CurrentList() { return m_StepList; }
        std::map<double,Note> & RealTimeList() { return m_RealTimeList; }

        void Clear()
        {
            m_StepList.Clear();
            m_Captured.Clear();
            m_OpenNotes.clear();
            m_RealTimeList.clear();
            m_RealTimeUndoIndex.clear();
        }

        std::string ToString();

        char MidiInputModeAsChar();

        bool HandleMidi(snd_seq_event_t *ev, double inBeat = 0);

        void SetMidiInputMode( int val );
        int MidiInputMode();
        bool RealTimeRecord() { return m_MidiInputMode == MIDI_INPUT_REAL_TIME; }
        int MidiInputModeAsColour(std::vector<int> choices) { return choices.at(m_MidiInputMode); }
        bool MidiInputModeChanged();    // Call once and clear flag.

        void SetPhaseIsZero(double beat, double quantum);

        Cluster * Step(double phase, double stepValue);

    protected:
#ifndef MA_BLUE
        ableton::Link * m_Link = NULL;
#endif // MA_BLUE
        int m_MidiInputMode = MIDI_INPUT_OFF;
        int m_openNotes = 0;
        Note m_Activity;   // Something to show what's coming in.
        Cluster m_Captured;
        StepList m_StepList;
        bool m_MidiInputModeChanged;
        double m_LinkQuantum;
        double m_BeatAtLastPhaseZero;

        std::map<unsigned char, Note> m_OpenNotes;
        std::map<double, Note> m_RealTimeList;
        std::vector<std::map<double, Note>::iterator> m_RealTimeUndoIndex;

    private:
};

#endif // LISTBUILDER_H
