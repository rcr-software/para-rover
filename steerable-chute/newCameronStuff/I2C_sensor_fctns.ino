void getPressure()
{
  //collects and averages pressure over 100 readings
  

  for(int i=0;i<101;i++)
  {
    sea_level_pressure += (bmp.readPressure()/100);
  }
  sea_level_pressure = sea_level_pressure / 100;
  
}

void bmp_init_check()
{
    if(!bmp.begin())
  {
    Serial.println(F("Init error with BMP"));
    while(1);
  }
  
}

void bmp_set_presets()
{
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  
}

void bno_init_check()
{
    if(!bno.begin())
  {
    Serial.println("Init error with BNO");
    while(1);
  }
  
}

void printEvent(sensors_event_t* event) 
{
  double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem

  if (event->type == SENSOR_TYPE_ORIENTATION) {
    Serial.print("Orient:");
    x = event->orientation.x;
    y = event->orientation.y;
    z = event->orientation.z;
  }
  
  Serial.print("\tYAW= ");
  Serial.print(x);
  Serial.print(" |\tPITCH= ");
  Serial.print(y);
  Serial.print(" |\tROLL= ");
  Serial.println(z);
}

double returnYAW(sensors_event_t* event)
{
  double x = -100000;
  if (event->type == SENSOR_TYPE_ORIENTATION)
    { 
    x = event->orientation.x;
    }

  return x;   
}

double returnPITCH(sensors_event_t* event)
{
  double y = -100000;
  if (event->type == SENSOR_TYPE_ORIENTATION)
    { 
    y = event->orientation.y;
    }

  return y;   
}

double returnROLL(sensors_event_t* event)
{
  double z = -100000;
  if (event->type == SENSOR_TYPE_ORIENTATION)
    { 
    z = event->orientation.z;
    }

  return z;   
}
