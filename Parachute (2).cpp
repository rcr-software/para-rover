Struct Cords()
{
	int x;
	int y;
	//int z;
	Cords(Adafruit_GPS gps, /*double z*/)
	{
		x = gps.x;
		y=gps.y;
		//this.z = z
	}

}


Class ParachuteController
{
	Adafruit_StepperMotor *motor1 = AFMS.getStepper(200, 1);
	Adafruit_StepperMotor *motor2 = AFMS.getStepper(200, 2);
	Adafruit_GPS GPS;
	//Adafruit_BMP280 bmp;
	
	Cords init;
	Dir motor1Dir;
	Dir motor2Dir;

	ParachuteController(Adafruit_GPS GPS, Adafruit_BMP280 bmp;)
	{
		motor1 = AFMS.getStepper(200, 1);
		motor2 = AFMS.getStepper(200, 2);
		
		motor->setSpeed(10);  // 10 rpm   
		motor2->setSpeed(10);
		
		this.GPS = GPS;
		this.bmp = bmp;
		init=new Cords(GPS,/*bmp.getheight()*/);
		
		motor1Dir = UNKOWN;
		motor2Dir = UNKOWN;
	}
	
	public void Calibrate()
	{
		Cords cur(gps,/*bmp.getheight()*/);
		motor1->step(100, FORWARD, DOUBLE); 
		delay(3000);
		if(abs(cur.x-gps.x) > abs(cur.y-gps.y))
		{
			if(gps.x-cur.x > 0)
			{
				motor1Dir = NORTH;
				motor2Dir = EAST;
			}
			else
			{
				motor1Dir = SOUTH;
				motor2Dir = WEST;

			}
		}
		else
		{
			if(gps.y-cur.y > 0)
			{
				motor1Dir = WEST;
				motor2Dir = NORTH;
			}
			else
			{
				motor1Dir = EAST;
				motor2Dir = SOUTH;
			}
		}
		motor1->step(100, BACKWARD, DOUBLE); 
		cur = new Cords(gps,/*bmp.getheight()*/)
		motor2->step(100, FORWARD, DOUBLE); 
		delay(3000);
		if(abs(cur.x-gps.x) > abs(cur.y-gps.y))
		{
			if(gps.x-cur.x > 0)
			{
				motor2->step(100, BACKWARD, DOUBLE);
				if(motor2Dir == NORTH)
					return true;
				else
					return false;
			}
			else
			{
				motor2->step(100, BACKWARD, DOUBLE);
				if(motor2Dir == SOUTH)
					return true;
				else
					return false;

			}
		}
		else
		{
			if(gps.y-cur.y > 0)
			{
				motor2->step(100, BACKWARD, DOUBLE);
				if(motor2Dir == WEST)
					return true;
				else
					return false;
			}
			else
			{
				motor2->step(100, BACKWARD, DOUBLE);
				if(motor2Dir == EAST)
					return true;
				else
					return false;
			}
		}
		

	}
}