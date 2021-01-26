import fake_serial 
import packet_parse

ser = fake_serial.FakeSerial('/dev/tty.usbmodem141401', baudrate=115200, timeout = 0.1)

# The real thing would be:
# ser = serial.Serial('/dev/tty.usbmodem141401', baudrate=115200)

in_data = b''
while True:

    # This would need to accout for data usually not being available most of the time,
    # I'm not sure how to do non-blocking reads with the actual serial module.
    in_data += ser.read(5)

    # the serial in data comes as packets formatted as
    # {1 byte describing length of packet} + packet + {1 byte checksum of the packet}
    packet_length = in_data[0]
    length_if_ready = packet_length + 2
    if len(in_data) >=  length_if_ready:
        new_struct = in_data[:length_if_ready]
        in_data = 
