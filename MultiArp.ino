#include <algorithm>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <unordered_map>

#include <avr/wdt.h>

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

// Watchdog things

void startup_early_hook()
{
    WDOG_TOVALL = (1000); // The next 2 lines sets the time-out value. This is the value that the watchdog timer compare itself to.
    WDOG_TOVALH = 0;
    WDOG_STCTRLH = (WDOG_STCTRLH_ALLOWUPDATE | WDOG_STCTRLH_WDOGEN | WDOG_STCTRLH_WAITEN | WDOG_STCTRLH_STOPEN); // Enable WDG
    //WDOG_PRESC = 0; // prescaler
}

volatile int deliberateStall = 0;
volatile uint8_t dogCount = 0;
IntervalTimer wdTimer;


void DogHandler()
{
    // Can we just grab the programcounter first, every time,
    // but only do something with it if we think there's a problem.

#if 0
  // Setup a pointer to the program counter. It goes in a register so we
  // don't mess up the stack.
  uint8_t upStack = (uint8_t*)SP;

  // The stack pointer on the AVR micro points to the next available location
  // so we want to go back one location to get the first byte of the address
  // pushed onto the stack when the interrupt was triggered. There will be
  // PROGRAM_COUNTER_SIZE bytes there.
  ++upStack;
//void CApplicationMonitor::WatchdogInterruptHandler(uint8_t *puProgramAddress)
//  CApplicationMonitorHeader Header;
//
//  LoadHeader(Header);
//  memcpy(m_CrashReport.m_auAddress, puProgramAddress, PROGRAM_COUNTER_SIZE);
//  SaveCurrentReport(Header.m_uNextReport);

#endif

//  Serial.print("Kicking the dog! ");
//  Serial.println(dogCount++);
    if ( dogCount > 0 )
    {
        static bool firstTime = true;
        if ( firstTime )
        {
            Serial.print("\033[12;20H !!! Main loop stopped ... !!! ");
            // Adding (reverse) attribute here seems to affect other writes.
//            Serial.print("\033[12;25H\033[5m Main loop stopped ... !!! ");
            // First time doesn't work. We seem to need second or third
            // attempt before anything appears.
//            firstTime = false;
        }
        deliberateStall = 0;
//        if ( dogCount % 8 == 0 )
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
//    else
//        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    noInterrupts();
    WDOG_REFRESH = 0xA602;
    WDOG_REFRESH = 0xB480;
    interrupts();
    dogCount ++;
}




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
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);

    g_PatternStore.SetFocus();

    Serial.begin(115200);
    delay(1000); // Do this else the following print() is missed.
    // You have about 6 secs to open the serial monitor before a watchdog reset
    while(!Serial);
    delay(100);
//    printResetType();
    wdTimer.begin(DogHandler, 500000); // kick the dog every 500msec

    g_TextUI.FWriteXY(0, 0, "Hello, world ...");
    delay(1000);

    g_TextUI.ResetScreen();
    set_top_line();
    queue_next_step(0);

}

int loopCount = 0;

void loop()
{
    dogCount = 0;

    UpdateSysTimeMicros();

//    if ( ++loopCount % 50000 == 0 )
//        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

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

    uint8_t midiChannel = g_Sequencer.MidiChannel() + 1; // usbMIDI not zero based, it seems.
    bool callStep = false;
    bool sendNow = false;

    int eventsProcessed = 0;
    while ( snd_seq_event_t * ev = g_Sequencer.GetEvent(g_SysTimeMicros) )
    {
        switch (ev->type)
        {
            case SND_SEQ_EVENT_ECHO:
                // This is our 'tick', so schedule everything
                // that should happen next, including the
                // next tick.
                callStep = true;    // Defer until we've sent out all other midi.
                digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
//                queue_next_step(0);
                break;
            case SND_SEQ_EVENT_NOTEON:
//                digitalWrite(LED_BUILTIN, HIGH);
                usbMIDI.sendNoteOn(ev->data.note.note, ev->data.note.velocity, midiChannel);
                sendNow = true;
                break;
            case SND_SEQ_EVENT_NOTEOFF:
//                digitalWrite(LED_BUILTIN, LOW);
                usbMIDI.sendNoteOff(ev->data.note.note, ev->data.note.velocity, midiChannel);
                sendNow = true;
                break;
        }
        g_Sequencer.PopEvent();
        eventsProcessed++;
    }

    if ( callStep )
    {
//        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        queue_next_step(0);
    }

    if ( sendNow )
    {
        g_TextUI.FWriteXY(4, 8, "Events: %i", eventsProcessed);
        usbMIDI.send_now();
    }

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
