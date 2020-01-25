#include <RH_RF95.h>

//#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

#include <rover_packet_types.h>

// for seperate radio boi
#define RFM95_RST     11   // "A"
#define RFM95_CS      10   // "B"
#define RFM95_INT     6    // "D"
/*
// for embedded radio boi
#define RFM95_RST     4   // "A"
#define RFM95_CS      8   // "B"
#define RFM95_INT     3    // "D"
*/

#define RF95_FREQ 915.0
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// late include :(
#include <struct_send_receive.h>

// setup/loop entry section
void setup()  {


    if(!bno.begin())
    {
        /* There was a problem detecting the BNO055 ... check your connections */
        while(1) {
            Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        }
    }
  
  delay(1000);
    
  bno.setExtCrystalUse(true);

  randomSeed(analogRead(0));

  Serial.println("IAM THAT PARACHUTE BOI");

  //setup pins
  pinMode(5, OUTPUT); // tone
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  Serial.begin(115200);

  // manual reset the radio
  digitalWrite(RFM95_RST, LOW); delay(100);
  digitalWrite(RFM95_RST, HIGH); delay(100);

  // initialize the radio
  if (!rf95.init()) {
      Serial.println("Init failed");
      while(1);
  }
  if (!rf95.setFrequency(RF95_FREQ)) {
      Serial.println("Freq setting failed");
      while(1);
  }



  // optional: high power mode
  //rf95.setTxPower(23, false);

  Serial.println("Setup complete.");
  delay(100);
}

long last_send = 0;

void loop()  {
    if (rf95.available()) {
        Serial.println("try to receive a message");
        int received_code = try_receive_message();
        if (received_code == 1) {
            joystick_t* joy = (joystick_t*) radio_receive_buffer;
            int freq = 440 + 12 * (int) joy->x1 + 24 * (int) joy->y1;
            tone(5, freq, 50);
        }
    }

    if (millis() > 10000 && last_send <= millis() - 10000) {
        last_send = millis();
        Serial.println("Send a sensor data!");
        sensors_event_t event; 
        bno.getEvent(&event);
        send_sensor_data(0, 0, 0, event.orientation.x, event.orientation.y, event.orientation.z);
    }





}
