'''
Not part of the actual project, just here to test the joystick and get the
axis and buttons figured out
'''
  
import pygame, sys, time
from pygame.locals import *
from serial_wrapper import SerialWrapper

pygame.init()
pygame.joystick.init()
joystick = pygame.joystick.Joystick(0)
joystick.init()
ser = SerialWrapper('/dev/cu.usbmodem141401')

print('number joysticks connected:', pygame.joystick.get_count())


def get_joys():
    return [joystick.get_axis(i) for i in range(joystick.get_numaxes())]

def get_buttons():
    return [joystick.get_button(i) for i in range(joystick.get_numbuttons())]


def get_number_of_new_button(new_buttons, last_buttons):
    for i, (new, last) in enumerate(zip(new_buttons, last_buttons)):
        if new and not last:
            return i

def send_joystick():
    nj = [800*x for x in get_joys()]
    vert = nj[0]
    hor = -nj[3]
    ser.send_struct('joystick_t', {'x1': 0, 'y1': vert + hor, 'x2': 0, 'y2': vert - hor})

class DebouncedJoystickPoll():
    def __init__(self):
        self.last_joystick = 0

    def poll_events(self):
        was_joy = False
        for event in pygame.event.get():
            if event.type == QUIT:
                return
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    return
            if event.type == pygame.JOYBUTTONDOWN:
                print(get_buttons())
            if event.type == pygame.JOYBUTTONUP:
                last_buttons = get_buttons()
            if event.type == pygame.JOYAXISMOTION:
                pass
    def send_joy_if_time_passed(self):
        if time.time() - self.last_joystick > 0.1:
            print(get_joys())
            send_joystick()
            self.last_joystick = time.time()


db = DebouncedJoystickPoll()
while True:
    db.poll_events()
    db.send_joy_if_time_passed()
    maybe_struct = ser.get_next_struct_if_available()
    if maybe_struct != None:
        print(maybe_struct)

