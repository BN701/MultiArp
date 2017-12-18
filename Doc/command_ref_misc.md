# Multi Arp Command and Menu Reference: Misc


**BN - Dec 17, 2017**

* [**autoreset**](#autoreset)
* [**label**](#label)
* [**midi**](#midi)
* [**page**](#page)
* [**quantum**](#quantum)
* [**quit**](#quit)
* [**reset**](#reset)
* [**run/stop**](#run-stop)
* [**status**](#status)
* [**step**](#step)
* [**velocity, gate**](#velocity-gate)

## **autoreset**

Synonyms: (none)

Parameters: ***on***, ***off***

Turning on *autoreset* means that whenever a new pattern starts playing its list positions will start from the beginning each time.

If turned off, a pattern will pick up playing from where it was interrupted the last time is was playing.

---

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

## quantum

Synonyms: ***q***

Parameters: ***n.nn***

Sets the Ableton Link *quantum*. This is effectively loop length for the entire sequencer. The new quantum value will take effect when the current quantum ends.

This from the [Ableton Link Dev Guide](https://ableton.github.io/link/):

*"In order to enable the desired bar and loop alignment, an application provides a quantum value to Link that specifies, in beats, the desired unit of phase synchronization. Link guarantees that session participants with the same quantum value will be phase aligned, meaning that if two participants have a 4 beat quantum, beat 3 on one participant’s timeline could correspond to beat 11 on another’s, but not beat 12. It also guarantees the expected relationship between sessions in which one participant has a multiple of another’s quantum. So if one app has an 8-beat loop with a quantum of 8 and another has a 4-beat loop with a quantum of 4, then the beginning of an 8-beat loop will always correspond to the beginning of a 4-beat loop, whereas a 4-beat loop may align with the beginning or the middle of an 8-beat loop."*

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

## status

Synonyms: ***stat***

Shows a quick summary of global parameters:

* Number of patterns loaded in the sequencer
* Whether *edit lock* on or off
* Current default *velocity* and *gate*

This is an early command and has been mostly superceded by ***use***, which also shows default *velocity* and *gate* settings, along with current *scale* settings and whether these values are taken from the current pattern of the defaul global settings.

---
## step

Synonyms: ***note***

Parameters: ***n.nn***

Sets the ***step value*** for the sequencer. The value is stored globally or with the pattern that has *edit focus*, depending on whether ***use*** is set to *global* or *pattern*.

From the Concepts section: The ***step value*** controls the rate of step list playback and indeed the overall ‘pulse’ of the sequencer. Step value is the conversion rate between the internal sequencer pulse and Ableton Link’s beat value. A value of 4 is essentially a quarter note, 8 is an eighth-note and so on, and values 3, 7, etc divide a four beat bar as you would expect. Step values (and beat values) can be non-integer.

---

## velocity, gate

Sets default velocity and gate length for notes that do not have these values set individually.

The values are stored globally or with the pattern that has *edit focus*, depending on whether ***use*** is set to *global* or *pattern*.

#### velocity
Synonyms: ***vel***

Parameters: ***n***

Velocity needs to be an integer in the range 1 .. 127.

#### gate

Synonyms: (none)

Parameters: ***nn.n%***, ***hold***, ***h***, ***normal***, ***n***

Gatelength is set as a percentage of current beat step length. e.g. 50% means the gate will be held open for half of each step.

If a note is followed be one or more rests, turning on ***gate hold*** means that note will be held open until the final rest, at which point normal gatelength is used. Turn this feature off with ***gate normal***.

#### Notes
* You can set a gatelength of more than 100%, in which case notes will last proportionally longer than a single step. Beware, though, that repeated notes of the same pitch may not work as expected. When a note is put in the ALSA Sequencer queue, its note off event will be placed in the queue at the same time. If the note off event is scheduled to happen *after* the next note begins, the first note's note-off will close down the second note prematurely. (I have a to-do item to clear existing note-off events when a new note-on arrives.)

* *Gate hold* only works for notes that do note have gatelength set individually.
