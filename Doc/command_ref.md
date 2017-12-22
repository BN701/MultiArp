# Multi Arp Command and Menu Reference: Home

**BN - Dec 22, 2017**

This is an attempt to document things **as they are** right now. A snapshot of a rambling work in progress, with all its inconsistent design choices and unplanned thoughts sprawling across the weeks and months like a garden full of weeds. As much as anything I'm using this exercise as a chance to review things as a whole. Please do not judge me on what you may find here!

*22-Dec-17*

#### Sections

* [**Using the command line**](#using-the-command-line)

* [**Changing the playing pattern**](#changing-the-playing-pattern)

* [**General Commands**](command_ref_misc.md)

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


* [**Pattern Commands**](command_ref_pattern.md)

  * [edit](command_ref_pattern.md#edit)
  * [play](command_ref_pattern.md#play)
  * [new](command_ref_pattern.md#new)
  * [copy](command_ref_pattern.md#copy)
  * [clear](command_ref_pattern.md#clear)
  * [**delete**](command_ref_pattern.md#delete)
  * [**undo**](command_ref_pattern.md#undo)
  * [**lock**](command_ref_pattern.md#lock)

* [**Pattern Chains**](command_ref_chain.md)

  * [chain](command_ref_chain.md#chain)
  * [chain quantum, natural, off](command_ref_chain.md#chain-quantum-natural-off)
  * [chain clear](command_ref_chain.md#chain-clear)
  * [chain new](command_ref_chain.md#chain-new)
  * [chain delete](command_ref_chain.md#chain-delete)
  * [chain jump](command_ref_chain.md#chain-jump)


* [**Step List**](command_ref_step_list.md)

  * [list](command_ref_step_list.md#list)



* [**Real Time List**](command_ref_realtime_list.md)

  * [import](command_ref_realtime_list.md#import)
  * [real time](command_ref_realtime_list.md#real-time)
  * [real time delete](command_ref_realtime_list.md#real-time-delete)
  * [real time rate](command_ref_realtime_list.md#real-time-rate)
  * [real time quantum](command_ref_realtime_list.md#real-time-quantum)
  * [real time phase](command_ref_realtime_list.md#real-time-phase)
  * [real time echo](command_ref_realtime_list.md#real-time-echo)


* [**Triggers**](command_ref_trigs.md)

  * [trigs](#trigs)
  * [trigs arp](#trigs-arp)



* [**Scale and Transpose**](command_ref_scale.md)

  * [root](command_ref_scale.md#root)
  * [scale](command_ref_scale.md#scale)
  * [scale from list](command_ref_scale.md#scale-from-list)
  * [scale show](command_ref_scale.md#scale-show)
  * [scale controls](command_ref_scale.md#scale-controls)
  * [transpose](command_ref_scale.md#transpose)

* [**Feel and Swing**](command_ref_feel.md)

  * [feel](command_ref_feel.md#feel)
  * [feel on, off](command_ref_feel.md#feel-on-off)
  * [feel new](command_ref_feel.md#feel-new)
  * [feel add](command_ref_feel.md#feel-add)
  * [feel remove](command_ref_feel.md#feel-remove)
  * [feel respace](command_ref_feel.md#feel-respace)


* [**Global Settings**](command_ref_use)

  * [load](command_ref_use#load)
  * [store](command_ref_use#store)
  * [use](command_ref_use#use)


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






{"misc", C_HELP_1},
{"pattern", C_HELP_3},
{"control", C_HELP_2}
};
```
