'''
Wraps around serial port (or fake serial using a .bin file)
Uses packet_parse.py to do conversions

Definitions:
struct: the structured data, as stored on the rover. Depicted as a dictionary in python.
packet: the bytes that make up a C struct, as a bytestring. 
        -`pack` takes a name of a struct type (from rover/packets.h) and a dict with each member
            and returns a bytestring holding all the data
        -`unpack` takes in bytesting and returns (name, struct) where the name is the name of the type
            of the struct and struct is a dict containing the data 
stream: This is a packet with one additional byte added to the beginning and end, to make it
        suitable for serial transmission. The byte at the beginning is the length of the packet (so
        the receiver can know how many bytes to read) and the byte at the end is the checksum of the
        packet (simply the sum of the bytes in the packet, mod 256).

SerialWrapper and FileWrapper provide the same interface, but one operates on a pyserial object
while the other operates on a file.
'''
import serial
from packet_parse import pack, unpack, stream_to_packet, packet_to_stream


def long_stream_to_struct(long_stream):
    '''Takes in a stream (with possible extra bytes at the end)
        and returns (extra_bytes, (name, struct)).
       Returns long_steam, None is there aren't enough bytes.
    '''
    packet_length = long_stream[0]
    length_if_ready = packet_length + 2
    if len(long_stream) >= length_if_ready:
        stream = long_stream[:length_if_ready]
        extra_bytes = long_stream[length_if_read:]
        packet = stream_to_packet(stream)
        name, struct = unpack(packet)
        return extra_bytes, name, struct
    return long_stream, None


'/dev/tty.usbmodem141401'
class SerialWrapper:
    def __init__(self, filename):
        self.incoming_stream = b''
        self.ser = serial.Serial(filename, baudrate=115200, timeout=0)

    def get_next_struct_if_available(self):
        self.incoming_stream += self.ser.read()
        self.incoming_stream, name_and_struct = long_stream_to_struct(self.incoming_stream)
        return name_and_struct

    def send_struct(name, data):
        self.ser.write(packet_to_stream(pack(name, data)))

class FileWrapper
    def __init__(self, filename):
        with open(filename, 'rb') as fp:
            self.incoming_stream = fp.read()

    def get_next_struct_if_available(self):
        self.incoming_stream, name_and_struct = long_stream_to_struct(self.incoming_stream)
        return name_and_struct

    def send_struct(name, data):
        pass

#real_serial = SerialWrapper('/dev/tty.usbmodem141401')
fake_serial = FileWrapper('fake_data.bin')
