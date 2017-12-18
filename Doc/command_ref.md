# Multi Arp Command and Menu Reference: Home

**BN - Dec 17, 2017**

#### Sections

* [**Using the command line**](#using-the-command-line)

* [**Changing the playing pattern**](#changing-the-playing-pattern)

* [**Misc Commands**](command_ref_misc.md)

 * [autoreset](command_ref_misc.md#autoreset)
 * [label](command_ref_misc.md#label)
 * [midi](command_ref_misc.md#midi)
 * [page](command_ref_misc.md#page)
 * [quantum](command_ref_misc.md#quantum)
 * [quit](command_ref_misc.md#quit)
 * [reset](command_ref_misc.md#reset)
 * [run, stop](command_ref_misc.md#run-stop)
 * [status](command_ref_misc.md#status)
 * [step](command_ref_misc.md#step)
 * [velocity, gate](command_ref_misc.md#velocity-gate)



## Using the command line

There's a command prompt on line 7 of the UI. To make anything happen, you have to type at least one ***command***, though some of them will also open up a ***menu***.

The complete set commands appears below. Many of them also have abbreviated forms, and I've listed these too. Most commands have one or more ***parameters***.

Some commands act on a particular pattern, or a particular list within a pattern. The pattern or list which will receive such a command is said to have ***edit focus***. By default, *edit focus* follows the playing pattern, but the *edit focus* can be set to a different pattern than the one that is playing.

## Changing the playing pattern

When the sequencer is running, type the number of the pattern that you want to play and the pattern will change when the beat reaches the end of the current Ableton Link *quantum*.

If  ***pattern chain*** mode is active, the number will be interpreted as a stage in the pattern chain, and new stage will become active when the beat reaches the end of the current *quantum*. This overrides the stage that would otherwise play and is useful, for instance, when you want to jump away from a chain stage that is marked for indefinite repeat.

This is the one case where you use the command line without typing a command.

---



```
{"help", C_HELP},





{"edit", C_EDIT},
{"e", C_EDIT},
{"cue", C_CUE},
{"play", C_CUE},
{"new", C_NEW},
{"copy", C_COPY},
{"delete", C_DELETE},
{"del", C_DELETE},
{"clear", C_CLEAR},
{"undo", C_UNDO},

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

{"tran", C_TRANSPOSE},
{"transpose", C_TRANSPOSE},

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
