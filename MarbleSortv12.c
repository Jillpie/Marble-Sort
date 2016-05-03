#pragma config(Sensor, in1,    sensorLine,     sensorLineFollower)
#pragma config(Sensor, in2,    sensorLight,    sensorReflection)
#pragma config(Sensor, in3,    potLine,        sensorPotentiometer)
#pragma config(Sensor, in4,    potPistion,     sensorPotentiometer)
#pragma config(Sensor, in5,    potLight,       sensorPotentiometer)
#pragma config(Sensor, in6,    potGate,    	   sensorPotentiometer)
#pragma config(Sensor, dgtl3,  eleEncoder,     sensorTouch)
#pragma config(Sensor, dgtl11, button1,        sensorTouch)
#pragma config(Sensor, dgtl12, button2,        sensorTouch)
#pragma config(Motor,  port1,           lightFlashlight, tmotorVexFlashlight, openLoop, reversed)
#pragma config(Motor,  port2,           pistionMotor,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           lightMotor,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           lineMotor,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           tickServo,     tmotorServoStandard, openLoop)
#pragma config(Motor,  port7,           jamMotor,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           elevatorMotor,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,          	gateMotor,     tmotorVex393_MC29, openLoop)

//VARIBLES AND CONSTANTS:
	//Task Main FLow Control and Debug:
		bool status = false;
		bool stage = false;

	//Planning Functions:
		int SEAVALUE = 30;

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
			int LIGHTCOLOR = 400;
			int COLORTRESH = 5;

	//Action Functions:
		int ACTIONTIME = 3000;

		//Servo Action:
			int SERVOLETIN = -110;
			int SERVOLETOUT = 110;

	//Other Functions:
		//Pistion Action:
			bool pistionStage = false;
			int PISTIONMIN = 350;
			int PISTIONMAX = 1400;

		//Jamming Action:

		//Button1 Triggered:
			bool button1Trigger = false;
			bool button1Pressed = false;

		//Button2 Triggered:
			bool button2Trigger = false;
			bool button2Pressed = false;

		//Cons:
			int CONTIME = 150000;

			//System:
				bool conToggle = false;
				int ELEPHY = 600;
				int phyI = 1;

			//Gate:
				bool gateSea = false;
				int POTGATEOPEN = 600;
				int POTGATECLOSE = 200;

	//Universal Constants:
		int POTFALLRIGHT = 650;
		int POTFALLLEFT = 3200;
		int POTNEUTRAL= 1900;
		int MOTORPOWER = 21;

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
		turnFlashlightOn(lightFlashlight, 127);
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

	void servoAction(){							//Controls the servo tick; or intake of the marbles
		if(time1(T1) > ((3 * ACTIONTIME)/4) && time1(T1) < ACTIONTIME){	//the 3rd time block 
			startMotor(tickServo,SERVOLETIN);	//Postitions servo to "LET IN" a marble in side of it 
		}
		if(time1(T1) >= ACTIONTIME){			//Time block 4 or the ending portion of the ACTION TIME
			startMotor(tickServo,SERVOLETOUT);	//Positions servo to Nutral letting out the marble it was holding in
		}
	}

	void pistionAction(){						//Defines how to move pistion motor to help hopper
		if(pistionStage == true){				//Pistion works with two phases, push and pull; ergo two positions required
			startMotor(pistionMotor,MOTORPOWER);//Sets power to the motor contoling the pistion or prodding axle
			if(SensorValue(potPistion) > PISTIONMAX){
				pistionStage = false;			//Makes sure after a certain distance the phase changes between push or pull
			}
		}
		if(pistionStage == false){				//Secound stage of the piston( I think this should be the pull part? but it dont matter)
			startMotor(pistionMotor,-MOTORPOWER);
			if(SensorValue(potPistion) < PISTIONMIN){
				pistionStage = true;
			}
		}
	}

	void jammingAction(){
		startMotor(jamMotor, MOTORPOWER * -2);
	}

	void solidGlassAction(){					//
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

	void colorGlassAction(){					//
		if((time1(T1) > ((1 * ACTIONTIME)/4)) && (time1(T1) < ((2 * ACTIONTIME)/4))){
			if(marbleColorLight == false){
				if(SensorValue(potLight) > POTFALLRIGHT){
					startMotor(lightMotor,MOTORPOWER);
				}
				else{
					startMotor(lightMotor,0);
				}
			}
			if(marbleColorLight == true){
				if(SensorValue(potLight) < POTFALLLEFT){
					startMotor(lightMotor,-MOTORPOWER);
				}
				else{
					startMotor(lightMotor,0);
				}
			}
		}
		if(time1(T1) > (2 * ACTIONTIME)/4){
			if(SensorValue(potLight) < POTNEUTRAL + 100){
				startMotor(lightMotor,-MOTORPOWER);
			}
			if(SensorValue(potLight) > POTNEUTRAL - 100){
				startMotor(lightMotor,MOTORPOWER);
			}
			if((SensorValue(potLight) < POTNEUTRAL + 100) && (SensorValue(potLight) > POTNEUTRAL - 100)){
				startMotor(lightMotor,0);
			}
		}
	}

	void resetActu(){
		startMotor(pistionMotor, 0);
		startMotor(lightMotor, 0);
		startMotor(lineMotor, 0);
		startMotor(jamMotor, 0);
		startMotor(gateMotor, 0);
		startMotor(elevatorMotor, 0);
	}

	void button1Triggered(){
		if(SensorValue(button1) == 1 ){
			button1Trigger = true;
		}
		if((SensorValue(button1) == 0) && (button1Pressed == true)){
			button1Pressed = true;
		}
	}

	void button2Triggered(){
		if(SensorValue(button2) == 1 ){
			button2Trigger = true;
		}
		if((SensorValue(button2) == 0) && (button2Pressed == true)){
			button2Pressed = true;
		}
	}

	void statusControl(){
		if((status == false) && (button1Pressed == true)){
			button1Pressed = false;
			status = true;
		}
		if((status == true) && (button1Pressed == true)){
			button1Pressed = false;
			status = false;
			resetActu();
		}
	}

	void conGate(){
		if(gateSea == true){
			if(SensorValue(potGate) <= POTGATEOPEN){
				startMotor(gateMotor, MOTORPOWER * -2);
			}else{
				gateSea = false;
			}
		}
		if(gateSea == false){
			if(SensorValue(potGate) >= POTGATECLOSE){
				startMotor(gateMotor, MOTORPOWER * 2);
			}else{
				startMotor(gateMotor, 0);
			}
		}
	}

	void conSystem(){
		conGate();
		if(((button2Pressed == true) && (conToggle == false)) || time1(T3) >= CONTIME){
			startMotor(elevatorMotor,MOTORPOWER);
			if(SensorValue(eleEncoder) >= (ELEPHY * phyI)){
				phyI ++;
				gateSea = true;
			}
			conToggle = true;
		}
		if((button2Pressed == true) && (conToggle == true)){
			resetActu();
			clearTimer(T3);
			conToggle = false;
		}
	}

task main(){
	while(true){								//Main Loop everytihng should be in; unless it should be ran only once
		button1Triggered();
		button2Triggered();
		statusControl();
		if(status == true){
			conSystem();
			pistionAction();						//Not part of a phase; preforms piston function 
			jammingAction();
			if(stage == false){						//If planning stage:
				solidGlassPlanSea();				//Run these functions
				colorGlassPlanSea();
				if((solidGlassFin == true) && (colorGlassFin == true)){
					stage = true;					//Sets the phase to Action unlimatly making planning complete
					solidGlassFin = false;
					colorGlassFin = false;			//Sets the planning stage functions to false making the program run planning when its done after action
					clearTimer(T1);					//Clears timmer so timmer is 0 at start of action phase
				}
			}
			if(stage == true){						//Action phase or stage
				servoAction();						//functions that are defined by the planning as to waht they should do during action
				solidGlassAction();
				colorGlassAction();
				if(time1(T1) > ACTIONTIME + 1000){	//Defines the ending of the Action phase by action time + 1 sec
					startMotor(lineMotor,0);		//Sets sensor modual motors to stop moving in case they're still moving; as they should not be moving in Planning phase
					startMotor(lightMotor,0);
					stage = false;					//Sets phase to Planning 
				}
			}
		}
	}
}
