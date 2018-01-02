# Multi Arp Command and Menu Reference: Trig List Menu

**BN - Jan 01, 2018**

A *pattern* can have one (and only one) trigger list which can be used to control the order in which *step lists* are used when the pattern plays.

(If no trigger list is set up, the pattern just cycles through all step lists in turn, playing one step from each step list for each step of pattern. I may add other 'automatic' modes as we go along.)

#### Trigger List Menu
![](Screenshot_TrigList_Menu.png)

The Trigger List menu lets you edit the *Trigger List* for the current pattern. You can open the menu from the top level [*Pattern Menu*](menu_ref_pattern.md), or by using the [*trigs*](command_ref_trigs.md#trigs) command.

A trigger list consists of one or more *trigger stages* that specify which *step list(s)* are to be used for each stage. An indiviual trigger stage can be set to be skipped or muted, and can also vary the step length of the sequencer as a whole. It can also trigger additional notes for repeat, delay and arpeggio effects that continue after the current step of the sequencer has completed.

There is no limit to the number stages in a trigger list.

#### Trigger List progress panel

There is also a trigger progress panel, which appears half way down the main screen. This shows a summary of each stage with a highlight bar showing the active stage as the pattern plays. (Although this also looks like it could be a menu, it isn't.)

![](Screenshot_TrigList_Progress_Summary.png)

The panel shows eight stages at at time spread evenly across the screen. In the case that there are more than eight stages in the trigger list, a crude underline progress indicator grows across the panel at the top, showing overall progress. (This tops out at 64 steps, and I haven't thought about what to do after that.)

### Tirgger List Menu Items

* [**Trig Stage 1**](#trigger-stage)
* [**Trig Stage 2**](#trigger-stage)
* [**...**](#trigger-stage)

Acitve key combinations in the Trigger List menu:

|     | Action     |
| :--- | :--- |
| **enter** | Opens the Trigger Stage submenu for the highlighted stage. |
| **backspace** | Returns to Pattern Menu menu (only if opened from the Pattern Menu).|
|**shift-left/right**|Adds a new, empty trigger stage to the left or right of the current, highlighted stage.|
|**ctrl-left/right**|Adds a copy the highlighted trigger stage and places it before or after the current stage.|
|**shift-del**|Deletes the highlighted item. There is no *undo* option for this operation.|


## Trigger Stage

![](Screenshot_TrigList_Item_Menu.png)

#### Trigger Stage Menu Items
* [**Triggers**](#triggers)
* [**Multiplier**](#multiplier)
* [**Skip**](#skip)
* [**Mute**](#mute)
* [**Repeat**](#repeats)
* [**Repeat Time**](#repeat-time)
* [**Decay Mode**](#decay-mode)
* [**Velocity Decay**](#velocity-decay)
* [**Arpeggio Stages**](#arpeggio-stages)

Acitve key combinations in the Trigger Stage menu:

|     | Action     |
| :--- | :--- |
| **enter**, **backspace** | Returns to Trig List Menu menu.|
|**up/down**|Change value of the current menu item. For decimal fields, the values change in steps of 0.1. For *arpeggio-stages*, this changes the *interval* value.|
|**shift-up/down**|For decimal fields, changes the value in steps of 0.01. For *arpeggio-stages*, changes the *step count*.|
|**shift-left/right**|Adds a new *arpeggio stage* to the left or right of the current, highlighted stage.|
|**shift-del**|Deletes the highlighted *arpeggio-stage*. There is no *undo* option for this operation.|



## Triggers

Sets which *step lists* are to be triggered for this stage.

Use the **up/down** cursors keys to scroll through combinations: 1, 2, 1+2, 3, 3+1, 3+2, 3+2+1, 4, etc. (Yes, this can get tedious, but I haven't implemented anything better.)  

There are two additional values, **all** and **none**, which appear at the start  of the list. (These should be self-explantory.)

There are no audits to check whether values entered here correspond to actual step lists. If you choose a step list that doesn't exist, or maybe is removed at some point later on, then the value is just ignored.

## Multiplier

Adjusts the length of step *for the entire sequencer* for this stage in the trigger list. Use the **up/down** cursor keys to change the value in steps of 0.1. (Use **shift-up/down** to change the value in steps of 0.01.)

## Skip

Sets this stage to be skipped with processing moving immediately to the next stage in the trigger list.

(If all stages are set to skip the sequencer does nothing for each step.)

## Mute

Sets this stage to be muted. Nothing is triggered for the this stage in the list. Processing moves to the next stage when the next step of the pattern comes around.

## Repeats

Sets the number of repeats for this stage. If set to **off**, nothing is repeated, you just hear the notes from the step lists that are being triggered. A value of 1 means one repeat, 2 means two repeats, and so on.

The speed of repeats is controlled by [Repeat Time](#repeat-time), below.

## Repeat Time

If repeat time is set to **off**, repeats are spread evenly within the duration of the current step. This can be used to quickly generate unusual subdivisions within each step of the pattern.

Any other value here is used as a beat value which will play back according to the sequencer's current tempo. For instance, a setting of 4 *repeats* with a *repeat time* 0.25 will generate four additional sixteenth notes that play after this stage in the trigger list, regardless of when the next step in the pattern is due to fire.

Use the **up/down** cursor keys to change the value in steps of 0.1. Use **shift-up/down** to change the value in steps of 0.01.

## Decay Mode

Velocity Decay is a mechanism which reduces note velocity of successive repeats. There are five decay modes:

* **off:** there is no reduction in velocity.

* **linear:** a constant value, ***Start Velocity x (1 - Velocity Decay)***, is subtracted from note velocity for each repeat. The note will be repeated until velocity gets to zero or the number of repeats specified by ***Repeats*** is reached.

* **exponential:** note velocity multiplied by ***Velocity Decay*** for each repeat. The note will be repeated until velocity gets to zero or the number of repeats specified by ***Repeats*** is reached.

* **linear-unlimited:** same as *linear* but number of repeats is ignored (unless repeats are ***off***). The note will be repeated until velocity gets to zero.

* **exponential-unlimited:** same as *exponential* but number of repeats is ignored (unless repeats are ***off***). The note will be repeated until velocity gets to zero:

***Notes***

At the moment the two *unlimited* modes will operate even if ***Repeats*** is set to ***off***.

It's possible to generate lots of midi events using this mechanism and this may lead to (what I think is) the ALSA Sequencer queue filling up. If this happens, the sequencer will freeze for a few seconds (and continue to freeze unless you reverse whatever change you made that generated all those events).

The *exponential* modes generate a more natural sounding decay, but will generate many more quiet notes which will probably never be heard. With the *linear* modes, the initial repeats will be louder and fade away more quickly. This may be preferable aesthetically and is certainly safer when preventing overloads.

Also with a view to preventing too many notes, both *unlimited* modes are ignored if ***Velocity Decay*** is set to 1.00 or above.

## Velocity Decay

Sets the rate of decay for each of the ***Decay Modes***.

If decay mode is *exponential*, this value is used as a multiplier for each step. For example, if starting velocity is 64 and decay is set to 0.8, the velocity of the first repeat will be *0.8 x 64*, the second repeat will be *0.8 x 0.8 x 64*, and so on.

If decay mode is *linear*, this value is used to create a decrement value, *D*, which is subtracted from velocity on each step. Starting with the same values as the last example, the decrement is calculated as *D = 64 x (1 - 0.8)*, which comes out as 12.8. So the velocity of the first repeat will be *64 - 12.8*, the second *64 - 12.8 - 12.8*, and so on. (I used this approach, rather than using the decay value directly, as it makes is simpler to change between linear and exponential modes without having to set new decay value each time.)

For the *linear* modes, *Velocity Decay* can be set to values greater than 1.0. This will generate repeats that get louder (though velocity cannot move beyone 127, the midi limit). If you attempt to set a value greater than 1.0 when one of the *unlimited* modes is active, the setting will be ignored and repeats will be disabled temporarily.

## Arpeggio Stages

It's possible to set up note shifts for repeating notes to generate arpeggio effects. The *Arpeggio Stages* menu is not immediately visible, but when activated it appears as an extention to the right hand side of the ***Trigger Stages*** menu.

![](Screenshot_TrigList_Item_Arp.png)

The image above shows three *Arp Stages*. Each stage has an *interval* and *step count*. The *interval* determines how big a step to add to note number for each step, and *step count* determines how many steps to add *interval* before resetting to zero. *Interval* is applied according to the current ***scale settings*** and is more than a simple chromatic shift.

The current interval from all stages is added to note number, but only the first stage will be stepped for every repeat. When it reaches its *step count* it resets to zero and the second stage will be stepped just once and the first stage loops again. This continues until the second stage reaches its own *step count*, in which case the third stage will be stepped once. This process continues until ***Repeats*** is reached or note velocity gets to zero.

So, in the example above, the first repeat will be two scale degrees above the original note, the second repeat four scale degrees above. However, the third repeat will be just one degree above because the first stage has reset and the second stage has now come into play. The fifth repeat will be three degrees above, the sixth will be five above. And so on ...

You can have as many stages as you like, but it's probably not worth having more than three or four as things can get very complicated very quickly.

You create the first stage by pressing **shift-left** or **shift-right** with the menu highlight anywhere in the *Trigger Stage Menu*. After that, these key combinations will only work if an existing stage is highlighted.
