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

#define LINK_PLATFORM_LINUX
#include <ableton/Link.hpp>

#include <alsa/asoundlib.h>

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
        ListBuilder(ableton::Link * linkInstance);
        virtual ~ListBuilder();

        bool HandleMidi(snd_seq_event_t *ev, double inBeat = 0);
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

        void SetMidiInputMode( int val );
        int MidiInputMode();
        bool RealTimeRecord() { return m_MidiInputMode == MIDI_INPUT_REAL_TIME; }
        int MidiInputModeAsColour(std::vector<int> choices) { return choices.at(m_MidiInputMode); }
        bool MidiInputModeChanged();    // Call once and clear flag.

        void SetPhaseIsZero(double beat, double quantum);

        Cluster * Step(double phase, double stepValue);

    protected:
        ableton::Link * m_Link = NULL;
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
