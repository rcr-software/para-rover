import pygame, sys, time
import serial
from pygame.locals import *

# local file include
from packet_parse import pack, unpack, packet_to_streamable

pygame.init()
pygame.joystick.init()
# TODO is it okay to just take `joystick(0)`?
joystick = pygame.joystick.Joystick(0)
joystick.init()

print('number joysticks connected:', pygame.joystick.get_count())
# TODO make this into a command line arg or YAML
ser = serial.Serial('/dev/tty.usbmodem141401', baudrate=115200)
print(ser.name)


def send_bytestring(bs):
    ser.write(packet_to_streamable(bs))

def get_joys():
    return [joystick.get_axis(i) for i in range(joystick.get_numaxes())]

def get_buttons():
    return [joystick.get_button(i) for i in range(joystick.get_numbuttons())]


def dist(a, b):
    return sum([(x-y)**2 for x,y in zip(a,b)])

def get_number_of_new_button(new_buttons, last_buttons):
    for i, (new, last) in enumerate(zip(new_buttons, last_buttons)):
        if new and not last:
            return i


buttons = ['select','left stick','right stick', 
            'start','up', 'right', 'down', 'left',
            'left trigger', 'right trigger', 'left bumper', 'right bumper',
            'triangle', 'circle', 'x', 'square',
            'meta', 'unknown', 'not sure']

joy_zero = get_joys()
last_joys = get_joys()
last_buttons = get_buttons()
def send_joystick():
    new_joys = get_joys()
    nj = [a-b for a,b in zip(new_joys, joy_zero)]
    #print('\t'.join(['{:10.4f}'.format(x) for x in nj]))
    send_bytestring(pack('joystick_t', x1=nj[0], y1=nj[1], x2=nj[2], y2=nj[3]))
    last_joys = new_joys

def poll_events():
    for event in pygame.event.get():
        if event.type == QUIT:
            return
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                return
        if event.type == pygame.JOYBUTTONDOWN:
            global last_buttons
            new_buttons = get_buttons()
            print(get_buttons())
            button_code = get_number_of_new_button(new_buttons, last_buttons)
            if button_code is not None:
                print(buttons[button_code])
                send_bytestring(pack('button_t', button=button_code.to_bytes(1, byteorder='big')))
                last_buttons = new_buttons
        if event.type == pygame.JOYBUTTONUP:
            last_buttons = get_buttons()
        if event.type == pygame.JOYAXISMOTION:
            pass

last_joystick = time.time()

while True:
    poll_events()

    if (time.time() - last_joystick > .1):
        send_joystick()
        last_joystick = time.time()

    bytes_in = list(ser.read(ser.in_waiting))
    if len(bytes_in) > 0:
        print(bytes_in)

ser.close()
pygame.quit()
