void sd_init()
{
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");

  }
  Serial.println("card initialized.");
  File dataFile = SD.open("GPSlog.txt", FILE_WRITE);
  String introData = "\n\nNEW INIT CHEESED TO MEET YOU\n\n";
  if (dataFile)
  {
    dataFile.print(introData);
    dataFile.close();
  }
}

void sd_logdata(float LAT, float LONG, float ALT, float BMP_ALT, float YAW, float PITCH, float ROLL)
{
  String dataString1 = ",LAT:";
  dataString1 += String(LAT);
  String dataString2 = ",LONG:";
  dataString2 += String(LONG);
  String dataString3 = ",ALT:";
  dataString3 += String(ALT);
  String dataString4 = ",BMPALT";
  dataString4 += String(BMP_ALT);
  String dataString5 = ",YAW:";
  dataString5 += String(YAW);
  String dataString6 = ",PIT:";
  dataString6 += String(PITCH);
  String dataString7 = ",ROL:";
  dataString7 += String(ROLL);

  
  File dataFile = SD.open("GPSlog.txt", FILE_WRITE);

  if (dataFile)
  {
    dataFile.print(dataString1);
    dataFile.print(dataString2);
    dataFile.print(dataString3);
    dataFile.print(dataString4);
    dataFile.print(dataString5);
    dataFile.print(dataString6);
    dataFile.println(dataString7);
    
    dataFile.close();   
  }
  else
  {
    Serial.println("error with SD writing");
  }
  
  
}
