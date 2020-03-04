import tkinter as tk
import serial
import struct

base_station = serial.Serial("/dev/cu.usbmodem1421", timeout=0.1)


def send_joystick(x1, y1, x2, y2):
    packet = struct.pack('iffff', 1, x1, y1, x2, y2)
    base_station.write(b'%c' % len(packet))
    base_station.write(packet)

class Application(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.master = master


        self.text_box = tk.Text(master=master)
        self.text_box.pack(side=tk.RIGHT)
        self.text_box.config(state="disabled")
        

        master.bind("<KeyPress>", lambda e: self.keydown(e))
        self.pack()
        self.create_widgets()
    def keydown(self, e):
        print('down', e.char)
        c = e.char
        if c == 'w':
            send_joystick(0, 1, 0, 0);
        elif c == 'a':
            send_joystick(1, 0, 0, 0);
        elif c == 's':
            send_joystick(0, -1, 0, 0);
        elif c == 'd':
            send_joystick(-1, 0, 0, 0);
        else:
            print('not revognised by bui')

    def create_widgets(self):
        self.hi_there = tk.Button(self)
        self.hi_there["text"] = "Hello World\n(click me)"
        self.hi_there["command"] = self.say_hi
        self.hi_there.pack(side="top")

        self.quit = tk.Button(self, text="QUIT", fg="red",
                              command=self.master.destroy)
        self.quit.pack(side="bottom")

    def say_hi(self):
        foo = base_station.readline().decode('utf-8') 
        self.text_box.config(state="normal")
        self.text_box.insert(tk.END, foo)
        self.text_box.config(state="disabled")
        print("hi there, everyone!")
        #self.text_box.insert(tk.END, "Hello!\n")
        base_station.write(b"w")

root = tk.Tk()
app = Application(master=root)
app.mainloop()
