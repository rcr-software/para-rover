#include "Arduino.h"
#include "sensors.h"

Sensors::Sensors() {
    Serial.print(F("Initialzing VL53L0X..."));
    if (!lox.begin()) {
        Serial.println(F("FAIL"));
        //while(true);
    }
    Serial.println(F("DONE"));

    Serial.print(F("Initializing BMP180... "));
    if (!bmp.begin()) {
        Serial.println("FAIL");
        //while (true);
    }

    Serial.println(F("DONE"));
}

long Sensors::read_lidar()
{
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false);
    if (measure.RangeStatus != 4) {
        return measure.RangeMilliMeter;
    } else {
        return -1;
    }
}

float Sensors::read_temperature()
{
    return bmp.readTemperature();
}

long Sensors::read_pressure()
{
    return bmp.readPressure();
}
