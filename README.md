## Branches

The main branch is now called `main`. Currently there are no other branches
with up to date code. If you add another branch, document them and their purpose in this part of the
README.

# Setup

Once you have the arduino and a Feather M0 connected, you'll need the aruduino IDE and the 
following extra steps:

## Arduino libraries installed using libary manager:

Adafruit Motor Shield V2 Library (say 'yes' to dependencies)

## Arduino libraries installed manually:

Radio library: go to this page, click the green "RadioHead-1.62.zip" download and follow their
instructions

## Getting the feather m0 "board files":

Adafruit boards instructions:
https://learn.adafruit.com/add-boards-arduino-v164/setup

All you have to do is add
https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
in preferences > Additional Boards Manager Urls

Then add "Adafruit SAMD Boards" in the boards manager

You'll probably get warnings for "Multiple libaries were found for ..." when compiling, just ignore
them.

# Tricky stuff

The `RH_RF95::recv(uint8_t* buf, uint8_t* len)` function is sneaky! 
- The `len` variable is the number of byes to be copied from the radio's internal buffer.
- If it is less than the amount available, you WILL ONLY GET `len` BYTES COPIED
- If it is greater, then it will be set to however many are copied.
- Thus, you must always set you `len` variable to RH_RF95_MAX_MESSAGE_LEN before calling `recv`
  or you might miss some bytes. See https://github.com/kenbiba/RH-RF95/blob/master/RH_RF95.cpp#L213
  to be further confused.

Also see the note in rover.ino about AccelStepper. It is tricky.


# TODO

- Refactor rover into multiple files
- Sensor data reading on the rover
- Sensor data receiving in the gui backend I have right now 
- Make the rover echo back an acknoledgement for each packet it receives
    - The GUI should eventually show when communication is getting spotty.
- Display GUI for that data (this will be assigned to Bhumika)
    - I need to make a synthetic data file for her to use to test the gui
- Obstacle avoiding using the LIDAR. Keeton can handle this.
- Staging protocol (in rocket, release, etc)

