#include <algorithm>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <unordered_map>

#include "maCommand.h"
#include "maTextUI.h"

using namespace std;

void setup()
{
    Serial.begin(9600);
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

string commandString;
int incomingByte;

void set_status(int y, int x, const char *format, ... )
{
    char text[100];
    va_list args;
    va_start(args, format);
    vsnprintf(text, 100, format, args);
    va_end(args);

    char ansi[20];
    snprintf(ansi, 20, "\033[%i;%iH", y, x);

    Serial.print(ansi);
    Serial.print(text);
    Serial.print("\033[0K");

}

void loop() {

    if ( Serial.available() > 0 )
    {
        // read the incoming byte:
        incomingByte = Serial.read();
        handle_key_input(CursorKeys::no_key, (xcb_keysym_t)incomingByte);
    }
    else
    {
        incomingByte = -1;
    }

    char text[80];

    if ( incomingByte >= 0 )
    {
        snprintf(text, 80, "\033[20;4HKey: %i \033[0K", incomingByte);
        Serial.print(text);
    }



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
