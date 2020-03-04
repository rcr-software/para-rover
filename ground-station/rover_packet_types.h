#ifndef rpt_h
#define rpt_h

typedef struct {
    int type_spec; //0
    int a; int b; int c;
} triplet_t;

typedef struct {
    int type_spec; //1
    float x1;
    float y1;
    float x2;
    float y2;
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

// NOTE!!!!
// type 3 reserved for strings 



#endif

