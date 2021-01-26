# pyserial class emulator for testing purpose.
# Adapted from here: http://www.science.smith.edu/dftwiki/index.php/PySerial_Simulator

class FakeSerial:
    def __init__(self, port='COM1', baudrate = 19200, timeout=1,
                  bytesize = 8, parity = 'N', stopbits = 1, xonxoff=0,
                  rtscts = 0):
        self.name     = port
        self.port     = port
        self.timeout  = timeout
        self.parity   = parity
        self.baudrate = baudrate
        self.bytesize = bytesize
        self.stopbits = stopbits
        self.xonxoff  = xonxoff
        self.rtscts   = rtscts
        self._isOpen  = True
        self._receivedData = b''
        self._data = open('fake_data.bin', 'rb').read()

    def isOpen(self):
        return self._isOpen

    def open(self):
        self._isOpen = True

    def close(self):
        self._isOpen = False

    def write(self, string):
        self._receivedData += string

    def read(self, n=1):
        s = self._data[0:n]
        self._data = self._data[n:]
        return s

    def readline(self):
        raise Exception("not implemented")

    def __str__(self):
        return  "Serial<id=0xa81c10, open=%s>(port='%s', baudrate=%d," \
               % (str(self.isOpen), self.port, self.baudrate) \
               + " bytesize=%d, parity='%s', stopbits=%d, xonxoff=%d, rtscts=%d)"\
               % (self.bytesize, self.parity, self.stopbits, self.xonxoff,
                   self.rtscts)
