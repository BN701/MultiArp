# Multi Arp

## Command and Menu Reference

**BN - Dec 17, 2017**

## **label**

Synonyms: ***name***, ***n***

Sets a label for the current pattern.

---

## **midi**

Synonyms: **m**, **channel**, **chan**

Parameters: ***n***, ***real time***, ***step***, ***quick***, ***off***

**midi *n***

Set output midi channel to ***n***, where n ranges from 1 to 16. (The app currently listens on all channels for incoming midi.)

**midi step**

Synonyms: ***midi s***, ***m s***

Sets midi input more to *step*. Hold down successive notes or chords on the midi keyboard for each steo in the sequence. Press ***space*** on the PC keyboard to enter a rest. ***Backspace*** deletes the last step. ***Enter*** completes entry and updates the current list.

**midi quick**

Synonyms: ***midi q***, ***m q***

Sets midi input mode to *quick*. Hold down a chord or series of notes to enter a monophonic sequence. Notes continue to be added in the order you press keys for as long as at least one key is key is held. The *held* key can change as long as you play legato. Letting go of the last note completes note entry and updates the current list.

Note that in *quick mode* you can't enter chords or rests.

**midi real time**

Synonyms: ***midi rt***, ***m rt***

Sets midi input mode to *real time*. Notes are captured in real time and playback quantized to the current *step value*. The input buffer will loop according to the current Ableton Link *quantum* setting. Captured notes will play back when their time comes. ***Backspace*** will delete the most recently entered note.  Capture continues until ***Enter*** is pressed and a new *real time* list is added to the current pattern.

(Quantized playback will be configurable to be independent of *step value*, with an option to turn it off altogether.)

**midi off**

Synonyms: ***m off***

Turns off midi input.

**Notes**

The background colour of the status bar changes to orange for *step* or *quick* mode, and red for *real time* mode.

---

## **page**

Synonyms: ***p***

Parameters: ***one***, ***1***, ***two***, ***2***, etc, ***hold***

Key combinations: ***tab***, ***shift-tab***

Sets the display mode, or *page*, for the lower two-thirds of the display. Pages one and two show step and real time lists, the only difference between these two modes is the way that step lists scroll across the screen.

Page three is a work-in-progress attempt to show scale transposion activity. Here you can use ***hold*** to turn off screen updates for a closer look.

---

## **quit**

Synonyms: ***exit***

Close this instance of the sequencer.

Note that there is no confirmation prompt and all pattern data will be lost.

---

## **reset**

Parameters: ***all***, ***beat***

On it's own, *reset* causes the current edit pattern's internal position counters  to zero, so all lists will start playing from the beginning. If the edit pattern also happens to be playing, then the reset will happen when global beat value returns to one.

Reset ***all*** will reset all patterns when global beat value returns to one.

Reset ***beat*** is for situaions where the current beat is incrementing in values that never reach an exact multiple of the current Ableton Link *quantum* value. It takes the current beat value and shifts it to the next whole number. The sequencer will take this into account when it schedules its next step and there will be an audible glitch in timing.


---

## **run**, **stop**

Starts and stops the sequencer on the next Ableton Link *quantum* boundary.

The point at which we stop depends on beat value hitting an exact multiple of quantum. There are times when this won't happen. For instance when we change *step value* we may find ourselves with odd, fractional beat values that are incrementing in regular amounts. e.g. 1.25, 2.25, 3.25, 4.25. In this case use ***halt***, or ***stop now***, to stop the sequencer immediately. (You can also try ***reset beat*** to get out of these situaions without stopping.) Overall, this whole area needs some additional thought.

---





```
{"help", C_HELP},




{"autoreset", C_SET_RESETONPATTERNCHANGE},

{"note", C_STEPVAL},
{"step", C_STEPVAL},
{"quantum", C_QUANTUM},
{"q", C_QUANTUM},
{"gate", C_GATELENGTH},
{"gate hold", C_GATE_HOLD,},
{"gate h", C_GATE_HOLD,},
{"gate normal", C_GATE_NORMAL,},
{"gate n", C_GATE_NORMAL,},

{"velocity", C_VELOCITY},
{"vel", C_VELOCITY},

{"tran", C_TRANSPOSE},
{"transpose", C_TRANSPOSE},

{"trigs", C_TRIGS},
{"t", C_TRIGS},
{"trigs arp", C_TRIGS_ARPEGGIO},
{"t arp", C_TRIGS_ARPEGGIO},

{"scale", C_SCALE},
{"s", C_SCALE},
{"scale from list", C_SCALE_FROM_LIST},
{"scale show", C_SCALE_SHOW},
{"scale help", C_SCALE_HELP},
{"help scale", C_SCALE_HELP},
{"scale controls", C_SCALE_CONTROLS},
{"scale c", C_SCALE_CONTROLS},
{"scale k", C_SCALE_CONTROLS},
{"root", C_SETROOT},

{"feel", C_FEEL},
{"f", C_FEEL},
{"feel help", C_FEEL_HELP},
{"f help", C_FEEL_HELP},
{"help feel", C_FEEL_HELP},
{"feel on", C_FEEL_ON},
{"f on", C_FEEL_ON},
{"feel off", C_FEEL_OFF},
{"f off", C_FEEL_OFF},
{"feel new", C_FEEL_NEW},
{"f new", C_FEEL_NEW},
{"feel add", C_FEEL_ADD},
{"f add", C_FEEL_ADD},
{"feel remove", C_FEEL_REMOVE},
{"f remove", C_FEEL_REMOVE},
{"feel new", C_FEEL_NEW},
{"f new", C_FEEL_NEW},
{"feel respace", C_FEEL_RESPACE},
{"f respace", C_FEEL_RESPACE},

{"edit", C_EDIT},
{"e", C_EDIT},
{"cue", C_CUE},
{"play", C_CUE},
{"new", C_NEW},
{"copy", C_COPY},
{"delete", C_DELETE},
{"del", C_DELETE},
{"clear", C_CLEAR},

{"chain", C_PATTERN_CHAIN},
{"c", C_PATTERN_CHAIN},
{"chain:", C_PATTERN_CHAIN},

{"chain off", C_PATTERN_CHAIN_OFF},
{"chain natural", C_PATTERN_CHAIN_NATURAL},
{"chain n", C_PATTERN_CHAIN_NATURAL},
{"c n", C_PATTERN_CHAIN_NATURAL},
{"chain quantum", C_PATTERN_CHAIN_QUANTUM},
{"chain q", C_PATTERN_CHAIN_QUANTUM},
{"c q", C_PATTERN_CHAIN_QUANTUM},
{"chain clear", C_PATTERN_CHAIN_CLEAR},
{"chain new", C_PATTERN_CHAIN_NEW},
{"chain delete", C_PATTERN_CHAIN_DELETE},
{"chain jump", C_PATTERN_CHAIN_JUMP},
{"chain help", C_PATTERN_CHAIN_HELP},

{"lock", C_EDIT_CURSOR_LOCK},
{"unlock", C_EDIT_CURSOR_UNLOCK},

{"list", C_LIST},
{"l", C_LIST},
{"import", C_LIST_IMPORT},
{"i", C_LIST_IMPORT},
{"real time", C_LIST_RT},
{"rt", C_LIST_RT},
{"real time delete", C_LIST_RT_DELETE},
{"rt delete", C_LIST_RT_DELETE},
{"rt del", C_LIST_RT_DELETE},
{"real time rate", C_LIST_RT_RATE},
{"rt rate", C_LIST_RT_RATE},
{"rt r", C_LIST_RT_RATE},
{"real time quantum", C_LIST_RT_QUANTUM},
{"rt quantum", C_LIST_RT_QUANTUM},
{"rt q", C_LIST_RT_QUANTUM},
{"real time phase", C_LIST_RT_START_PHASE},
{"rt p", C_LIST_RT_START_PHASE},
{"real time echo", C_LIST_RT_ECHO},
{"rt echo", C_LIST_RT_ECHO},
{"rt e", C_LIST_RT_ECHO},

{"status", C_STATUS},
{"stat", C_STATUS},
{"undo", C_UNDO},

{"store", C_STORE},
{"store step", C_STORE_STEP},
{"store gate", C_STORE_GATE},
{"store vel", C_STORE_VELOCITY},
{"store scale", C_STORE_SCALE},
{"store all", C_STORE_ALL},
{"store help", C_STORE_HELP},
{"help store", C_STORE_HELP},
{"load help", C_STORE_HELP},
{"help load", C_STORE_HELP},
{"load", C_LOAD},
{"load step", C_LOAD_STEP},
{"load gate", C_LOAD_GATE},
{"load vel", C_LOAD_VELOCITY},
{"load scale", C_LOAD_SCALE},
{"load all", C_LOAD_ALL},

{"use", C_USE},
{"use globals", C_USE_GLOBAL_PLAYDATA},
{"use g", C_USE_GLOBAL_PLAYDATA},
{"use pattern", C_USE_PATTERN_PLAYDATA},
{"use p", C_USE_PATTERN_PLAYDATA},
{"use help", C_USE_HELP},
{"help use", C_USE_HELP},

{"misc", C_HELP_1},
{"pattern", C_HELP_3},
{"control", C_HELP_2}
};
```
