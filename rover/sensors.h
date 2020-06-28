#include <Adafruit_VL53L0X.h>
#include <Adafruit_BMP085.h>

class Sensors
{
    public:
        Sensors();
        long read_lidar();
        float read_temperature();
        long read_pressure();
    private:
        Adafruit_VL53L0X lox;
        Adafruit_BMP085 bmp;
};
