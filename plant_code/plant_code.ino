#include <Servo.h>


int solenoidPin = 4; //output pin to use
int soilPin = A1; // soil moisture analog
int soilPower = 7; // soil moisture + pin
int lightPin = A3; // photoresistor signal
int lightPower = 3; // photoresistor + pin

// unsigned long currentTime;
// unsigned long oldTime;

int soilMax = 1000; //max threshold for soil moisture, subject to change
int soilMin = 700; //min threshold for soil moisture, subject to change
int maxWaterTime = 5000; // max amt of time(millis) for watering
int lightThreshold = 70; // trigger threshold from photoresistor


Servo servo;
int pos = 0;

bool isShaded = false;

bool isSolenoidRunning = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(solenoidPin, OUTPUT); // sets pin as power output for solenoid
  pinMode(soilPower, OUTPUT); // sets pin for power output for moisture sensor
  pinMode(lightPower, OUTPUT); // sets pin for power output
  servo.attach(9); 
  digitalWrite(soilPower, LOW);
  Serial.begin(9600);
  // oldTime = millis(); // set initial time
  // currentTime = oldTime;

}

void loop() {
  int curMoisture = readSoil();
  // if the solenoid is not running and the moisture is less than the minimum
  // turns on solenoid valve
  if(!isSolenoidRunning && curMoisture < soilMin)
  {
     isSolenoidRunning = true;
     digitalWrite(solenoidPin, HIGH);
     // oldTime = millis(); // reset old time
     // currentTime = oldTime;
  }

  // if the solenoid is running and the moisture is greater than the maximum
  // turn off the solenoid valve
  if(isSolenoidRunning)
  {
    // currentTime = millis(); // change current time
    // set max duration that the solenoid remains open, check if it exceeds it.
    // unsigned long deltaTime = currentTime - oldTime;
    // compare if deltaTime exceeds max
    if(curMoisture >= soilMax)
    {
      // oldTime = currentTime;
      isSolenoidRunning = false;
      digitalWrite(solenoidPin, LOW);
    }
  }

  int curLight = readLight();
  // put your main code here, to run repeatedly:
  if(!isShaded && curLight > lightThreshold)
  {
    for(pos; pos >= 0; pos--)
    {
      servo.write(pos);
      delay(10);
    }
    delay(200);
    isShaded = true;
  }
  if(isShaded && curLight <= lightThreshold)
  {
    for(pos; pos <= 90; pos++)
    {
      servo.write(pos);
      delay(10);
    }
    delay(200);
    isShaded = false;
  }
  delay(1000);
}

/*
 * method that sends power to the soil moisture sensor, reads its output value, turns off the power, and
 * returns the moisture read.
 */
int readSoil()
{
  digitalWrite(soilPower,HIGH);
  int val = analogRead(soilPin);
  digitalWrite(soilPower,LOW);
  Serial.println(val);
  return val;
}

int readLight()
{
  digitalWrite(lightPower,HIGH);
  int val = analogRead(lightPin);
  digitalWrite(lightPower,LOW);
  Serial.print("light val: ");
  Serial.println(val);
  return val;
}
