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
