#ifndef ssr_h
#define ssr_h


//#include "Arduino.h"
#include "rover_packet_types.h"
#include <RH_RF95.h>
extern class RH_RF95 rf95;

//char print_buffer[100];

// Struct 0: triplet
int send_triplet(int a, int b, int c);
void print_triplet(triplet_t * packet);
// Struct 1: joystick
int send_joystick(float x1, float y1, float x2, float y2);
void print_joystick(joystick_t * packet);
// Struct 2: sensor data
int send_sensor_data(float altitude, double gps_x, double gps_y, 
        float orientation_x, float orientation_y, float orientation_z);

void print_sensor_data(sensor_data_t * packet);

// globals section
//uint8_t* radio_receive_buffer;
extern uint8_t radio_receive_buffer[RH_RF95_MAX_MESSAGE_LEN];

int try_receive_message();

void print_struct(uint8_t* buffer);


#endif
