//#include "src/main.h"
//#include <Wire.h>
#include <Math.h>

#include "drive.h"
#include "radio.h"
#include "packets.h"
#include "sensors.h"
#define SERIAL_BAUD_RATE 115200

Drive* drive;
Radio* radio;
Sensors* sensors;

int dot_count = 0;

// does something with received packets
void dispatch() {

    int packet_number = *((int *)radio->receive_buffer);
    joystick_t* joystick;
    switch (packet_number) {
        case PAC_TRIPLET:
            Serial.println("Got a triplet lol");
            break; 
        case PAC_JOYSTICK:
            joystick = (joystick_t *)radio->receive_buffer; 
            Serial.println((int) 255*joystick->y1);
            Serial.println((int)255*joystick->y2);
            drive->drive((int) 255*joystick->y1, (int)255*joystick->y2);
            break;
        case PAC_SENSOR:
            Serial.println("Got a sensor lol");
            break;
        default:
            Serial.println("TF is this??");
    }
}


void setup() {
    Serial.begin(SERIAL_BAUD_RATE);

    Serial.println(F("Initializing..."));
    drive = new Drive();
    radio = new Radio(11, 10, 6, 915.0);
    sensors = new Sensors();
    Serial.println(F("Initialization complete!"));
}

void loop() {
    // dn.setDriveSpeed((int)255*data->y1, (int)255*data->y2);
    if (dot_count % 10000 == 0) {
        Serial.print(".");
    }
    if (dot_count == 600000) {
        Serial.println();
        sensor_data_t sensor_data;
        sensor_data.type_spec = 2;
        sensor_data.altitude = sensors->read_altitude();
        sensor_data.temperature = sensors->read_temperature();
        sensor_data.lidar_distance = sensors->read_lidar();
        radio->send((void*) &sensor_data, sizeof(sensor_data_t));

        Serial.println();
        dot_count = 0;
    }
    dot_count += 1;

    

    if (radio->receive()) {
        Serial.println();
        dispatch();
    }

}
