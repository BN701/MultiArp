# Multi Arp Command and Menu Reference: Real Time List


**BN - Dec 17, 2017**

* [**import**](#import)
* [**real time**](#real-time)
* [**real time delete**](#real-time-delete)
* [**real time rate**](#real-time-rate)
* [**real time quantum**](#real-time-quantum)
* [**real time phase**](#real-time-phase)
* [**real time echo**](#real-time-echo)

## import

Synonyms: ***i***

Parameters: ***pathname***

Imports a midi file to the pattern which has *edit focus*. Each track in the midi file will be set up on a new *real time list*. The loop length, or [*real time quantum*](#real-time-quantum),  for the all lists will be set to the same value, and is decided by final note of the longest track.

---
## real time

Synonyms: ***rt***

Parameters: (see below)

Typing ***real time*** without any parameters opens the *real time menu* for the current real time list.

**Note -** You are only able to browse between lists if you open the *real time menu* as a submenu of the *edit menu*.

---
## real time delete

Synonyms: ***rt delete***, ***rt del***

Parameters: (none)

Deletes the *real time list* which currently has *edit focus*.

WARNING: There is no *undo* on this operation. If you are concerned about losing data, make a copy of the pattern before making changes.

---
## real time rate

Synonyms: ***rt rate***, ***rt r***

Parameters: ***r.rr***, [***i.ii***]

Sets the rate *multiplier*, or *playback rate*, for **all** *real time lists* in the current pattern. If the second parameter *i.ii* is given, it will be added to *r.rr* for each additional list in the pattern.

For instance, the following command will set the playback rate of the first list to 1.00 - i.e. normal speed - the second list to 1.01, the third to 1.02, and so on:

```
real time rate 1.00 0.01

```

---
## real time quantum

Synonyms: ***rt quantum***", ***rt q***

Parameters: ***q.qq***

Sets *real time quantum* for all lists in the current pattern to the same value. Use the [Real Time Menu](#menu_ref_realtime_list.md) to set this value individually for each list.

*Real time quantum* is just Ableton Link speak for loop length. When used in combination with start time you can loop over just a smaller section of the list. You can change start time for individual lists using the [Real Time Menu](#menu_ref_realtime_list.md), but I probably need to add a function here to set for all lists at once.

---
## real time phase

Synonyms: ***rt phase***, ***rt p***

Parameters: ***n.nn%***

Sets a starting beat value according to the phase of the combined real time list set. e.g. If it takes all lists 40 beats to get back to all being at phase zero, specifying 50% gets you to beat 20. The beat value is stored so that the pattern begins at this point each it starts or reset.

This function is only useful if playback rates are set differently for each list and, maybe, set to create interesting phasing relationships. It gives you the option of starting at any point in the overall phase cycle, rather than just beginning at zero.


---
## real time echo

Synonyms: ***rt echo***, ***rt e***

Parameters: ***incrememt n.nn target t.tt interval i***

This is an even more experimental feature than the last, and should perhaps be called *real time drift*, or *event cascade*. It is also unfinished and important parts of it are still hard coded at time of writing. (I had to stop work on it part way through.) Here's the gist:

* A note has an increment value (not a multiplier) which is used to update its phase value (beat position on the timeline).

* All notes are updated once per list cycle when list phase hits zero.

* A note also has a target phase value which, when reached, will trigger some other event(s). Initially I thought target would expressed as a percentage of the overall pattern phase, but entering it in terms of list phase, which is just a beat value, is just as effective and much simpler. E.g. target 1.5 just means target is reached one and a half beats behind its starting position. What happens at this point is discussed below.

* All notes in a list, captured or imported, start off with zero increment. We can use this as a flag to indicate which notes in a list are ‘active’ and need processing.

* Processing is initiated with this command, *real time echo*, which creates copies of all notes with zero increment and ‘activates’ them with parameters supplied with the command.

* As notes reach the end of the real time quantum they currently wrap to the beginning. Alternatively, notes could be deleted, or left to continue moving. In the latter case, quantum can be increased to ‘reveal’ them, either manually or automatically.

* A note also has an interval parameter which can be used to offset new notes. This should be tied into the pattern’s Translate Table so that current scale rules are used.

* When a target is reached, any number of things can happen, and I’m probably going to have to begin by hard coding a number or different scenarios:
 * The note can be deleted.
 * The note can be frozen in its new position (increment cleared).
 * The note increment can be cleared, or set to a new value.
 * New note(s) can be created with new values.
 * The BeginEcho() routine can be called so that all inactive notes can be echoed again, with new parameters, or parameters adapted from the 'parent' note.
