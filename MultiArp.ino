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

int messageNumber = 0;
unsigned long previousMillis = 0;
unsigned long interval = 1000;

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

uint64_t sysTimeMicros()
{
    static uint32_t time_wraps = 0;
    static uint32_t time_prev_u = 0;

    uint32_t current_u = micros();

    if ( current_u < time_prev_u )
        time_wraps++;

    time_prev_u = current_u;

    uint64_t t = time_wraps;
    return (t << 32) + current_u;
}

void loop() {

    // User input ...

    if ( Serial.available() > 0 )
    {
        BaseUI::key_command_t key = g_TextUI.KeyInput();
        handle_key_input(key);
    }

    // Midi send queue ...

    uint8_t midiChannel = g_Sequencer.MidiChannel();

    while ( snd_seq_event_t * ev = g_Sequencer.GetEvent(sysTimeMicros()) )
    {
        switch (ev->type)
        {
            case SND_SEQ_EVENT_ECHO:
                // This is our 'tick', so schedule everything
                // that should happen next, including the
                // next tick.
                queue_next_step(0);
                break;
            case SND_SEQ_EVENT_NOTEON:
                usbMIDI.sendNoteOn(ev->data.note.note, ev->data.note.velocity, midiChannel);
                break;
            case SND_SEQ_EVENT_NOTEOFF:
                usbMIDI.sendNoteOff(ev->data.note.note, ev->data.note.velocity, midiChannel);
                break;
        }
        g_Sequencer.PopEvent();
    }

    // Check if it's time for another tick ...

//    unsigned long currentMillis = millis();
//
//    g_TextUI.FWriteXY(4, 8, "m:%lu", currentMillis);
//    g_TextUI.FWriteXY(4, 9, "u:%llu", sysTimeMicros());
//
//    if ( currentMillis - previousMillis > interval )
//    {
//        previousMillis = currentMillis;
//
//        g_TextUI.SendSaveCursor();
//        g_TextUI.FWriteXY(12, 2, "%s (%i)", words.at(messageNumber).c_str(), testMap.at(words.at(messageNumber)));
//        g_TextUI.ClearEOL();
//        g_TextUI.SendRestoreCursor();
//        ++messageNumber %= 3;
//    }

}
