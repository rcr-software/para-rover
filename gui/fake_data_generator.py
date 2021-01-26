from packet_parse import pack, packet_to_streamable
import random

def normalize(a, b, c):
    m = (a**2 + b**2 + c**2)**.5
    return a/m, b/m, c/m

class FakeRover():
    def __init__(self):
        self.state = 'takeoff'
        self.altitude = 0
        self.temperature = 70
        self.lidar_distance = 3
        self.gps_x = 34.5199
        self.gps_y = -105.8701
        self.orientation_x = 0
        self.orientation_y = 0
        self.orientation_z = 0

    def step(self):
        self.temperature += 0.01 * (random.random() * 2 - 1)
        if self.state == 'takeoff':
            self.altitude += 5
            self.orientation_z = 1 + random.random()
            self.gps_x += 0.01 * (2 * random.random() - 1)
            self.gps_x += 0.01 * (2 * random.random() - 1)
            self.lidar_distance = 100*int(3 + 0.1 * random.random())
            if self.altitude > 1000:
                self.state = 'falling'
        if self.state == 'falling':
            self.altitude -= 1
            self.orientation_z = 0.1 * random.random()
            self.gps_x += 0.01 * (2 * random.random() - 1)
            self.gps_x += 0.01 * (2 * random.random() - 1)
            self.lidar_distance = 100*int(3 + 0.1 * random.random())
            if self.altitude <= 0:
                self.state = 'driving'
        if self.state == 'driving':
            self.gps_x += 0.001 * random.random()
            self.gps_x += 0.01 * random.random()
            self.lidar_distance = 100*int(10 + 5 * random.random())

    def get_state(self):
        return {'altitude': self.altitude,
                'temperature': self.temperature,
                'lidar_distance': self.lidar_distance,
                'gps_x': self.gps_x,
                'gps_y': self.gps_y,
                'orientation_x': self.orientation_x,
                'orientation_y': self.orientation_y,
                'orientation_z': self.orientation_z}

    def get_packet(self):
        return pack('sensor_data_t', **self.get_state())

    def get_streamable(self):
        return packet_to_streamable(self.get_packet())

rover = FakeRover()
#rover.step()
#print(rover.get_state())
#print(rover.get_packet())
#print(rover.get_streamable())

with open('fake_data.bin', 'wb') as f:
    for _ in range(1000):
        f.write(rover.get_streamable())
        rover.step()

