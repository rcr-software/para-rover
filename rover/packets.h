#ifndef rpt_h
#define rpt_h

// REMEMBER to update this when changing structs
enum packets{PAC_STRING = 0,  PAC_TRIPLET, PAC_JOYSTICK, PAC_SENSOR, PAC_BUTTON};


enum button_code_t {BUTTON_SELECT = 0, BUTTON_LEFT_STICK, BUTTON_RIGHT_STICK, 
            BUTTON_START, BUTTON_UP, BUTTON_RIGHT, BUTTON_DOWN, BUTTON_LEFT,
            BUTTON_LEFT_TRIGGER, BUTTON_RIGHT_TRIGGER, BUTTON_LEFT_BUMPER, BUTTON_RIGHT_BUMPER,
            BUTTON_TRIANGLE, BUTTON_CIRCLE, BUTTON_X, BUTTON_SQUARE,
            BUTTON_META, BUTTON_UNKNOWN, BUTTON_NOT_SURE};

// IMPORTANT: Do not add any structs or typedefs in this file except these, they could break
// packet_parse.py and it is some seriously arcane bullshit you don't want to deal with.
// Also restrict struct fields to basic types (see type_tuple_to_struct_spec in
// packet_parse.py for a full list of allowed types) and character arrays 
// Also, don't exceed 80 bytes with any of these

typedef struct {
    int type_spec; //0
    // This is a special type meant for sending debug strings
    char string[81];
} string_t;

typedef struct {
    int type_spec; //1
    int a; int b; int c;
} triplet_t;

typedef struct {
    int type_spec; //2
    float x1;
    float y1;
    float x2;
    float y2;
} joystick_t;

typedef struct {
    int type_spec; //3
    long altitude;

    float temperature;
    long lidar_distance;

    double gps_x;
    double gps_y;
    float orientation_x;
    float orientation_y;
    float orientation_z;
} sensor_data_t;

typedef struct {
    int type_spec; //3
    char button;
} button_t;

#endif

