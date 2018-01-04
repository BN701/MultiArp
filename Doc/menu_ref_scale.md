# Multi Arp Command and Menu Reference: Template Menu


**BN - Jan 04, 2018**

![](Screenshot_Scale_Menu.png)

This menu shows settings for the active *translation table*.

Each pattern has *translation table* which defines a root and scale - *key* - for any transposes applied to the pattern. There is also a global table if you don't want to worry about individual patterns. (See the [use](command_ref_use.md#use) for more information.)

You open this menu with the [Scale](command_ref_scale.md#scale) command. The only key combinations active on this menu are **up/down/left/right** for changing field and value. (The usual *enter/backspace* keypresses to return to a previous menu are not present.)

Please read the [Scales Reference](command_ref_scale.md) page for more background and other details that I may not have included here.

### Menu Items

* [**Chromatic Transpose**](#chromatic-transpose)
* [**Tonal Transpose**](#tonal-transpose)
* [**Root**](#root)
* [**Scale**](#scale)
* [**Premap Mode**](#premap-mode)
* [**Accidentals Mode**](#accidentals-mode)

---

## Chromatic Transpose

Adds a 'brute force' transpose value, in semitones, to notes as they play. The transponse value is added after any changes made by the *translation table*.

---
## Tonal Transpose

Sets the number of steps to apply in the current *tonal transpose*.

A *tonal transpose*, or *tonal shift*, attempts to transpose the pattern's note data whilst staying within the current key, as defined by *root* and *scale*.

See the [**scale show**](command_ref_scale.md#scale-show) command for more background on this.

---
## Root

Sets the *root note* of the current scale.

---
## Scale

Sets the current scale. Available options are:

* **Major**
* **Natural minor**
* **Harmonic minor**
* **Major dorian**
* **Major phrygian**
* **Major lydian**
* **Major mixolydian**
* **Major locrian**
* **Major pentatonic**
* **Minor pentatonic**
* **Major blues**
* **Minor blues**


---
## Premap Mode

Values:
* **off**
* **mute**
* **leave**
* **upper**
* **lower**

Maps notes onto the current scale before any other transpose operarations are applied.

***Off*** means that no mapping takes place. The other four modes control what happens when a source note is a diminished fifth. Details follow ...

In all modes, a source note is mapped into the octave above the current root. The semitone value can then be used to decided whether it's a second, third, fourth, etc and, if the doesn't already fit onto the current scale, the semitone value of the original note will be nudged up or down accordingly.

However, there's one situation where these rules cannot be applied automatically. If the souce note is six semitones above root - a *diminished fifth* - we don't always know where to place it in the destination scale.

If the destination scale is *lydian*, which has a diminished fifth and a fifth, then we'll move the incoming note up to make way for any natural fourths which will need to become diminished fifths. Similarly, if the destination scale is *locrian*, which as no natural fifth, we move the incoming note down a natural fourth to make way for any natural fifths which will themselves become diminished.

In other cases though, there's nothing there's no automatic choice. So the options here control what happens. *Mute* means the note doesn't sound; *Leave* means the note sounds but isn't adjusted; *upper* and *lower* nudges the note up or down a semitone, respectively. The safe option is obviously *mute*, but if you're loosing too many notes, try one of the other options to see which works best.

---
## Accidentals Mode

* **upper**
* **lower**
* **upper, mute if clash**
* **lower, mute if clash**

If you read over the section on [**scale show**](command_ref_scale.md#scale-show), you will see we have a question surrounding how to handle *accidentals* - notes that don't fall naturally within the current scale - when performing a *tonal transpose*.

This option is where you choose how *accidentals* are handled. *Upper* and *lower* modes mean the note tracks either the nearest scale note above or below it, respectively. The two *mute* options behave in the same way with an additional check to see if there's likely to already be a note at that position in the scale.
