import os

# the two named pipes are "up" (from ground to rover) and "down" (from rover to ground).
# The actual usb/uart acts like two fifo's.

print("hi im the ground station. I send joysticks and receive random data")
if os.path.exists("up"):
    os.remove("up")
if os.path.exists("down"):
    os.remove("down")

os.mkfifo('up')
os.mkfifo('down')

pipein = open('down', 'rb')
pipeout = open('up', 'wb')


pipeout.write(b'left ')
pipeout.write(b'right ')
pipeout.write(b'left ')
pipeout.flush()

print('asdf')

while True:
    line = pipein.read(1)
    print('Ground station got: ', line, '\n')
