# Multi Arp Command and Menu Reference: Step List


**BN - Dec 17, 2017**

* [**list *nn***](#list-nn)
* [**list new**](#list-new)
* [**list delete**](#list-delete)
* [**list *nn* clear**](#list-nn-clear)


## list *nn*

Synonyms: ***l***

Parameters: ***nn*** ***note***, [***note n***]

Use ***list*** with just a list number, ***nn*** (starting from 1), to set *edit focus* to this list and display its contents.

Use ***list nn*** with a *note list* to set or replace the contents of the list. If list *nn* doesn't exist, it will be created. You do not have to use the ***new*** command first.

A *note list* is a set of notes separated by spaces and commas. If notes are separated by spaces, they represent a chord on one step in the list. Use a comma to separate each step on the list.

For instance, following entry sets up a list with four steps, including a three-note chord on the second step:
```
list 1 C4, D4 F4 Bb4, C4, Eb4
```
***Notes***

When adding notes with *list nn*, if *nn* is more than one entry beyond the end of the step list set, new empty lists will be created to fill the gap. e.g. If a pattern has three lists and you create list 6, empty lists for entries 4 and 5 will be created as well.

Similarly, if you use this command when there are no patterns in the sequencer, a new empty pattern will be created to hold the first list.

You can also set *edit focus* and change *step list* contents using the [Step List Menu](menu_ref_step_list.md). At the moment there is no command shortcut for opening the step list menu. You have to open the Pattern Menu and type *enter* after selecting a *step list*. This is an omission.

The fastest way you can set up step lists is using a midi keyboard attached to the PC. See [Midi Note Entry](readmore.md#midi-note-entry) for more details on this.

---
## list new

Synonyms: (none)

Parameters: (none)

Creates an empty *step list* and adds it to the end of the list. Notes can be added with a *list nn* command, from the [Step List Menu](menu_ref_step_list.md), but the fastest way to add notes to a step list is with a midi keyboard attached to the PC. See [Midi Note Entry](readmore.md#midi-note-entry) for more details on this.

---
## list delete

Synonyms: (none)

Parameters: (none)

Deletes the step list which currently has *edit focus*.

Currently *delete nn* to delete a specific step list without changing *edit focus* hasn't been set up. This is an omission.

WARNING: There is no confirmation and no undo with this command. If you are concerned about losing important note data, make a copy of the current pattern before making any changes.

---
## list *nn* clear

Synonyms: (none)

Parameters: ***nn***

Clears the notes from a *step list* but doesn't delete the list itself.

Example:

```
list 1 clear
```
