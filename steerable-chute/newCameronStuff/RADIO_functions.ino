void RADIO_waitresponse()
{
    Serial.println("\nListening response...\n");
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.waitAvailableTimeout(3000))
    {
      if (rf95.recv(buf, &len))
      {
       Serial.print("\ngot reply:");
       Serial.println((char*)buf);
       Serial.print("\nRSSI: ");
       Serial.println(rf95.lastRssi(), DEC);
       

      }
      else
      {
        Serial.println("\nrecv failed");
      }
    }
    else
    {
      Serial.println("\nNo reply");
    }
}

void packet_send(float a, float b, float c)
{
  
  ++packetnum;
  if (even % 2 == 0) {
    --packetnum;
  }
  ++even;
  
  float RSSI_VALUE = rf95.lastRssi();


  

  char radiopacket[] = "s,#    ,    ,        ,        ,      "; 
  itoa(packetnum, radiopacket+3,10);
  itoa(RSSI_VALUE, radiopacket+8, 10);
  itoa(a, radiopacket+13, 10);
  itoa(b, radiopacket+22, 10);
  itoa(c, radiopacket+31, 10);

  
  Serial.print("Packet being sent::"); Serial.println(radiopacket);
  //radiopacket[19] = 0;
  
  Serial.println("Sending...");
  delay(1);
  rf95.send((uint8_t *)radiopacket, 37);
}
