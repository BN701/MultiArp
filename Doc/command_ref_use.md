# Multi Arp Command and Menu Reference: Global Settings

Settings for *step, gate, velocity* and *scale* can be stored globally, for use by all patterns, or with each pattern. The three commands here control which settings are in use at any given time, and enable settings to be copied between the global buffer and individual patterns.

**BN - Dec 17, 2017**

* [**load**](#load)
* [**store**](#store)
* [**use**](#use)

## store

Synonyms: (none)

Parameters: ***step, gate, vel, scale, all***

Copies global values for *step, gate, velocity* or *scale* to the pattern that currently has *edit focus*.

## load

Synonyms: (none)

Parameters: ***step, gate, vel, scale, all***

Copies values for *step, gate, velocity* or *scale* from the pattern that currently has *edit focus* to the global settings buffer.

## use

Synonyms: (none)

Parameters: ***globals, g, pattern, p***

Controls whether settings for *step, gate, velocity, scale* and *feel* are taken from the *global* settings buffer, or from whichever *pattern* is playing.

(Note that there is a global *feel map* and also a pattern *feel map*. The *use* command will switch between them but at the moment *store/load* is not implemented for feel data.)

Typing *use* without any parameters shows the current status, either **P:** or **GLOB:** with a summary of current settings.

When editing these values, the *use* setting controls whether global or pattern values are edited. In this case though, if use is set to *pattern* it's the pattern which has *edit focus* which receives the updates.
