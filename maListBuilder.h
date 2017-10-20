//
//    MultiArp - Another step in the Great Midi Adventure!
//    Copyright (C) 2017  Barry Neilsen
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

#include <alsa/asoundlib.h>

#include "maPatternStore.h"

#define MIDI_INPUT_OFF      0
#define MIDI_INPUT_QUICK    1
#define MIDI_INPUT_FULL     2


class ListBuilder
{
    public:
        ListBuilder();
        virtual ~ListBuilder();

        bool HandleMidi(snd_seq_event_t *ev);
        bool HandleKeybInput(int c);

        NoteList & CurrentList() { return m_NoteList; }
        void Clear() { m_NoteList.Clear(); }

        std::string ToString();

        char MidiInputModeAsChar();

        void SetMidiInputMode( int val );
        int MidiInputMode();
        bool MidiInputModeChanged();    // Call once and clear flag.

    protected:
        int m_MidiInputMode;
        int m_openNotes;
        Chord m_Chord;
        NoteList m_NoteList;
        bool m_MidiInputModeChanged;

    private:
};

#endif // LISTBUILDER_H
