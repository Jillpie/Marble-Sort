#pragma config(Sensor, in1,    sensorLine,     sensorLineFollower)
#pragma config(Sensor, in2,    sensorLight,    sensorReflection)
#pragma config(Sensor, in3,    potLine,        sensorPotentiometer)
#pragma config(Sensor, dgtl1,  sensorEncoder,  sensorQuadEncoder)
#pragma config(Sensor, dgtl11, button1,        sensorTouch)
#pragma config(Sensor, dgtl12, button2,        sensorTouch)
#pragma config(Motor,  port1,           colorFlashLight, tmotorVexFlashlight, openLoop, reversed)
#pragma config(Motor,  port5,           lineMotor,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           tickServo,     tmotorServoStandard, openLoop)

//VARIBLES AND CONSTANTS:
	//Task Main FLow Control and Debug:
		bool stage = false;

	//Planning Functions:
		int SEAVALUE = 20;

		//Solid Glass Plan Sea:
			bool marbleTypeLine;
			int solidGlassSolid;
			int solidGlassGlass;
			bool solidGlassFin = false;
			int LINESOLID = 2500;
			int LINETRESH = 50;

		//Color Glass Plan Sea:
			bool marbleColorLight;
			int colorGlassGlass;
			int colorGlassColor;
			bool colorGlassFin = false;
			int LIGHTCOLOR = 300;
			int COLORTRESH = 5;

	//Action Functions:
		int ACTIONTIME = 4000;

		//Servo Action:
			int SERVOLETIN = -110;
			int SERVOLETOUT = 110;

	//Other Functions:
		//Pistion Action:
			bool pistionStage = false;
			int PISTIONMIN = 300;
			int PISTIONMAX = 400;


	//Universal Constants:
		int POTFALLRIGHT = 650;
		int POTFALLLEFT = 3200;
		int POTNEUTRAL= 1900;
		int MOTORPOWER = 21;
		int POTFALLRIGHT = 650;
		int POTFALLLEFT = 3200;
		int POTNEUTRAL= 1900;

//FUNCTIONS:
	void solidGlassPlanSea(){
		if(solidGlassFin == false){
			if(SensorValue(sensorLine) < LINESOLID + LINETRESH){
				solidGlassSolid ++;
				solidGlassGlass = 0;
			}
			if(SensorValue(sensorLine) > LINESOLID - LINETRESH){
				solidGlassSolid = 0;
				solidGlassGlass ++;
			}
			if(solidGlassGlass >= SEAVALUE){
				solidGlassFin = true;
				marbleTypeLine = true;
			}
			if(solidGlassSolid >= SEAVALUE){
				solidGlassFin = true;
				marbleTypeLine = false;
			}
		}
	}

	void colorGlassPlanSea(){
		if(colorGlassFin == false){
			if(SensorValue(sensorLight) < LIGHTCOLOR + COLORTRESH){
				colorGlassColor ++;
				colorGlassGlass = 0;
			}
			if(SensorValue(sensorLight) > LIGHTCOLOR - COLORTRESH){
				colorGlassColor = 0;
				colorGlassGlass ++;
			}
			if(colorGlassGlass >= SEAVALUE){
				colorGlassFin = true;
				marbleColorLight = true;
			}
			if(colorGlassColor >= SEAVALUE){
				colorGlassFin = true;
				marbleColorLight = false;
			}
		}
	}

	void servoAction(){
		if(time1(T1) > ((3 * ACTIONTIME)/4) && time1(T1) < ACTIONTIME){
			startMotor(tickServo,SERVOLETIN);
		}
		if(time1(T1) >= ACTIONTIME){
			startMotor(tickServo,SERVOLETOUT);
		}
	}

	void pistionAction(){
		if(pistionStage == true){
			startMotor(pistionMotor,MOTORPOWER);
			if(SensorValue(potPistion) > PISTIONMAX){
				pistionStage = false;
			}
		}
		if(pistionStage == false){
			startMotor(pistionMotor,-MOTORPOWER);
			if(SensorValue(potPistion) < PISTIONMIN){
				pistionStage = true;
			}
		}
	}

	void solidGlassAction(){
		if((time1(T1) > ((2 * ACTIONTIME)/4)) && (time1(T1) < ((3 * ACTIONTIME)/4))){
			if(marbleTypeLine == false){
				if(SensorValue(potLine) > POTFALLRIGHT){
					startMotor(lineMotor,MOTORPOWER);
				}
				else{
					startMotor(lineMotor,0);
				}
			}
			if(marbleTypeLine == true){
				if(SensorValue(potLine) < POTFALLLEFT){
					startMotor(lineMotor,-MOTORPOWER);
				}
				else{
					startMotor(lineMotor,0);
				}
			}
		}
		if(time1(T1) > (3 * ACTIONTIME)/4){
			if(SensorValue(potLine) < POTNEUTRAL + 100){
				startMotor(lineMotor,-MOTORPOWER);
			}
			if(SensorValue(potLine) > POTNEUTRAL - 100){
				startMotor(lineMotor,MOTORPOWER);
			}
			if((SensorValue(potLine) < POTNEUTRAL + 100) && (SensorValue(potLine) > POTNEUTRAL - 100)){
				startMotor(lineMotor,0);
			}
		}
	}

	void colorGlassAction(){
		if((time1(T1) > ((2 * ACTIONTIME)/4)) && (time1(T1) < ((3 * ACTIONTIME)/4))){
			if(marbleTypeLine == false){
				if(SensorValue(potLine) > POTFALLRIGHT){
					startMotor(lineMotor,MOTORPOWER);
				}
				else{
					startMotor(lineMotor,0);
				}
			}
			if(marbleTypeLine == true){
				if(SensorValue(potLine) < POTFALLLEFT){
					startMotor(lineMotor,-MOTORPOWER);
				}
				else{
					startMotor(lineMotor,0);
				}
			}
		}
		if(time1(T1) > (3 * ACTIONTIME)/4){
			if(SensorValue(potLine) < POTNEUTRAL + 100){
				startMotor(lineMotor,-MOTORPOWER);
			}
			if(SensorValue(potLine) > POTNEUTRAL - 100){
				startMotor(lineMotor,MOTORPOWER);
			}
			if((SensorValue(potLine) < POTNEUTRAL + 100) && (SensorValue(potLine) > POTNEUTRAL - 100)){
				startMotor(lineMotor,0);
			}
		}

	}

task main(){
	while(true){
		pistionAction();
		if(stage == false){
			solidGlassPlanSea();
			colorGlassPlanSea();
			clearTimer(T1);
			if((solidGlassFin == true) && (colorGlassFin == 1)){
				stage = true;
				solidGlassFin = false;
				colorGlassFin = 0;
			}
		}
		if(stage == true){
			servoAction();
			solidGlassAction();
			colorGlassAction();
			if(time1(T1) > ACTIONTIME + 1000){
				startMotor(lineMotor,0);
				stage = false;
			}
		}
	}
}
