# Multi Arp Command and Menu Reference: Triggers

**BN - Dec 17, 2017**

* [**trigs**](#trigs)
* [**trigs arp**](#trigs-arp)

## trigs

Synonyms: ***t***

Parameters: ***all L1 L2 L3 L1+L2 ...***

Typing ***trigs*** followed by a list of *step list IDs* will set up a new trigger list. Each parameter, separated by spaces, will set up an addifional trigger stage. Each stage tells the sequencer which *step list* to use for the next step. You can specify just one list, any combinations of lists separated by the '+' symbol, or the word *all* as a short cut to use all lists for a given stage.

To just open the [Trig List Menu](menu_ref_trig_list.md) don't give any parameters.

There is no *undo* when using this command, so make a copy of the pattern if you're not ready to loose current settings.

As an example, in a pattern containing three *step lists*, the following command will set up a list which uses step list 1 for the first and third stages, and alternates between 2 and 3 for the second and fourth stages:
```
trigs 1 2 1 3

```

A more complicated example would be to use list 1 for all stages but with different combinations of lists 2 & 3 on stages 2, 3 and 4:
```
trigs 1 1+2 1+3 all

```

---
## trigs arp

Synonyms: ***t arp***

Not implemented. Was a placeholder to set up some hard coded test data, but I've turned it off now that we have the Trigs Menu. It may come back at some point.


## **template**

Synonyms: ***syn 1***, ***syn 2***

Parameters: ***param 1***, ***n.nn***

Describe command and parameters here. Fairly freeform.
