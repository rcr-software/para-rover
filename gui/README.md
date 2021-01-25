# Installation

Make sure your Python and pip are both Python 3. Then install requirements with:

`pip -r requirements.txt`

# Running

First, modify this line in `joystick_test.py` to point to ther ground station serial device:

`ser = serial.Serial('/dev/tty.usbmodem141401', baudrate=115200)`

Then, with ground station connected, rover power on, and ps3 controller connected over blutooth (and
not plugged in), run

`python joystick_test.py`
