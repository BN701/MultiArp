#!/bin/sh
cd ~/"Code Blocks Projects"/MultiArp/bin/Release
xfce4-terminal --tab -x ./MultiArp Bubbles -midi=1 -step=4.00
xfce4-terminal --tab -x ./MultiArp Space Echo -midi=2 -step=12.00 -transpose=24
xfce4-terminal --tab -x ./MultiArp Bells -midi=3 -step=4.00 -transpose=-24
xfce4-terminal --tab -x ./MultiArp Alarm Clock -midi=4 -step=16.00
