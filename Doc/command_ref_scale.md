# Multi Arp Command and Menu Reference: Scale and Transpose

**BN - Jan 04, 2018**

Scale mappings and transpositions are handled within the sequencer using a thing called a *translation table*.

Each pattern has one *translation table*, and there's also a global table that you can use if you don't want to use the current pattern table. You switch between the tables with the [use](command_ref_use.md#use) command.

The *scale* commands here have been largely superseded by the [Scale Menu](menu_ref_scale.md), but here they are nonetheless.

* [**root**](#root)
* [**scale**](#scale)
* [**scale from list**](#scale-from-list)
* [**scale show**](#scale-show)
* [**scale controls**](#scale-controls)
* [**transpose**](#transpose)

## root

Synonyms: (none)

Parameters: ***C, C#, Eb, C5, F#6, etc.***

Sets the root note for the current *translation table*. This is used in conjunction with the scale parameter set in the [Scale Menu](menu_ref_scale.md).

---
## scale

Synonyms: ***s***

Parameters: (none)

This command opens the [Scale Menu](menu_ref_scale.md) for either the current pattern or the global settings.

(To see which settings are being used, global or pattern, see the [**use**](command_ref_use.md#use) command.)

---
## scale from list

Synonyms: (none)

Parameters: (none)

Experimental feature (which may no longer work) which sets the current scale according to the notes in current *step list*. Notes are mapped onto a single octave and the default root note is C.

The idea behind this is to allow user defined scales and then have transpositions restricted to notes in that scale.

---
## scale show

Synonyms: (none)

Parameters: (none)

Shows the current interval list and note map used for transpositions. For a major scale the output looks like this:
```
I: 2, 2, 1, 2, 2, 2, 1     Map: I . II . III IV . V . VI . VII
```
The *interval list* lists the steps of the scale, in semitones. The *interval list* for a major scale looks like this:
```
[ 2, 2, 1, 2, 2, 2, 1 ]
```
When carrying out a (non-chromatic) transposition the *note map* give us the *degree* - I, II, III, etc. - in the scale of the note we're working on. This is the zero based index of the note within the *interval list*. For a major scale, the *note map* looks liks this:
```
[ 0, 0, 1, 0, 2, 3, 0, 4, 0, 5, 0, 6 ]
```
As an example, say you have three notes, C, E and F, and you want them transposed two steps upwards upwards with all notes staying in C major. Taking each note at a time, read the index of the note from the note map to get its position in the interval list. Take this value and add the next entry to it to give us the total shift in semitones for each note. For C, the numbers are 2 + 2 = 4, for E we have 1 + 2 = 3, and for F it's 2 + 2 = 4 again. If the transpose was three steps, or *degrees*, then you'd add three intervals starting at a note's index in the interval list.

You can probably see there's a problem with this approach. Things work well for notes that sit within a particular scale, but what to do with those that don't?  (These are referred to as *accidentals*.) What do you do, for instance, with Eb, or F#? You will see that those entries are set to zero in the *note map*. This tells the *translation table* that it needs to do something different with these notes. I've set up four possible ways of handling *accidentals* and which particular method is used depends on [Accidentals Mode](menu_ref_scale.md#accidentals-mode) in the scale menu.

(By the way, I've begun referring to this kind of non-chromatic transpose as a *tonal shift*, but I've no idea whether that's a valid thing to do.)


---
## transpose

Synonyms: ***tran***

Parameters: ***n [now]***

Adds a 'brute force' transpose value, in semitones, to notes as they play. The transponse value is added after any changes made by the *translation table*.

The new transpose value will be applied when the sequencer completes that current *quantum*. To override this add *now* to the command.
