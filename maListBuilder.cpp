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



#include "maBaseUI.h"
#include "maListBuilder.h"
#include "maScreen.h"

#if defined(MA_BLUE)
#include <cstdio>

#include "maState.h"

#else
#include <alsa/asoundlib.h>
#endif // MA_BLUE

#if defined(MA_BLUE) && !defined(MA_BLUE_PC)

#include "alsa_types.h"

#else
#define LINK_PLATFORM_LINUX
#include <ableton/Link.hpp>

#include <alsa/asoundlib.h>

#endif // MA_BLUE


using namespace std;

// From curses.h

#define KEY_BACKSPACE	0407		/* backspace key */

#define LOG_ON 0

#if LOG_ON
ofstream fLog;
#endif

ListBuilder::ListBuilder()
{
    //ctor
}

#if defined(MA_BLUE)

ListBuilder g_ListBuilder;

#else

extern ableton::Link g_Link;
ListBuilder g_ListBuilder(&g_Link);

ListBuilder::ListBuilder(ableton::Link * linkInstance):
    m_Link(linkInstance)
{
    //ctor
#if LOG_ON
    fLog.open("ListBuilder.log");
#endif

}
#endif // !MA_BLUE

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

//        if ( m_MidiInputMode == MIDI_INPUT_OFF )
//        {
            m_StepList.Clear();
            m_Captured.Clear();
//        }
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

        case MIDI_INPUT_CHORD:
            return 'C';

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
#if defined(MA_BLUE)
                const char * format = " Open: %u, captured: %u";
#else
                const char * format = " Open: %lu, captured: %lu";
#endif
                snprintf(buff, 100, format, m_OpenNoteMap.size(), m_RealTimeList.size());
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
//            return m_Activity.ToString(false);
            return "";
    }
}


void ListBuilder::SetPhaseIsZero(double beat, double quantum)
{
    m_BeatAtLastPhaseZero = beat;
    m_LinkQuantum = quantum;
}


bool ListBuilder::HandleKeybInput(int key)
{
    if ( !g_State.RecState() )
        return false;

    switch (key)
    {
        case BaseUI::key_cmd_midi_commit:
            switch ( m_MidiInputMode )
            {
            case MIDI_INPUT_REAL_TIME:
                if ( !m_RealTimeList.empty() )
                {
                    ItemMenu::UpdateFocusItem(m_RealTimeList, g_State.Quantum());
                    Clear();
                    return true;
                }
                break;
            case MIDI_INPUT_STEP:
                if ( !m_StepList.Empty() )
                {
                    ItemMenu::UpdateFocusItem(m_StepList);
                    Clear();
                    return true;
                }
                break;
            default:
                break;
            }
            return false;

        case BaseUI::key_cmd_midi_rest:
            m_StepList.Add();
            return true;

        case BaseUI::key_cmd_midi_clear_last:
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

bool ListBuilder::HandleMidi(snd_seq_event_t *ev, double inBeat)
{
    if ( ! (m_TemporaryRecordOverride || g_State.RecState()) )
    {
        Note n;   // Something to show what's coming in.
        n.m_NoteNumber = ev->data.note.note;
        set_status(STAT_POS_2, "Midi: %s", n.ToString(false).c_str());
        return false;
    }

    int inputMode = m_TemporaryRecordOverride ? MIDI_INPUT_CHORD : m_MidiInputMode;

    switch ( inputMode )
    {
        case MIDI_INPUT_REAL_TIME:
            {
#ifdef MA_BLUE
                // MA_BLUE Todo: What are we going to use for timestamping?
                //               Hopefully, incoming event will be timestamped
                //               and we can calculate a beat from that.
                //               (If we know start time and have constant tempo,
                //               then life would be simple, but that isn't going
                //               to be the case.)

                double beat = g_State.BeatFromEvent(ev);
#else
                if ( m_Link == NULL )
                    throw string("ListBuilder::HandleMidi() - Expecting Ableton Link Instance to be set.");
                chrono::microseconds t_now = m_Link->clock().micros();
                ableton::Link::SessionState timeline = m_Link->captureAppSessionState();
                double beat = timeline.beatAtTime(t_now, m_LinkQuantum);
#endif
                // Create notes for note-on, complete notes and calculate
                // duration for note off.
#if LOG_ON
                char buff[100];
                snprintf(buff, 100, "Beat %6.2f", beat);
                fLog << buff;
#endif

                if ( ev->type == SND_SEQ_EVENT_NOTEON )
                {
                    Note note(ev->data.note.note, ev->data.note.velocity);
                    note.SetPhase(beat);    // Temporarily store beat in the notes Phase member.

                    map<unsigned char,Note>::iterator it = m_OpenNoteMap.find(ev->data.note.note);
                    if ( it != m_OpenNoteMap.end() )
                        m_OpenNoteMap.at(ev->data.note.note) = note;
                    else
                        m_OpenNoteMap.insert(make_pair(ev->data.note.note, note));
#if LOG_ON
                    snprintf(buff, 100, " opening %3i\n", ev->data.note.note);
                    fLog << buff;
#endif
                }
                else
                {
                    map<unsigned char,Note>::iterator it = m_OpenNoteMap.find(ev->data.note.note);
                    if ( it != m_OpenNoteMap.end() )
                    {
                        Note note = m_OpenNoteMap.at(ev->data.note.note);
                        m_OpenNoteMap.erase(it);

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
                        snprintf(buff, 100, " closing %3i, phase %6.2f length %6.2f\n",
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

                    map<unsigned char,Note>::iterator it = m_OpenNoteMap.find(ev->data.note.note);
                    if ( it != m_OpenNoteMap.end() )
                        m_OpenNoteMap.at(ev->data.note.note) = note;
                    else
                        m_OpenNoteMap.insert(make_pair(ev->data.note.note, note));
                }
                else
                {
                    map<unsigned char,Note>::iterator openPair = m_OpenNoteMap.find(ev->data.note.note);
                    if ( openPair != m_OpenNoteMap.end() )
                    {
                        double beatStart = openPair->second.Phase();
                        openPair->second.SetLength(inBeat - beatStart);

                        m_RealTimeList.insert(make_pair(openPair->second.Phase(), openPair->second));
                        m_OpenNoteMap.erase(openPair);

//                        Note note = m_OpenNoteMap.at(ev->data.note.note);
//                        m_OpenNoteMap.erase(it);
//
//                        double beatStart = note.Phase();
//                        note.SetLength(inBeat - beatStart);
//
//                        m_RealTimeList.insert(make_pair(note.Phase(), note));
                    }

                }

            }
            return false;

        case MIDI_INPUT_STEP:
            if ( ev->type == SND_SEQ_EVENT_NOTEON )
            {
                m_Captured.Add(ev->data.note.note, ev->data.note.velocity);
                m_OpenNotes += 1;
            }
            else if ( --m_OpenNotes <= 0 )  // I guess we might receive a stray note-off before any note-ons.
            {
                m_StepList.Add(m_Captured);
                m_Captured.Clear();
            }
            break;

        case MIDI_INPUT_CHORD:

            // Build the current chord until all notes released.

            if ( ev->type == SND_SEQ_EVENT_NOTEON )
            {
                m_Captured.Add(ev->data.note.note, ev->data.note.velocity);
                m_OpenNotes += 1;
            }
            else if ( --m_OpenNotes <= 0 )
            {
                ItemMenu::UpdateFocusItem(m_Captured);
                Clear();
                return true;
            }
            break;

        case MIDI_INPUT_QUICK:
            if ( ev->type == SND_SEQ_EVENT_NOTEON )
            {
                m_StepList.Add(ev->data.note.note, ev->data.note.velocity);
                m_OpenNotes += 1;
            }
            else if ( m_OpenNotes > 0 )
            {
                m_OpenNotes -= 1;
            }
            if ( m_OpenNotes == 0 )
            {
                ItemMenu::UpdateFocusItem(g_ListBuilder.CurrentList());
                Clear();
                return true;
            }
            break;

        default:
            break;
    }

    return false;
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
