#include <algorithm>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <unordered_map>

#include "maAnsiUI.h"
#include "maCommand.h"
#include "maListBuilder.h"
#include "maPatternStore.h"
#include "maScreen.h"
#include "maSequencer.h"
#include "maState.h"
#include "maStep.h"

using namespace std;

// These (one of many variants that I tried) get us round
// "undefined reference to _write
extern "C"{
    int _write();
}

extern "C"{
    int _write(){return -1;}
}

vector<string> words = {"Progress ...", "         ... bar ...", "                 ... here."};

unordered_map<string, int> testMap =
{
    {"Progress ...", 1},
    {"         ... bar ...", 7},
    {"                 ... here.", 400}
};

Sequencer g_Sequencer;
ListBuilder g_ListBuilder;

AnsiUI g_TextUI;
State g_State;


StepList StepList::EmptyList;
RealTimeList RealTimeList::EmptyList;


PatternStore g_PatternStore;
Pattern Pattern::EmptyPattern;

// Maintain a 64 bit time counter
// as the built in micros() function
// wraps at about 70 minutes (4200
// seconds).

uint64_t g_SysTimeMicros = 0;

uint64_t UpdateSysTimeMicros()
{
    static uint32_t time_wraps = 0;
    static uint32_t time_prev_u = 0;

    uint32_t current_u = micros();

    if ( current_u < time_prev_u )
        time_wraps += 1;

    time_prev_u = current_u;

//    uint64_t t = time_wraps;

    return g_SysTimeMicros = (static_cast<uint64_t>(time_wraps) << 32) + current_u;
}


void setup()
{
    g_PatternStore.SetFocus();

    Serial.begin(115200);
    delay(1000); // Do this else the following print() is missed.
    g_TextUI.FWriteXY(0, 0, "Hello, world ...");
    delay(1000);

    g_TextUI.ResetScreen();
    set_top_line();
    queue_next_step(0);

}

void loop() {

    UpdateSysTimeMicros();

    // User input ...

    if ( Serial.available() > 0 )
    {
        BaseUI::key_command_t key = g_TextUI.KeyInput();
        handle_key_input(key);
    }

    // Midi read ... (I don't know if there's an input queue or how big it is, we'll have to experiment)

    while ( usbMIDI.read() )
    {
        //  ... and should be buffer incoming events ourselves or handle
        // them as we see them. (We'll do the latter, to begin with.)

        snd_seq_event_t ev = {0};

        // Not sure why they don't just give us the raw type value. I'm
        // sure they have their reasons.

        int type = usbMIDI.getType();
        g_TextUI.FWriteXY(0, 21, "Midi: %0x", type);

        switch (type)
        {
            case usbMIDI.NoteOff:               //	0x80	Note Off
                ev.type = SND_SEQ_EVENT_NOTEOFF;
                break;
            case usbMIDI.NoteOn:                //  0x90	Note On
                ev.type = SND_SEQ_EVENT_NOTEON;
                break;
            case usbMIDI.AfterTouchPoly:        //  0xA0	Polyphonic AfterTouch
            case usbMIDI.ControlChange:	        //  0xB0	Control Change / Channel Mode
            case usbMIDI.ProgramChange:	        //  0xC0	Program Change
            case usbMIDI.AfterTouchChannel:	    //  0xD0	Channel (monophonic) AfterTouch
            case usbMIDI.PitchBend:	            //  0xE0	Pitch Bend
            case usbMIDI.SystemExclusive:	    //  0xF0	System Exclusive
            case usbMIDI.TimeCodeQuarterFrame:	//  0xF1	System Common - MIDI Time Code Quarter Frame
            case usbMIDI.SongPosition:	        //  0xF2	System Common - Song Position Pointer
            case usbMIDI.SongSelect:            //  0xF3	System Common - Song Select
            case usbMIDI.TuneRequest:	        //  0xF6	System Common - Tune Request
            case usbMIDI.Clock:	                //  0xF8	System Real Time - Timing Clock
            case usbMIDI.Start:	                //  0xFA	System Real Time - Start
            case usbMIDI.Continue:	            //  0xFB	System Real Time - Continue
            case usbMIDI.Stop:	                //  0xFC	System Real Time - Stop
            case usbMIDI.ActiveSensing:	        //  0xFE	System Real Time - Active Sensing
            case usbMIDI.SystemReset:	        //  0xFF	System Real Time - System Reset        }
                break;
        }


        switch (ev.type)
        {
            case SND_SEQ_EVENT_NOTEON:
            case SND_SEQ_EVENT_NOTEOFF:
                ev.time.time.tv_sec = g_SysTimeMicros / 1000000;
                ev.time.time.tv_nsec = (g_SysTimeMicros % 1000000) * 1000;
                ev.data.note.note = usbMIDI.getData1();
                ev.data.note.velocity = usbMIDI.getData2();
                g_TextUI.FWriteXY(0, 22, "Type: %0x Note: %i, Vel: %i)", ev.type, ev.data.note.note, ev.data.note.velocity);
                handle_midi_event(& ev);
                break;
            default:
                break;
        }
    }


    // Midi send queue ...

    uint8_t midiChannel = g_Sequencer.MidiChannel();
    bool callStep = false;
    while ( snd_seq_event_t * ev = g_Sequencer.GetEvent(g_SysTimeMicros) )
    {
        switch (ev->type)
        {
            case SND_SEQ_EVENT_ECHO:
                // This is our 'tick', so schedule everything
                // that should happen next, including the
                // next tick.
                callStep = true;    // Defer until we've sent out all other midi.
//                queue_next_step(0);
                break;
            case SND_SEQ_EVENT_NOTEON:
                usbMIDI.sendNoteOn(ev->data.note.note, ev->data.note.velocity, midiChannel);
                usbMIDI.send_now();
                break;
            case SND_SEQ_EVENT_NOTEOFF:
                usbMIDI.sendNoteOff(ev->data.note.note, ev->data.note.velocity, midiChannel);
                usbMIDI.send_now();
                break;
        }
        g_Sequencer.PopEvent();
    }

    if ( callStep )
        queue_next_step(0);

#if 0
    // Occasional debug code ...

    unsigned long currentMillis = millis();
    static unsigned long previousMillis = 0;
    static unsigned long interval = 100;

    if ( currentMillis - previousMillis > interval )
    {
        previousMillis = currentMillis;
        g_TextUI.FWriteXY(4, 8, " millis:%11lu", currentMillis);
        g_TextUI.FWriteXY(4, 9, " micros:%14llu", g_SysTimeMicros);
    }
#endif
}
