#include <ESP32Servo.h>

#define INPUT_PIN_one 5
#define INPUT_PIN_two 4

#define TRIG_PIN_1 26
#define ECHO_PIN_1 25
const int servoPin1 = 11;  //mag trial and error kayo doon sa mga available pins pa, kung saan gagana yung servo motor   

#define TRIG_PIN_2 16
#define ECHO_PIN_2 17
const int servoPin2 = 11;  //mag trial and error kayo doon sa mga available pins pa, kung saan gagana yung servo motor    

#define TRIG_PIN_3 18
#define ECHO_PIN_3 19
const int servoPin3 = 11; //mag trial and error kayo doon sa mga available pins pa, kung saan gagana yung servo motor     

#define BIN_HEIGHT 50  // Baguhin niyo ito based doon sa height ng trashbin niyo in cm yung unit

const int LID_OPEN_ANGLE = 180;  // Lid open position in degrees
const int LID_CLOSE_ANGLE = 0;   // Lid closed position in degrees
const int STEP_DELAY = 5;        // Delay between steps
const int ANGLE_STEP = 10;       // Angle step for servo movement
Servo lidServo1, lidServo2, lidServo3;  



const int buzzerPin1 = 8;  // any available pin
const int buzzerPin2 = 9;  // any available pin
const int buzzerPin3 = 10; // any available pin

String material_text;

void setup() {
    Serial.begin(115200);
    pinMode(INPUT_PIN_one, INPUT);
    pinMode(INPUT_PIN_two, INPUT);

    pinMode(TRIG_PIN_1, OUTPUT);
    pinMode(ECHO_PIN_1, INPUT);

    pinMode(TRIG_PIN_2, OUTPUT);
    pinMode(ECHO_PIN_2, INPUT);

    pinMode(TRIG_PIN_3, OUTPUT);
    pinMode(ECHO_PIN_3, INPUT);

    lidServo1.attach(servoPin1, 500, 2400);  
    lidServo2.attach(servoPin2, 500, 2400);
    lidServo3.attach(servoPin3, 500, 2400);
    
    lidServo1.write(LID_CLOSE_ANGLE);
    lidServo2.write(LID_CLOSE_ANGLE);
    lidServo3.write(LID_CLOSE_ANGLE);

    pinMode(buzzerPin1, OUTPUT);
    pinMode(buzzerPin2, OUTPUT);
    pinMode(buzzerPin3, OUTPUT);
}

void loop() {
    int state_one = digitalRead(INPUT_PIN_one);
    int state_two = digitalRead(INPUT_PIN_two);

    if (state_one == 0 && state_two == 0) {
        material_text = "Unknown";
        closeLid(1);
        closeLid(2);
        closeLid(3);        
    } 
    else if (state_one == 1 && state_two == 0) {
        material_text = "Metal";
        openLid(1);
        closeLid(2);
        closeLid(3);
        delay(5000);
        closeLid(1);
        closeLid(2);
        closeLid(3); 
    } 
    else if (state_one == 0 && state_two == 1) {
        closeLid(1);
        openLid(2);
        closeLid(3);
        delay(5000);
        closeLid(1);
        closeLid(2);
        closeLid(3);         
    } 
    else if (state_one == 1 && state_two == 1) {
        material_text = "Paper";
        closeLid(1);
        closeLid(2);
        openLid(3);
        delay(5000);
        closeLid(1);
        closeLid(2);
        closeLid(3);   
    }
    Serial.println("Detected Material: " + material_text);
    digitalWrite(INPUT_PIN_one, LOW);  
    digitalWrite(INPUT_PIN_two, LOW);
    delay(1000);  

    float freeSpace1 = measureFreeSpace(TRIG_PIN_1, ECHO_PIN_1);
    float freeSpace2 = measureFreeSpace(TRIG_PIN_2, ECHO_PIN_2);
    float freeSpace3 = measureFreeSpace(TRIG_PIN_3, ECHO_PIN_3);
  
    Serial.print("TB 1 FS: "); Serial.println(freeSpace1);
    Serial.print("TB 2 FS: "); Serial.println(freeSpace2);
    Serial.print("TB 3 FS: "); Serial.println(freeSpace3);
  
  if (freeSpace1 > 95) 
  {
    Serial.println("Opening lids 3");
    openLid(1);
    buzz(1);
    delay (1000);
    stopBuzz(1);

  } 
  else if (freeSpace2 > 95) 
  {
    Serial.println("Opening lids 2");
    openLid(2);
    buzz(2);
    delay (1000);
    stopBuzz(2);

  } 
  else if (freeSpace3 > 95) 
  {
    Serial.println("Opening lid 3");
    openLid(3);
    buzz(3);
    delay (1000);
    stopBuzz(3);
  } 
  else 
  {
    Serial.println("All lids are close");
    closeLid(1);
    closeLid(2);
    closeLid(3);
    stopBuzz(1);
    stopBuzz(2);
    stopBuzz(3);
  }
}

void openLid(int servo) {
  Servo* lidServo;
  if (servo == 1) lidServo = &lidServo1;
  else if (servo == 2) lidServo = &lidServo2;
  else if (servo == 3) lidServo = &lidServo3;
  else return;

  for (int angle = LID_CLOSE_ANGLE; angle <= LID_OPEN_ANGLE; angle += ANGLE_STEP) {
    lidServo->write(angle);
    delay(STEP_DELAY);
  }
  lidServo->write(LID_OPEN_ANGLE);
}

void closeLid(int servo) {
  Servo* lidServo;
  if (servo == 1) lidServo = &lidServo1;
  else if (servo == 2) lidServo = &lidServo2;
  else if (servo == 3) lidServo = &lidServo3;
  else return;

  for (int angle = LID_OPEN_ANGLE; angle >= LID_CLOSE_ANGLE; angle -= ANGLE_STEP) {
    lidServo->write(angle);
    delay(STEP_DELAY);
  }
  lidServo->write(LID_CLOSE_ANGLE);
}

float measureFreeSpace(int trigPin, int echoPin) {
  long duration;
  int distance;
  float freeSpacePercent;
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH, 30000);
  distance = duration * 0.034 / 2;
  freeSpacePercent = ((float)distance / BIN_HEIGHT) * 100;
  return constrain(freeSpacePercent, 0, 100);
}

void buzz(int buzzer) {
  int buzzerPin;
  if (buzzer == 1) buzzerPin = buzzerPin1;
  else if (buzzer == 2) buzzerPin = buzzerPin2;
  else if (buzzer == 3) buzzerPin = buzzerPin3;
  else return;
  digitalWrite(buzzerPin, HIGH);
}

void stopBuzz(int buzzer) {
  int buzzerPin;
  if (buzzer == 1) buzzerPin = buzzerPin1;
  else if (buzzer == 2) buzzerPin = buzzerPin2;
  else if (buzzer == 3) buzzerPin = buzzerPin3;
  else return;
  digitalWrite(buzzerPin, LOW);
}
