import os

print('hi im the rover, I receive joystick commands and send random data')

pipeout = open('down', 'wb')
pipein = open('up', 'rb')

pipeout.write(b'123 ')
pipeout.write(b'340 ')
pipeout.write(b'432 ')
pipeout.flush()

while True:
    line = pipein.read(1)
    print('rover got: ', line, '\n')


