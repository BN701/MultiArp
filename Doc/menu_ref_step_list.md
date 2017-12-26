# Multi Arp Command and Menu Reference: Step List Menu


**BN - Dec 17, 2017**

You reach the *Step List Menu* from the Pattern Menu.

There are two more levels to descend in the menu hierarchy before you  reach note data - pitch, velocity, note length - so the situation here isn't very usable. (Worse case of menu diving I've seen in a long time!) However, the mechanism does at least exist so here I'll do my best to describe it.

### Step List Menu Levels

* [**Step List**](#step-list)
* [**Step List Step**](#step-list-step)
* [**Note Edit**](#note-edit)

---
## Step List

![](Screenshot_Step_List_Menu.png)

The ID of the current *step list* is shown on the left, and to the right of this are are the contents of each step, which can either be a single note or a group of notes. From here you can browse steps, add, copy or delete them.

#### Key combinations

||Action|
| :--- | :--- |
| **up/down** |Browse to the next or previous step list in the pattern. This works on any entry (though should perhaps just be active on the step list ID, which isn't currently selectable.)|
| **enter** | Opens the [Step List Step](#step-list-step) menu for the highlighted step.|
| **backspace** | Returns to [Pattern Menu](menu_ref_pattern.md).|
|**shift-left/right**| Insert an empty step to the left or right of the highlighted step.|
|**ctrl-left/right**| Copy the highlighted step and place the copy to the left or right of the current step.|
|**shift-delete**| Delete the current step. WARNING: There is no *undo* with this operation.|

---

## Step List Step

![](Screenshot_Step_List_Step_Menu.png)

The ID of the current *step* - for reasons best known to the author here referred to as a *cluster* - appears on the left. To the right are the notes that are played for this step, either a single note or a chord.

#### Key combinations

||Action|
| :--- | :--- |
| **up/down** |Browse to the next or previous step in the list. This works on any entry (though should perhaps just be active on the step ID, which isn't currently selectable.)|
| **enter** | Opens the [Note](#note) menu for the highlighted note.|
| **backspace** | Returns to [Step List Menu](#step-list).|
|**shift-left/right**| Insert an empty note to the left or right of the highlighted step.|
|**ctrl-left/right**| Copy the highlighted note and places the copy to the left or right of the current step.|
|**shift-delete**| Delete the current note. WARNING: There is no *undo* with this operation.|

---
## Note Edit

![](Screenshot_Note_Menu.png)

The ID of the current note appears on the left of the menu but, unlike the previous two levels, you have to return to the previous level to choose a different note to edit.

#### Key combinations
(The following key combinations apply to all fields.)

||Action|
| :--- | :--- |
| **up/down** |Change the value of the current field.|
| **enter**, **backspace** | Returns to [Step List Step Menu](#step-list-step).|

#### Fields
|Name||
| :--- | :--- |
|**Note**|Note name and octave. A single hyphen indicates and empty note, which acts as a rest. You can use **ctrl-up/down** to change octave without changing note name.|
|**Vel**| Midi note velocity, which has the range 1..127. If set to ***off***, the setting below '1', then the pattern's default velocity, or the sequencer's default velocity will be used when playing the note.|
|**Len**| Note length in beats and fractions of a beat. If set to ***off***, the pattern's or sequencer's default *gatelength* will be used to calculate note duration. |
