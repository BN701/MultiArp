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

using namespace std;

void setup()
{
    Serial.begin(38400);
    delay(1000); // Do this else the following print() is missed.
    Serial.print("\033[2J");

}

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

AnsiUI g_TextUI;
ListBuilder g_ListBuilder;
PatternStore g_PatternStore;
Sequencer g_Sequencer;

StepList StepList::EmptyList;
RealTimeList RealTimeList::EmptyList;
Pattern Pattern::EmptyPattern;

string commandString;
int incomingByte;

//void set_status(int y, int x, const char *format, ... )
//{
//    char text[100];
//    va_list args;
//    va_start(args, format);
//    vsnprintf(text, 100, format, args);
//    va_end(args);
//
//    char ansi[20];
//    snprintf(ansi, 20, "\033[%i;%iH", y, x);
//
//    Serial.print(ansi);
//    Serial.print(text);
//    Serial.print("\033[0K");
//
//}

size_t SerialWrite(const char * text)
{
    return Serial.write(text);
}

int SerialGetChar()
{
    return Serial.read();
}

void loop() {

    char text[80];

//    if ( Serial.available() > 0 )
//    {
//        // incomingByte = Serial.read();
//        BaseUI::key_command_t key = g_TextUI.KeyInput();
//        handle_key_input(key);
//    }
//    else
//    {
//        incomingByte = -1;
//    }
//
//
//    if ( incomingByte >= 0 )
//    {
//        snprintf(text, 80, "\033[20;4HKey: %i \033[0K", incomingByte);
//        Serial.print(text);
//    }

    // Check if it's time for another tick ...

    unsigned long currentMillis = millis();

    if ( currentMillis - previousMillis > interval )
    {
        previousMillis = currentMillis;
        snprintf(text, 80, "\033[3;4H%s (%i) \033[0K", words.at(messageNumber).c_str(), testMap.at(words.at(messageNumber)));
        Serial.print(text);
        if ( ++messageNumber > 2 )
            messageNumber = 0;
        }

}
