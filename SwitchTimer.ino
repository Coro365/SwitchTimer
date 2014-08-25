#include <Servo.h>

//pinNo
#define SERVOPIN	10
#define LED			13
#define BUTTON		12

//servo set
#define ON			130
#define DEFO		100
#define OFF			70

//millisec
#define HOUR		3600000
#define MINUTE		60000
#define SEC			1000

//Defolt Time
#define DEFOTIME	4

//Instance
Servo g_servo;


//7Segment Display (OSL10561-IB)
boolean pattern[11][8]={

	//Number[10][8](Not dot)
	{0,0,0,1,0,0,0,1}, //0
	{1,1,0,1,0,1,1,1}, //1
	{0,0,1,1,0,0,1,0}, //2
	{1,0,0,1,0,0,1,0}, //3
	{1,1,0,1,0,1,0,0}, //4
	{1,0,0,1,1,0,0,0}, //5
	{0,0,0,1,1,0,0,0}, //6
	{1,1,0,1,0,0,0,1}, //7
	{0,0,0,1,0,0,0,0}, //8
	{1,1,0,1,0,0,0,0}, //9

	//Black
	{1,1,1,1,1,1,1,1} //Not light

};


void setup() {
	Serial.begin(9600);

	//7Seg ModeSet
	for(int i=2; i<=9; i++){
		pinMode(i, OUTPUT);
	}

	//Servo
	g_servo.attach(SERVOPIN);
	g_servo.write(DEFO);

	//Button
	digitalWrite(BUTTON,INPUT_PULLUP);

	digitalWrite(LED, HIGH); 			//Run LED ON
	displayNum(0);						//Display Time Number
	Serial.print("SetUp OK!\n");
}


void loop() { 

	//Set val
	static unsigned long time = 0;
	static unsigned long limitTime = 0;

	//Flag
	static boolean firstloop = true;
	static boolean firstservo = true;

	//Set time
	if (firstloop == true){
		
		//Limit time set (hour)
		limitTime = HOUR * setTime();

		//Flag
		firstloop = false;

		//Debug
		Serial.print("Time set :");
		Serial.print(limitTime / HOUR);
		Serial.print("hour\n");

	}

	//Since Up Arduino(ms)
	time = millis();

	//Debug
	Serial.print(time / SEC);
	Serial.print("sec\n");

	//Time Up
	if (time > limitTime && firstservo == true){
		
		//Display Flash
		displayNumF(limitTime / HOUR);

		//Servo
		//on();
		off();

		//Run LED OFF
		digitalWrite(LED, LOW); 

		//Flag
		firstservo = false;
		
	}
	
	//Check Space
	delay(1 * SEC);
} 



int setTime(){

	//Val set
	int buttonCount = 0;
	int lastButtonStatus = LOW;
	int bottonStatus = LOW;

	//Time set
	long setStart = millis();
	long lastTime = 0;

	//Debug
	Serial.print("Time Set Start\n");

	//15sec loop
	while(1){

		//Now Time set
		long nowTime = millis();

		//Read Button
		int bottonStatus = digitalRead(BUTTON);
        Serial.print(bottonStatus);
        delay(50);

        //Push Button
		if(lastButtonStatus == HIGH && bottonStatus == LOW){

			buttonCount++;

			//Display
			displayNum(buttonCount);

			//Time set of Push button
			lastTime = millis();

			//Debug
			Serial.print("\nbuttonCount ");
			Serial.print(buttonCount);
			Serial.print("\n");

		}

		//3sec of Since last push 
		if(nowTime - lastTime > 3 * SEC && buttonCount != 0){
			Serial.print("\n3sec of Since last push\n");
			break;
		}

		//No Push Button and 5sec
		if(nowTime - setStart > 5 * SEC && buttonCount == 0){
			Serial.print("\nNo Push Button and 5sec\n");
			buttonCount = DEFOTIME;
			break;
		}

		//Status(Delete Long Push)
		lastButtonStatus = bottonStatus;

	}

	//Display Flash and Hide
	displayNumF(buttonCount);
	displayRefresh();

	return buttonCount;
}



//7 Seg Desplay
void displayNum(int num){
	for(int i=0; i<=8; i++){
		digitalWrite(i+2, pattern[num][i]);
	}

}

void displayNumF(int num){
	for(int i = 0; i < 10; i++){
		displayNum(num);
		delay(100);
		displayRefresh();
		delay(100);
	}

}

void displayRefresh(){
	for(int i=0; i<=8; i++){
		digitalWrite(i+2, pattern[10][i]);
	}
}



//Servo　motion
void on(){
	Serial.print("on!\n");

	for (int i=DEFO; i>=OFF; i--){
		g_servo.write(i);
	} 
	delay(1000);
	g_servo.write(DEFO);
}

void off(){
	Serial.print("off!\n");

	for (int i=DEFO; i<=ON; i++){
		g_servo.write(i);
	} 
	delay(1000);
	g_servo.write(DEFO);
}

void defo(){
	g_servo.write(DEFO);
}
