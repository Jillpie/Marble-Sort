





task main(){
	int values[100];
	for(int i = 0; i < 100; i++){
 	 values[i] = SensorValue[light];
	  wait1Msec(100);
	}

	int arrayTotal
	float arrayAvg;
	int array98 = values[98];
	int array99 = values[99];

	for(int p = 0; p < 100; p++){
		arrayTotal =+ values[p];
	}
	arrayAvg = arrayTotal / 99;    //maybe its /100? 
} 

//conclustion: This has not been tested, further testing is required.