void printGPS() {
  if (GPSSerial.available()) {
    gpsData = GPSSerial.readString();
    char c = GPSSerial.read();
    Serial.write(c);
  }
}
