
#include "struct_send_receive.h"


// common struct used to all transmissions, 
// sender and receiver must know what's being sent/received


// ******************************
// STRUCT SECTION
// Each struct gets:
// typedef (in header), sender function, and printer
//
// see RoverLib/rover_packet_types.h for the type defs,
// since they have to be in a header to work in function args
// ************************************

// also this SEND_PACKET macro which is used in every send function.
// assumes packet is struct, value is && of both success values
#define SEND_PACKET (rf95.send((uint8_t *)&packet, sizeof(packet)) && rf95.waitPacketSent())
#define pc(thing) Serial.print(thing); Serial.print(", ")
#define p(thing) Serial.print(thing)
#define nl() Serial.println("")

char print_buffer[100];

// Struct 0: triplet
int send_triplet(int a, int b, int c) {
    triplet_t packet;
    packet = {0, a, b, c};
    print_triplet(&packet);
    return SEND_PACKET;
}
void print_triplet(triplet_t * packet) {
    p("joystick(");
    pc(packet->a); pc(packet->b); p(packet->c);
    p(")"); nl();
}

// Struct 1: joystick
int send_joystick(float x1, float y1, float x2, float y2) {
    joystick_t packet;
    packet = {1, x1, y1, x2, y2};
    print_joystick(&packet);
    return SEND_PACKET;
}
void print_joystick(joystick_t * packet) {
    p("joystick(");
    pc(packet->x1); pc(packet->y1); pc(packet->x2); p(packet->y2);
    p(")"); nl();
}

// Struct 2: sensor data
int send_sensor_data(float altitude, double gps_x, double gps_y, 
        float orientation_x, float orientation_y, float orientation_z) {
    sensor_data_t packet;
    packet = {2, altitude, gps_x, gps_y, orientation_x, orientation_y, orientation_z};
    print_sensor_data(&packet);
    return SEND_PACKET;
}
void print_sensor_data(sensor_data_t * packet) {
    p("sensor_data(");
    pc(packet->altitude); pc(packet->gps_x); pc(packet->gps_y);
    pc(packet->orientation_x); pc(packet->orientation_y); p(packet->orientation_z);
    p(")"); nl();
}

// globals section

uint8_t radio_receive_buffer[RH_RF95_MAX_MESSAGE_LEN];

// functions section
void print_struct(uint8_t* buffer) {
    int dispatch_number = *((int *) buffer);
    switch (dispatch_number)
    {
        case 0:
            print_triplet((triplet_t*) buffer);
            break;
        case 1:
            print_joystick((joystick_t*) buffer);
            break;
        case 2:
            print_sensor_data((sensor_data_t*) buffer);
            break;
        case 3:
            p("S: "); p(String((char*) buffer+4)); nl();
            break;
        default:
            Serial.print("dispatch number not recorgnised: ");
            Serial.println(dispatch_number);
    }
}

int try_receive_message() {
    if (rf95.waitAvailableTimeout(1000)) {
        int dispatch_number;
        Serial.println("\r\ngot a message!");
        digitalWrite(LED_BUILTIN, HIGH);
        // must be set to what you expect,
        // but can be modified if r_len is oversized!
        uint8_t r_len = RH_RF95_MAX_MESSAGE_LEN;
        if (rf95.recv(radio_receive_buffer, &r_len)) {
            if (r_len != RH_RF95_MAX_MESSAGE_LEN) {
                Serial.print("length: ");
                Serial.println(r_len); 
            }
            print_struct(radio_receive_buffer);
        }
        dispatch_number = *((int *) radio_receive_buffer);
        return dispatch_number;
    } else {
        return 0;
    }
}
