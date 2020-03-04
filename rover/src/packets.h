#ifndef rpt_h
#define rpt_h

enum packets{PAC_TRIPLET = 0, PAC_JOYSTICK, PAC_SENSOR};

typedef struct {
    int type_spec; //0
    int a; int b; int c;
} triplet_t;

typedef struct {
    uint8_t type_spec; //1
    uint8_t x1;
    uint8_t y1;
    uint8_t x2;
    uint8_t y2;
} joystick_t;

typedef struct {
    int type_spec; //2
    float altitude;
    double gps_x;
    double gps_y;
    float orientation_x;
    float orientation_y;
    float orientation_z;
} sensor_data_t;



#endif

