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
#include "maSequencer.h"
#include "maState.h"

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
    Serial.begin(115200);
    delay(1000); // Do this else the following print() is missed.
    g_TextUI.WriteXY(0, 0, "Hello, world ...");
    delay(1000);

    g_TextUI.ResetScreen();
}


void loop() {

    if ( Serial.available() > 0 )
    {
        // incomingByte = Serial.read();
        BaseUI::key_command_t key = g_TextUI.KeyInput();
        handle_key_input(key);
    }

    // Check if it's time for another tick ...

    unsigned long currentMillis = millis();

    if ( currentMillis - previousMillis > interval )
    {
        previousMillis = currentMillis;
        g_TextUI.FWriteXY(4, 2, "%s (%i)", words.at(messageNumber).c_str(), testMap.at(words.at(messageNumber)));
        g_TextUI.ClearEOL();
        ++messageNumber %= 3;
    }

}
