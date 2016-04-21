
//VARLIBES AND CONSTANTS:
	int stage = 1;
	int ACTIONTIME = 4000;
	int PLANNINGAVG

	int SERVOLETIN;
	int SERVOLETOUT;

	int solidGlassTol = 0;
	int solidGlassI = 0;
	int solidGlassAvg = 0;
	int solidGlassFin = 0;
	int LINESOLID = 2200;
	int LINETRESH = 50;

	int POTFALLRIGHT = 100;
	int POTFALLLEFT = 2200;
	int POTNEUTRAL= 1100;


//FUNCTIONS:
	void planReset(){
		//solidGlassPLan:
			solidGlassTol = 0;
			solidGlassI = 0;
			solidGlassAvg = 0;
			solidGlassFin = 0;
			LINESOLID = 2200;
			LINETRESH = 50;
	}

	void solidGlassPlanAvg(){
		if(solidGlassFin == 0){
			solidGlassTol =+ SensorValue(sensorLine);
			solidGlassI ++;
			solidGlassAvg = solidGlassTol / solidGlassI;

			if(solidGlassI >= PLANNINGAVG){
				solidGlassFin = 1;
				if(solidGlassAvg < LINESOLID + LINETRESH){
					marbleTypeLine = 1;
				}
				else{
					marbleTypeLine = 2;
				}
			}
		}
	}

	void solidGlassPlanSea(){
		if(solidGlassFin == 0){
			if(SensorValue(sensorLine) < LINESOLID + LINETRESH){
				solidGlassSolid ++;
				solidGlassglass = 0;
			}
			if(SensorValue(sensorLine) > LINESOLID - LINETRESH){
				solidGlassSolid = 0;
				solidGlassglass ++;
			}
			if(solidGlassGlass >= SEAVALUE){
				solidGlassFin = 1;
				marbleTypeLine = 2;
			}
			if(solidGlassSolid >= SEAVALUE){
				solidGlassFin = 1;
				marbleTypeLine = 1;
			}
		}
	}

	void servoAction(){
		if(time1(T1) > (3*ACTIONTIME)/4) && time1(T1) < ACTIONTIME){
			startMotor(tickServo,SERVOLETIN);
		}
		if(time1(T1) >= ACTIONTIME){
			startMotor(tickServo,SERVOLETOUT);
		}
	}

	void solidGlassAction(){
		if((time1(T1) > (2 * ACTIONTIME)/4) && (time1(T1) < (3 * ACTIONTIME)/4))){
			if(marbleTypeLine == 1){
				if(SensorValue(potLine) > POTFALLRIGHT){
					startMotor(lineMotor,MOTORPOWER);
				}
				else{
					startMotor(lineMotor,0);
				}
			}
			if(marbleTypeLine == 2){
				if(SensorValue(potLine) > POTFALLLEFT){
					startMotor(lineMotor,-MOTORPOWER);
				}
				else{
					startMotor(lineMotor,0);
				}
			}
		}
		if(time1(T1) > (3 * ACTIONTIME)/4){
			if(SensorValue(potLine) < POTNEUTRAL){
				startMotor(lineMotor,-MOTORPOWER);
			}
			if(SensorValue(potLine) > POTNEUTRAL + 100){
				startMotor(lineMotor,MOTORPOWER);
			}
			if((SensorValue(potLine) < POTNEUTRAL + 100) && (SensorValue(potLine) > POTNEUTRAL)){
				startMotor(lineMotor,-MOTORPOWER);
			}
		}
	}

	void colorGlassAction(){

	}


task main(){
	while(true){
		if(stage == 1){
			solidGlassPlanAvg();
			//solidGlassPlanSea();
			clearTimer(T1);
			if(solidGlassFin == 1){
				stage = 2;
			}

		}
		if(stage == 2){
			servoAction();
			solidGlassAction();
			colorGlassAction();



			if(time1(T1) > ACTIONTIME + 100){
				stage = 1;
				planReset();
			}
		}
	}
}