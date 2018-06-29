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
// "undefined reference to _write"
extern "C"{
    int _write();
}

extern "C"{
    int _write(){return -1;}
}

extern "C" char* sbrk(int incr);
extern char _estack;
uint32_t g_heap_0 = reinterpret_cast<uint32_t>(sbrk(0));

// Watchdog things

void printResetType() {
  if (RCM_SRS1 & RCM_SRS1_SACKERR)   Serial.println("[RCM_SRS1] - Stop Mode Acknowledge Error Reset");
  if (RCM_SRS1 & RCM_SRS1_MDM_AP)    Serial.println("[RCM_SRS1] - MDM-AP Reset");
  if (RCM_SRS1 & RCM_SRS1_SW)        Serial.println("[RCM_SRS1] - Software Reset");
  if (RCM_SRS1 & RCM_SRS1_LOCKUP)    Serial.println("[RCM_SRS1] - Core Lockup Event Reset");
  if (RCM_SRS0 & RCM_SRS0_POR)       Serial.println("[RCM_SRS0] - Power-on Reset");
  if (RCM_SRS0 & RCM_SRS0_PIN)       Serial.println("[RCM_SRS0] - External Pin Reset");
  if (RCM_SRS0 & RCM_SRS0_WDOG)      Serial.println("[RCM_SRS0] - Watchdog(COP) Reset");
  if (RCM_SRS0 & RCM_SRS0_LOC)       Serial.println("[RCM_SRS0] - Loss of External Clock Reset");
  if (RCM_SRS0 & RCM_SRS0_LOL)       Serial.println("[RCM_SRS0] - Loss of Lock in PLL Reset");
  if (RCM_SRS0 & RCM_SRS0_LVD)       Serial.println("[RCM_SRS0] - Low-voltage Detect Reset");
}


#ifdef __cplusplus
extern "C" {
void startup_early_hook();
}
extern "C" {
#endif
  void startup_early_hook() {
#if 1
    // clock source 0 LPO 1khz, 4 s timeout
    WDOG_TOVALL = 5000; // The next 2 lines sets the time-out value. This is the value that the watchdog timer compare itself to.
    WDOG_TOVALH = 0;
    WDOG_STCTRLH = (WDOG_STCTRLH_ALLOWUPDATE | WDOG_STCTRLH_WDOGEN | WDOG_STCTRLH_WAITEN | WDOG_STCTRLH_STOPEN); // Enable WDG
#else
    // bus clock  4s timeout
    uint32_t ticks = 4000 * (F_BUS / 1000); // ms
    WDOG_TOVALL = ticks & 0xffff; // The next 2 lines sets the time-out value. This is the value that the watchdog timer compare itself to.
    WDOG_TOVALH = (ticks >> 16) & 0xffff;
    WDOG_STCTRLH = (WDOG_STCTRLH_ALLOWUPDATE | WDOG_STCTRLH_CLKSRC | WDOG_STCTRLH_WDOGEN | WDOG_STCTRLH_WAITEN | WDOG_STCTRLH_STOPEN);
#endif
    WDOG_PRESC = 0; // prescaler
  }
#ifdef __cplusplus
}
#endif


volatile int deliberateStall = 0;
volatile uint8_t dogCount = 0;

DMAMEM int g_debug_step = -1;
DMAMEM int g_debug_file = -1;
DMAMEM int g_debug_lineno = -1;
DMAMEM char g_debug_message[80];
DMAMEM int reboot_count = 0;


void DogHandler()
{
    // Can we just grab the program counter first, every time,
    // but only do something with it if we think there's a problem.


//  Serial.print("Kicking the dog! ");
//  Serial.println(dogCount++);
    if ( dogCount > 0 )
    {
        static bool firstTime = true;
        if ( firstTime )
        {
            char message[50];
            snprintf(message, 50, "\033[12;20H !!! Main loop stopped, step: %i ... !!!", g_debug_step);
            Serial.print(message);
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

AnsiUI g_TextUI;

StepList StepList::EmptyList;
RealTimeList RealTimeList::EmptyList;



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

string last_debug_step_before_reboot;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);

    g_PatternStore.SetFocus();
    ItemMenu::SetDefaultFocus();

//    Serial.begin(115200);
//    delay(1000); // Do this else the following print() is missed.
    // You have about 6 secs to open the serial monitor before a watchdog reset
    while(!Serial);
    delay(100);
//    wdTimer.begin(DogHandler, 500000); // kick the dog every 500msec

    g_TextUI.ResetScreen();
    g_TextUI.FWriteXY(0, 0, "Hello, world (%i) ...\n\n", g_debug_step);
    delay(2000);

    g_TextUI.ResetScreen();
    uint32_t heap_base = g_heap_0;
    g_heap_0 = reinterpret_cast<uint32_t>(sbrk(0));
    g_TextUI.FWriteXY(0, 7, "Stack base: %08X, Heap base: %08X, Heap now: %08X, used already: %i", & _estack, heap_base, g_heap_0, g_heap_0 - heap_base);
    g_TextUI.FWriteXY(0, 8, "Last logged positions: loop stage %i, file %i, line %i\n\r", g_debug_step, g_debug_file, g_debug_lineno);
    printResetType();
    Serial.println(g_debug_message);
    g_debug_message[0] = 0;
    g_debug_file = -1;
    g_debug_lineno = -1;
    set_top_line();
    queue_next_step(NULL);

}

int loopCount = 0;
queue<snd_seq_event_t> ticks;
elapsedMillis dog_time = 0;

void loop()
{
    if ( dog_time > 1000 )
    {
        DogHandler();
        dog_time = 0;
    }

    g_debug_step = 0;

    dogCount = 0;

    UpdateSysTimeMicros();

//    if ( ++loopCount % 50000 == 0 )
//        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

    // User input ...

    if ( Serial.available() > 0 )
    {
        g_debug_step = 10;
        BaseUI::key_command_t key = g_TextUI.KeyInput();
        g_debug_step = 101;
        if ( ! handle_key_input(key) )
        {
            g_debug_step = 1011;
            set_status(STAT_POS_2, "Deliberate stall to force watchdog restart ...");
            while (true);
        }
    }

    // Midi read ... (I don't know if there's an input queue or how big it is, we'll have to experiment)

    while ( usbMIDI.read() )
    {
        g_debug_step = 20;
        //  ... and should be buffer incoming events ourselves or handle
        // them as we see them. (We'll do the latter, to begin with.)

        snd_seq_event_t ev = {0};

        // Not sure why they don't just give us the raw type value. I'm
        // sure they have their reasons.

        int type = usbMIDI.getType();
        int data1 = usbMIDI.getData1();
        int data2 = usbMIDI.getData2();

        g_TextUI.FWriteXY(0, 21, "Midi: %0x, %03i, %03i", type, data1, data2);

        switch (type)
        {
            case usbMIDI.NoteOff:               //	0x80	Note Off
                ev.type = SND_SEQ_EVENT_NOTEOFF;
                break;
            case usbMIDI.NoteOn:                //  0x90	Note On
                ev.type = SND_SEQ_EVENT_NOTEON;
                break;
            case usbMIDI.ControlChange:	        //  0xB0	Control Change / Channel Mode
                ev.type = SND_SEQ_EVENT_CONTROLLER;
                break;
            case usbMIDI.AfterTouchPoly:        //  0xA0	Polyphonic AfterTouch
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
                ev.data.note.note = data1;
                ev.data.note.velocity = data2;
                g_TextUI.FWriteXY(0, 22, "Type: %0x Note: %i, Vel: %i)", ev.type, ev.data.note.note, ev.data.note.velocity);
                handle_midi_event(& ev);
                break;
            case SND_SEQ_EVENT_CONTROLLER:
                ev.data.control.param = data1;
                ev.data.control.value = data2;
                handle_midi_event(& ev);
                break;
            default:
                break;
        }
    }


    // Midi send queue ...

    uint8_t midiChannel = g_Sequencer.MidiChannel() + 1; // usbMIDI not zero based, it seems.
//    bool callStep = false;
    bool sendNow = false;

    int eventsProcessed = 0;
    while ( snd_seq_event_t * ev = g_Sequencer.GetEvent(g_SysTimeMicros) )
    {
        g_debug_step = 30;
        switch (ev->type)
        {
            case SND_SEQ_EVENT_ECHO:
                // This is our 'tick', so schedule everything
                // that should happen next, including the
                // next tick.
                ticks.push(*ev);
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

//    int debug_loop_count = 0;
    while ( !ticks.empty() )
    {
        g_debug_step = 40;
        queue_next_step(&ticks.front());
        ticks.pop();
//        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
//        if ( ++debug_loop_count > 100 )
//        {
//            set_status(STAT_POS_2, "Breaking out of tick loop ...");
//            break;
//        }
    }

    if ( sendNow )
    {
//        g_TextUI.FWriteXY(4, 8, "Events: %i", eventsProcessed);
        g_debug_step = 50;
        usbMIDI.send_now();
    }

    g_debug_step = 60;

    update_item_menus();

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
    g_debug_step = 70;
}
