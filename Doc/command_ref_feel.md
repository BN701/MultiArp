# Multi Arp Command and Menu Reference: Feel and Swing

**BN - Jan 04, 2018**

A *feel map* takes any steps that fall between whole beats and moves them to create simple (and not so simple) swing effects.

A *feel map* has one or more entries which divide the incoming beat time into two or more *windows*. Incoming windows are evenly spaced, equally sized. Outgoing windows are stretched according to the values in the *feel map*. Times will be adjusted according to where fall in the incoming window and the relationship between incoming and outgoing windows.

A map with one entry will look like this:
```
0.00 0.50 1.00
```
When the sequencer step value is eight, increasing the value of the middle entry will create a simple 'jazzy' swing effect. However, if step value is sixteen the timing of second *and* third steps in each beat will be stretched, but the fourth will be shortened.

If you set up a *feel map* with four entries, you can move the swing of all four sixteenth notes. And if you set up with three entries and a step value of twelve you can play with the kinds of 12/8 swing found in some West African rhythms.

You can add as many entries as you like to the *feel map*, though you'll have to run the sequencer at a very slow tempo to make sense of swing effects with notes smaller than a sixteenth note.



---
#### Commands

* [**feel**](#feel)
* [**feel on, off**](#feel-on-off)
* [**feel new**](#feel-new)
* [**feel add**](#feel-add)
* [**feel remove**](#feel-remove)
* [**feel respace**](#feel-respace)

## feel
Synonyms: ***f***

Parameters: (see below)

The *feel* command on its own opens the [Feel Menu](menu_ref_feel.md) for the current pattern or for global settings buffer, depending on the current [*use*](command_ref_use.md#use) setting.


---
## feel on, off

Turns the current *feel map* on or off.

---
## feel new

Parameters: ***N***, or ***0.nn 0.nn ...***

Initializes a new *feel map* map with *N* evenly spaced entries or, if the first parameter is a decimal value less than 1, will create a map using that and the following entries.

*new* on its own will clear the current list and create and empty list with entries at 0.00 and 1.00. There is no *undo* on this operation.

---
## feel add

Adds an entry to the current *feel map*. The new entry is added after the currently highlighted entry, and the value is set at half way between this and the next entry in the map.

(Use the [Feel Menu](menu_ref_feel.md) for much greater control when editing the *feel map*.)

---
## feel remove

Removes the currently highlighted entry.

---
## feel respace

Resets values of each point so that they are evenly spread between 0 and 1.
