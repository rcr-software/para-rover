To use, just run ground-station.py then rover.py, in that order. Both have
to be running at the same time. Might now work on windows, depending
on pipe behaviour. 

Once you have this working, figure out how to adapt existing gui.py
code to work both with this sort of interface and the actaul uart.
You will probably need to add wrapper read/write functions that
pick the correct way to read/write depending on if it's the real
device or the piped simulator.
