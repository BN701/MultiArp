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


#include "maListBuilder.h"


#include <alsa/asoundlib.h>
#include <fstream>
#include <iostream>

using namespace std;

// From curses.h

#define KEY_BACKSPACE	0407		/* backspace key */

#define LOG_ON 1

#if LOG_ON
ofstream fLog;
#endif

ListBuilder::ListBuilder(ableton::Link * linkInstance):
    m_Link(linkInstance)
{
    //ctor
#if LOG_ON
    fLog.open("ListBuilder.log");
#endif

}

ListBuilder::~ListBuilder()
{
    //dtor
#if LOG_ON
    fLog.close();
#endif

}

void ListBuilder::SetMidiInputMode( int val )
{
    if ( m_MidiInputMode != val )
    {
        m_MidiInputMode = val;
        m_MidiInputModeChanged = true;

        if ( m_MidiInputMode == MIDI_INPUT_OFF )
        {
            m_StepList.Clear();
            m_Captured.Clear();
        }
    }
}

int ListBuilder::MidiInputMode()
{
    return m_MidiInputMode;
}

bool ListBuilder::MidiInputModeChanged()
{
    // Call once and clear flag.

    if ( m_MidiInputModeChanged )
    {
        m_MidiInputModeChanged = false;
        return true;
    }
    else
        return false;
}



char ListBuilder::MidiInputModeAsChar()
{
    switch ( m_MidiInputMode )
    {
        case MIDI_INPUT_REAL_TIME:
            return 'R';

        case MIDI_INPUT_STEP:
            return 'S';

        case MIDI_INPUT_QUICK:
            return 'Q';

        default:
            return 'X';

    }
}

std::string ListBuilder::ToString()
{
    switch ( m_MidiInputMode )
    {
        case MIDI_INPUT_REAL_TIME:
            {
                char buff[100];
                sprintf(buff, " Open: %lu, captured: %lu", m_OpenNotes.size(), m_RealTimeList.size());
                return buff;
            }
            break;

        case MIDI_INPUT_STEP:
            if ( m_StepList.Empty() )
                return m_Captured.ToString();
            else if ( m_Captured.Empty() )
                return m_StepList.ToString(false);
            else
                return m_StepList.ToString(false) + "," + m_Captured.ToString();

        case MIDI_INPUT_QUICK:
            return m_StepList.ToString(false);

        default:
            return m_Activity.ToString(false);
    }
}


bool ListBuilder::HandleKeybInput(int c)
{
    switch (c)
    {
        case 10:
            switch ( m_MidiInputMode )
            {
            case MIDI_INPUT_REAL_TIME:
                return !m_RealTimeList.empty();

            case MIDI_INPUT_STEP:
                return !m_StepList.Empty();

            default:
                return false;
            }

        case 32:
            m_StepList.Add();
            return true;

        case KEY_BACKSPACE:
            switch ( m_MidiInputMode )
            {
            case MIDI_INPUT_REAL_TIME:
                if ( !m_RealTimeUndoIndex.empty() )
                {
                    m_RealTimeList.erase(m_RealTimeUndoIndex.back());
                    m_RealTimeUndoIndex.pop_back();
                    return true;
                }
                else
                    return false;
            case MIDI_INPUT_STEP:
                return m_StepList.DeleteLast();
            default:
                return false;
            }

        default:
            return false;
    }
}

void ListBuilder::SetPhaseIsZero(double beat, double quantum)
{
    m_BeatAtLastPhaseZero = beat;
    m_LinkQuantum = quantum;
}

bool ListBuilder::HandleMidi(snd_seq_event_t *ev, double inBeat)
{

    switch ( m_MidiInputMode )
    {
        case MIDI_INPUT_REAL_TIME:
            {
                if ( m_Link == NULL )
                    throw string("ListBuilder::HandleMidi() - Expecting Ableton Link Instance to be set.");
                chrono::microseconds t_now = m_Link->clock().micros();
                ableton::Link::Timeline timeline = m_Link->captureAppTimeline();
                double beat = timeline.beatAtTime(t_now, m_LinkQuantum);

                // Create notes for note-on, complete notes and calculate
                // duration for note off.
#if LOG_ON
                char buff[100];
                sprintf(buff, "Beat %6.2f", beat);
                fLog << buff;
#endif

                if ( ev->type == SND_SEQ_EVENT_NOTEON )
                {
                    Note note(ev->data.note.note, ev->data.note.velocity);
                    note.SetPhase(beat);    // Temporarily store beat in the notes Phase member.

                    map<unsigned char,Note>::iterator it = m_OpenNotes.find(ev->data.note.note);
                    if ( it != m_OpenNotes.end() )
                        m_OpenNotes.at(ev->data.note.note) = note;
                    else
                        m_OpenNotes.insert(make_pair(ev->data.note.note, note));
#if LOG_ON
                    sprintf(buff, " opening %3i\n", ev->data.note.note);
                    fLog << buff;
#endif
                }
                else
                {
                    map<unsigned char,Note>::iterator it = m_OpenNotes.find(ev->data.note.note);
                    if ( it != m_OpenNotes.end() )
                    {
                        Note note = m_OpenNotes.at(ev->data.note.note);
                        m_OpenNotes.erase(it);

                        double beatStart = note.Phase();
                        note.SetLength(beat - beatStart);

                        // Normalize beat Start

                        while ( beatStart < m_BeatAtLastPhaseZero )
                            beatStart += m_LinkQuantum;

                        note.SetPhase(beatStart - m_BeatAtLastPhaseZero);

                        // Syntax here is getting ridiculous! Here's what's going on:
                        //
                        // m_RealTimeList is a map of Note objects, keyed on their beat value.
                        // You have to make a std::pair to add items to the map.
                        // The insert() operation returns another pair, the first element of
                        // which is an iterator pointing to the new item in the map.
                        // We put the iterator on the undo stack, m_RealTimeUndoIndex.
                        //
                        // (TODO: We might need to handle clashes, as I guess it's not
                        // impossible for two events to have the same beat value.)

                        m_RealTimeUndoIndex.push_back(m_RealTimeList.insert(make_pair(note.Phase(), note)).first);

#if LOG_ON
                        sprintf(buff, " closing %3i, phase %6.2f length %6.2f\n",
                                ev->data.note.note,
                                note.Phase(),
                                note.Length());

                        fLog << buff;
#endif
                    }

                }

            }
            return false;

        case MIDI_INPUT_FILE:
            {
                // Similar to real time capture, except we don't get time from link
                // and we don't try to normalize the beat as that's provided as input.

                if ( ev->type == SND_SEQ_EVENT_NOTEON )
                {
                    Note note(ev->data.note.note, ev->data.note.velocity);
                    note.SetPhase(inBeat);    // Temporarily store beat in the notes Phase member.

                    map<unsigned char,Note>::iterator it = m_OpenNotes.find(ev->data.note.note);
                    if ( it != m_OpenNotes.end() )
                        m_OpenNotes.at(ev->data.note.note) = note;
                    else
                        m_OpenNotes.insert(make_pair(ev->data.note.note, note));
                }
                else
                {
                    map<unsigned char,Note>::iterator it = m_OpenNotes.find(ev->data.note.note);
                    if ( it != m_OpenNotes.end() )
                    {
                        Note note = m_OpenNotes.at(ev->data.note.note);
                        m_OpenNotes.erase(it);

                        double beatStart = note.Phase();
                        note.SetLength(inBeat - beatStart);

                        m_RealTimeList.insert(make_pair(note.Phase(), note));
                    }

                }

            }
            return false;

        case MIDI_INPUT_STEP:

            // Build the current chord and addto the note list when all keys
            // are released. Keep adding chords - which can be single notes,
            // of course - until something else, probably a keypress, uses and
            // clears the notelist.

            if ( ev->type == SND_SEQ_EVENT_NOTEON )
            {
                m_Captured.Add(ev->data.note.note, ev->data.note.velocity);
                m_openNotes += 1;
            }
            else if ( m_openNotes > 0 )
            {
                m_openNotes -= 1;
            }
            if ( m_openNotes == 0 )
            {
                m_StepList.Add(m_Captured);
                m_Captured.Clear();
            }
            return false;

        case MIDI_INPUT_QUICK:
            if ( ev->type == SND_SEQ_EVENT_NOTEON )
            {
                m_StepList.Add(ev->data.note.note, ev->data.note.velocity);
                m_openNotes += 1;
            }
            else if ( m_openNotes > 0 )
            {
                m_openNotes -= 1;
            }
            return m_openNotes == 0; // Tell calling function we have a complete notelist.

        default:
            m_Activity.m_NoteNumber = ev->data.note.note;
            return false;
    }

}


Cluster * ListBuilder::Step(double phase, double stepValue)
{
    m_Captured.Clear();

    double windowStart = phase - 2 / stepValue;
    double windowEnd = phase + 2 / stepValue;

    for ( map<double,Note>::iterator it = m_RealTimeList.lower_bound(windowStart);
                    it != m_RealTimeList.upper_bound(windowEnd); it++ )
        m_Captured.Add(it->second);

    // When phase is zero, window start will be negative, so we also need to
    // look for notes at the top of the loop that would normally be quantized
    // up to next beat zero.

    if ( windowStart < 0 )
    {
        windowStart += m_LinkQuantum;
        for ( map<double,Note>::iterator it = m_RealTimeList.lower_bound(windowStart);
                    it != m_RealTimeList.upper_bound(m_LinkQuantum); it++ )
            m_Captured.Add(it->second);
    }

    return & m_Captured;
}
