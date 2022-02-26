void GPS_setup()
{
  
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

}

void GPS_print()
{
    Serial.print("LOCATION: ");
    Serial.print(GPS.latitude, 4); //Serial.print(GPS.lat);
    Serial.print(", ");
    Serial.print(GPS.longitude, 4); //Serial.println(GPS.lon);
    Serial.print("\nAltitude: "); Serial.print(GPS.altitude);
}
