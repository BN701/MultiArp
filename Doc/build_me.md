Open terminal window. Create a top level folder of your choice, cd into it and then run:
```
git clone https://github.com/BN701/Multi-Arp.git
```
Now, stay in the same folder and clone the Ableton Link repository. (Note the followup sub-module update command.)
```
git clone https://github.com/Ableton/link.git
cd link
git submodule update --init --recursive
cd ..
```
We also use Craig Sapp's Midifile library for importing midi files:
```
git clone https://github.com/craigsapp/midifile
cd midifile
make library
cd ..
```
Finally, you need to install xclip. This utility is run as a shell command. You do not need to build with it.
