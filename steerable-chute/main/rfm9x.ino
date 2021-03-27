void Initialize_RFM() {
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.println("Feather LoRa TX Test!");
 
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
 
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
 
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
 
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}

void sendData() {
  Serial.println("Transmitting..."); // Send a message to rf95_server

  int16_t rssi = (int16_t)(rf95.lastRssi());
  char radiopacket = 'C';

  
  sprintf(buffer, "%c, %d, %d", radiopacket, packetnum, rssi);
  sendLen = strlen(buffer);  //get the length of buffer
  
  
  

  //Serial.print("rssi LAST: ");
  //Serial.println(rssi, DEC);
  
  //itoa(rssi, radiopacket+13, 10);
  Serial.print("Sending "); Serial.println(buffer);
  //radiopacket[19] = 0;
  
  Serial.println("Sending...");
  delay(10);
  //rf95.send((uint8_t *)radiopacket, 6);
  rf95.send((uint8_t *) buffer, sendLen);
  //rf95.send((uint8_t *)rssi, 7);
 
  Serial.println("Waiting for packet to complete..."); 
  delay(10);
  rf95.waitPacketSent();
  // Now wait for a reply
}


void receiveData() {
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  packetnum += 1;
 
  Serial.println("Waiting for reply...");
  if (rf95.waitAvailableTimeout(1000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("Got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);  
      rData = String((char*)buf);
        
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
  else
  {
    Serial.println("No Data");
  }
}
