#include "maListBuilder.h"

#include <alsa/asoundlib.h>

// From curses.h

#define KEY_BACKSPACE	0407		/* backspace key */

ListBuilder::ListBuilder():
    m_MidiInputMode(MIDI_INPUT_OFF),
    m_openNotes(0)
{
    //ctor
}

ListBuilder::~ListBuilder()
{
    //dtor
}

void ListBuilder::SetMidiInputMode( int val )
{
    if ( m_MidiInputMode != val )
    {
        m_MidiInputMode = val;
        m_MidiInputModeChanged = true;

        if ( m_MidiInputMode == MIDI_INPUT_OFF )
        {
            m_NoteList.Clear();
            m_Chord.Clear();
        }
    }
}

int ListBuilder::MidiInputMode()
{
    return m_MidiInputMode;
}

bool ListBuilder::MidiInputModeChanged()
{
    if ( m_MidiInputModeChanged )
    {
        m_MidiInputModeChanged = false;
        return true;
    }
    else
        return false;
}    // Call once and clear flag.

char ListBuilder::MidiInputModeAsChar()
{
    switch ( m_MidiInputMode )
    {
        case MIDI_INPUT_FULL:
            return 'F';

        case MIDI_INPUT_QUICK:
            return 'Q';

        default:
            return ' ';

    }
}

std::string ListBuilder::ToString()
{
    switch ( m_MidiInputMode )
    {
        case MIDI_INPUT_FULL:
            if ( m_NoteList.Empty() )
                return m_Chord.ToString();
            else if ( m_Chord.Empty() )
                return m_NoteList.ToString();
            else
                return m_NoteList.ToString() + "," + m_Chord.ToString();

        case MIDI_INPUT_QUICK:
            return m_NoteList.ToString();

        default:
            return "";
    }
}

bool ListBuilder::HandleMidi(snd_seq_event_t *ev)
{

    switch ( m_MidiInputMode )
    {
        case MIDI_INPUT_FULL:

            // Build the current chord and addto the note list when all keys
            // are released. Keep adding chords - which can be single notes,
            // of course - until something else, probably a keypress, uses and
            // clears the notelist.

            if ( ev->type == SND_SEQ_EVENT_NOTEON )
            {
                m_Chord.Add(ev->data.note.note, ev->data.note.velocity);
                m_openNotes += 1;
            }
            else if ( m_openNotes > 0 )
            {
                m_openNotes -= 1;
            }
            if ( m_openNotes == 0 )
            {
                m_NoteList.Add(m_Chord);
                m_Chord.Clear();
            }
            return false;

        case MIDI_INPUT_QUICK:
            if ( ev->type == SND_SEQ_EVENT_NOTEON )
            {
                m_NoteList.Add(ev->data.note.note, ev->data.note.velocity);
                m_openNotes += 1;
            }
            else if ( m_openNotes > 0 )
            {
                m_openNotes -= 1;
            }
            return m_openNotes == 0;

        default:

            return false;
    }

}


bool ListBuilder::HandleKeybInput(int c)
{
    switch (c)
    {
        case 10:
            return m_MidiInputMode == MIDI_INPUT_FULL && !m_NoteList.Empty();

        case 32:
            m_NoteList.Add();
            return true;

        case KEY_BACKSPACE:
            m_NoteList.DeleteLast();
            return true;

        default:
            return false;
    }
}
