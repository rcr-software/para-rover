## Branches

The main branch is now called `main`. Currently there are no other branches
with up to date code. If you add another branch, document them and their purpose in this part of the
readme.

# Setup

## Python setup

See `gui/` python readme.

## Arduino Setup

Once you have the arduino and a Feather M0 connected, you'll need the aruduino IDE and the 
following extra steps:

### Arduino libraries installed using libary manager:

Adafruit Motor Shield V2 Library (say 'yes' to dependencies)

### Arduino libraries installed manually:

Radio library: go to this page, click the green "RadioHead-1.62.zip" download and follow their
instructions

### Getting the feather m0 "board files":

Adafruit boards instructions:
<https://learn.adafruit.com/add-boards-arduino-v164/setup>

All you have to do is add
<https://adafruit.github.io/arduino-board-index/package_adafruit_index.json>
in preferences > Additional Boards Manager Urls

Then add "Adafruit SAMD Boards" in the boards manager

You'll probably get warnings for "Multiple libaries were found for ..." when compiling, just ignore
them.

# Uploading and Using

1. Plug in rover using a data-enable micro USB cable. Don't connect power to motor shield yet.
2. Launch Arduino IDE, make sure `tools > Board` is "Adafruit Feather M0", open `rover.ino`, and
     select `tools > port > whatever port the rover is`.  There's usually only one port, ignore any
     blutooth ports or stuff like that.
     - If you don't see it, you might have a bad cable, or the rover is crashed. Try quickly double
         pressing the rover's reset button to put it in bootloader mode (making the LED slowly pulse),
         then it should show up and you can continue. Also the tools menu has to be closed and open for
         the ports list to update.
3. Click upload. There are plenty of things to go wrong here, but unplugging and replugging and
double clicking the rover's reset button should fix most of them. 
4. Unplug the rover and plug in the ground station. Optionaly, also upload `ground-station.ino` to
    the ground station. It shouldn't ever need to be changed but it doesn't hurt to upload to it to
    make sure the code is still fresh ;)
6. Plug in the Taranis Q controller. It has to be turned on while unplugged, then plugged on. Also
    it will rattle furiously when the trims pass center or if you leave it on without touching it for a
    few minutes. Terrifying. Also it takes AA batteries, wasteful but fine for now.
7. open `gui/` in a terminal and run `python3 serial_wrapper.py`
    - if it says `"serial.serialutil.SerialException: [Errno 2] could not open port
      /dev/cu.usbmodem141401: [Errno 2] No such file or directory: ..."`, then you need to go into
      the code on line 14 and change the part in quotes to the port shown in the Arduino IDE that
      the ground station is connected to. The part could be more elegant but it works for now.
8. Move the joystick around and confirm the number printed out reflect the sticks' movements
9. Connect the small battery into the rover's matching plug
9. Connect the red and yellow wires from the motor controller's terminal block to the large battery.
    Red is positive and yellow is negative.
    - VERY IMPORTANT:
        Don't let the two wires plugged into the battery touch - wires have EXPLODED due to shorting
        a smaller lipo than the one here (by yours truly). And make sure not to reverse the
        polarity. There's a protection diode but I wouldn't trust it.
10. Now the joystick should drive the rover! Right now it has (badly implemented) arcade controls on
    the left joystick.
11. Unplug the rover batteries when not in use, and occastionally check them with the voltage alarm
    to see if they need charging.

# Tricky stuff

The `RH_RF95::recv(uint8_t* buf, uint8_t* len)` function is sneaky! 
- The `len` variable is the MAXIMUM number of byes to be copied from the radio's internal buffer.
- If it is less than the amount available, you WILL ONLY GET `len` BYTES COPIED
- If it is greater, then it will be set to however many are copied.
- Thus, you must always set you `len` variable to `RH_RF95_MAX_MESSAGE_LEN` before calling `recv`
  or you might miss some bytes. See
  <https://github.com/kenbiba/RH-RF95/blob/master/RH_RF95.cpp#L213> to be further confused.

Also see the notes in rover/Motors.cpp about AccelStepper. It is tricky.

# TODO

- Change `joystick` packet into a `drive` packet so it's more sensible that the python code is the
  one picking how to use the joystick. Alternatively, send all 4 axis and have the rover do the
  mixing. Nitpicking detail but still an issue that's bugging me.
- Sensor and GPS data reading on the rover
- Sensor data receiving in the gui backend I have right now 
- Make the rover echo back an acknoledgement for each packet it receives
    - The GUI should eventually show when communication is getting spotty.
- Display GUI for that data (this will be assigned to Bhumika)
    - I need to make a synthetic data file for her to use to test the gui
- Obstacle avoiding using the LIDAR. Keeton can handle this.
- Staging protocol (in rocket, release, etc)
- Backing up data in SD card

