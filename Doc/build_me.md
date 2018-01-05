# Build Instructions

BJN - Jan 05, 2018

MultiArp is being developed using the Code::Blocks  IDE, and the project files are included in the repository.

However, if you just want to dive in get the current source build, I've hacked together a makefile using some examples I found [here](https://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html). It's not elegant, but it seems to be enough for now.

Either way, steps to get up and running are as follows.

## Download MultiArp source
Open terminal window. Create a top level folder of your choice, cd into it and then run:
```
git clone https://github.com/BN701/Multi-Arp.git
```
## Get Ableton Link
Now, stay in the same folder and clone the Ableton Link repository. (Note the followup sub-module update command.)
```
git clone https://github.com/Ableton/link.git
cd link
git submodule update --init --recursive
cd ..
```
Ableton Link is a header only library. There are no addition build steps.
## Midi file support
We also use Craig Sapp's Midifile library for importing midi files:
```
git clone https://github.com/craigsapp/midifile
cd midifile
make library
cd ..
```
## Build MultiArp
```
cd MultiArp
make
```
The output file, called MultiArp, will be in the project directory. Either run it from there or copy it to wherever you'd like. It doesn't have any configuration files to worry about at the moment.

## Install *xclip*
Finally, you need to install *xclip*. This utility is run silently as a shell command whenever *MultiArp* uses copy/paste to transfer data between instances and a your text editor.

I installed it on my system with:
```
sudo apt install xclip
```

If your system doesn't have it, you can always get it from here: https://github.com/astrand/xclip.
