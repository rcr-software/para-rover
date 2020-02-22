from struct import *
import serial
print(pack('hhl', 1, 2, 3))

ser = serial.Serial('/dev/cu.usbmodem1421')  # open serial port
print(ser.name)         # check which port was really used
ser.write(b'%c' % 5)
ser.write(b'hello')     # write a string

packet = pack('iiii', 0, 1, 2, 3)
ser.write(b'%c' % len(packet))
ser.write(packet)

packet = pack('iffff', 1, 2.0, 3.0, 4.0, 5.0)
ser.write(b'%c' % len(packet))
ser.write(packet)

while True:
    print(ser.readline())
ser.close()             # close port
