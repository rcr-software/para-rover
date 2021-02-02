
So this code isn't very refined but it works pretty well. It allows for full duplex on the radio,
but I'm pretty sure the radio doesn't support full duplex so this is something of a waste.

```
    Serial                 packet radio
PC <-------> ground-station <---------> rover
```

The main idea here is that the Serial from the computer is a stream, while the packet radio is a
series of packets. So, the PC sends one byte for the length of the message, the message, then
checksum of the message. If it's correct, then the message is sent over the radio. 

To go the other way, when a packet is received, its length is sent to the PC, the each byte of the
packet, and a checksum. 

The PC runs the same protocol and it all just works great.
