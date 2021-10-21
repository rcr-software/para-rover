void RADIO_initialize(){
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  delay(1000);

  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while(!rf95.init())
  {
    Serial.println("LoRa radio init failed");
    while(1);
  }
  Serial.println("radio init ok");
  if (!rf95.setFrequency(RF95_FREQ))
  {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("set freq to: "); Serial.println(RF95_FREQ);
  rf95.setTxPower(23, false);
}
