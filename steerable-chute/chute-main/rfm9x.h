#ifndef _RFM_H
#define _RFM_H

class RFM {
//private:
  int16_t packetnum;
  int8_t rssi;
  char buffer[RH_RF69_MAX_MESSAGE_LEN];
  char radiopacket = 'C';
  String rData;
  byte sendLen;

  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  uint8_t receiveLen;

public:
  void init(void);                //initialize device
  void sendData(void);            //send packets
  void receiveData(void);         //receive packets

};

extern RFM rfm;
extern RH_RF95 rf95(RFM95_CS, RFM95_INT);

#endif //_RFM_H
