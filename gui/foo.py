from struct import *
import serial
print(pack('i', 3))

def encode_bytes(packet):
    ''' implements simple escape character/control character byte level 
    encoding. 'b' is packet terminator control character, a is escape character.
    Thus a -> aa, and b -> ab.'''
    packet = packet.replace(b'a', b'aa')
    packet = packet.replace(b'b', b'ab')
    packet += b'b'
    return packet


ser = serial.Serial('/dev/cu.usbmodem1411')  # open serial port
print(ser.name)         # check which port was really used


packet = pack('iiii', 0, 1, 2, 3)
print("packet len = ", len(packet))
ser.write(encode_bytes(packet))


packet = pack('i', 3) + b'Hello ROVER\0'
print("packet len = ", len(packet))
ser.write(encode_bytes(packet))

##ser.write(b'%c' % len(packet))
##ser.write(packet)

packet = pack('iffff', 1, 2.0, 3.0, 4.0, 5.0)
ser.write(encode_bytes(packet))
print("packet len = ", len(packet))
##ser.write(b'%c' % len(packet))
##ser.write(packet)

while True:
    print(ser.readline().decode('utf-8').strip())
ser.close()             # close port
