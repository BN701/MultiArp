# Multi Arp Command and Menu Reference: Feel Menu

**BN - Jan 04, 2018**

![](Screenshot_Feel_Menu.png)

The **Feel Menu** allows you to add, edit and delete entries in the current feel map. You get to the menu using the [feel](command_ref_feel.md#feel) command.

Please refer to the section on [Feel](command_ref_feel.md) commands for information about *feel maps* and how to use them.

### Menu Items

Each item in the menu represents one entry in the *feel map*. You cannot delete the first or last entries. If the map is empty, create a new one using the [feel new](command_ref_feel.md#feel-new) command.

Key combinations:

|     | Action     |
| :--- | :--- |
| **up/down** | Changes the value of the entry in steps of 0.1. A value cannot be increased or decreased beyond those of its nearest neighbours.|
| **shift-up/down** | Changes the value of an entry in steps of 0.01.|
| **shift-left/right** | Insert a new entry to the left or right of the currently highlighted entry.|
| **shift-delete** | Delete the current entry. There is no *undo* for this operation.|
