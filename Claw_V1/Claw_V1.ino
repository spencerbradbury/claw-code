#include <Servo.h>

Servo myservo;

#define servo_pin 7
#define trig 3
#define echo 4

long distance, averageDistance;
long d0, d1, d2, d3, d4;
unsigned long startTime, time;

// This is how close you have to have the claw to the surface to open.
#define THRESHOLD 20

//The start and end position of the servo. These will NEED to be adjusted to test before attatching to the claw or it will likely break
#define closedPos 0
#define openPos 90

void setup() {
  Serial.begin(9600);

  myservo.attach(servo_pin);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  d0 = d1 = d2 = d3 = d4 = THRESHOLD;
}

void loop() {
  
  distance = getDistance();
  averageDistance = rollingAverage(distance);

  if (averageDistance < THRESHOLD) {
    myservo.write(openPos);
    wait(5);
    myservo.write(closedPos);
    wait(5);
  }
  else {
    myservo.write(closedPos);
  }
}

//Get distance to table surface
long getDistance() {
  long cm, duration;

  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);

  cm = microsecondsToCentimeters(duration);

  Serial.print(cm);
  Serial.println("cm");

  return (cm);
}

//Conversion
long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}


//Compute a Rolling Average
long rollingAverage(long distance) {
  long value;

  d4 = d3;
  d3 = d2;
  d2 = d1;
  d1 = d0;
  d0 = distance;

  value = (d0 + d1 + d2 + d3 + d4) / 5;
  return (value);
}

//wait specified duration in seconds
//continues to measure distance and calculate rolling average
void wait(int duration){
  startTime = millis();
  time = millis();
  
  while ((time - startTime) < duration*1000){
    distance = getDistance();
    rollingAverage(distance);
    time = millis();
  }
}
