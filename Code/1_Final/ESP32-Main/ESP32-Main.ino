//// Note: This is already fix don't change this. 
#include <ESP32Servo.h>
// ---- Responsible for Binary transfer from ESP32-Cam to ESP32-Main ---- //
#define INPUT_PIN_one 5 // (Working)
#define INPUT_PIN_two 4 // (Working)
// ---------------------------------------------------------------------------
const int LID_OPEN_ANGLE = 0;        /// Pwede niyo iadjust yung degrees ng Servo motor.
const int LID_CLOSE_ANGLE = 90;      /// Pwede niyo iadjust yung degrees ng Servo motor.
const int LID_OPEN_ANGLE_rev = 270;  /// Pwede niyo iadjust yung degrees ng Servo motor. LID 1 ONLY
const int LID_CLOSE_ANGLE_rev = 90;  /// Pwede niyo iadjust yung degrees ng Servo motor. LID 1 ONLY
const int STEP_DELAY = 5;            /// Delay per added angle.
const int ANGLE_STEP = 10;           /// Angle per step delay.
Servo lidServo1, lidServo2, lidServo3;  // Don't change
String material_text;                   // Don't change
///------------------------------------------------------///

//// Note: Yung mga pins na ito ay para sa Ultrasonic sensor and sa Servo Motor ng Lid para Bin #3.
///        Mag trial and error kayo doon sa mga available pins pa, kung saan gagana yung servo motor.
#define TRIG_PIN_1 26 // (Working)
#define ECHO_PIN_1 25 // (Working)
const int servoPin1 = 2; //mag trial and error kayo doon sa mga available pins pa, kung saan gagana yung servo motor.  

//// Note: Yung mga pins na ito ay para sa Ultrasonic sensor and sa Servo Motor ng Lid para Bin #3.
////       Mag trial and error kayo doon sa mga available pins pa, kung saan gagana yung servo motor.
#define TRIG_PIN_2 19 // (Working)
#define ECHO_PIN_2 18 // (Working)
const int servoPin2 = 27; //mag trial and error kayo doon sa mga available pins pa, kung saan gagana yung servo motor.     

//// Note: Yung mga pins na ito ay para sa Ultrasonic sensor and sa Servo Motor ng Lid para Bin #3.
////       Mag trial and error kayo doon sa mga available pins pa, kung saan gagana yung servo motor.     
#define TRIG_PIN_3 16 // (Working)
#define ECHO_PIN_3 17 // (Working) 
const int servoPin3 = 14; //mag trial and error kayo doon sa mga available pins pa, kung saan gagana yung servo motor.     

//// Note: Baguhin niyo ito based doon sa height ng trashbin niyo in "cm" yung unit.
////       But do the calibration test, since na-test ko ito na 200 cm yung maximum distance.
#define MIN_DISTANCE 2   // Closest distance (100%) in cm
#define MAX_DISTANCE 200 // Farthest distance (0%)  in cm

//// Note: Trail and Error din kayo sa available pins, choose Digital Pin. Check for the pinouts for ESP32.
////       https://www.upesy.com/cdn/shop/files/doc-esp32-pinout-reference-wroom-devkit.png?width=692
const int buzzerPin1 = 35;  // (Working)
const int buzzerPin2 = 34;  // (Working) 
const int buzzerPin3 = 32;  // (Working) 

/// --------------------Added Pins for Moisture Sensor---------------//
#define moisture_sensor 13 /// change this if not applicable
int moist_threshold = 600; /// change the threshold value if needed

// ---------- Don't change --------------- //
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
// ------------------------------------------------ //
// Note: for this loop, you change the behavior each situation. 
void loop() {
    int state_one = digitalRead(INPUT_PIN_one);
    int state_two = digitalRead(INPUT_PIN_two);
    int wet_waste = analogRead(moisture_sensor);

    if (state_one == 0 && state_two == 0) 
    {
        material_text = "Unknown";
        closeLid_rev(1);
        closeLid(2);
        closeLid(3);        
    } 
    else if (wet_waste < moist_threshold) 
    {
        material_text = "Wet";
        openLid_rev(1);
        closeLid(2);
        closeLid(3);
        delay(5000);
        closeLid_rev(1);
        closeLid(2);
        closeLid(3); 
    } 
    else if (state_one == 0 && state_two == 1) 
    {
        closeLid_rev(1);
        openLid(2);
        closeLid(3);
        delay(5000);
        closeLid_rev(1);
        closeLid(2);
        closeLid(3);         
    } 
    else if (state_one == 1 && state_two == 1) 
    {
        material_text = "Paper";
        closeLid_rev(1);
        closeLid(2);
        openLid(3);
        delay(5000);
        closeLid_rev(1);
        closeLid(2);
        closeLid(3);   
    }

    Serial.println("Detected Material: " + material_text);
    digitalWrite(INPUT_PIN_one, LOW);  
    digitalWrite(INPUT_PIN_two, LOW);
    delay(500);  

    float freeSpace1 = measureFreeSpace(TRIG_PIN_1, ECHO_PIN_1);
    float freeSpace2 = measureFreeSpace(TRIG_PIN_2, ECHO_PIN_2);
    float freeSpace3 = measureFreeSpace(TRIG_PIN_3, ECHO_PIN_3);
  
    Serial.print("TB 1 FS: "); Serial.println(freeSpace1);
    Serial.print("TB 2 FS: "); Serial.println(freeSpace2);
    Serial.print("TB 3 FS: "); Serial.println(freeSpace3);

  //// Note: During calibration, change the value of 95, percentage to, meaning if greater than 95% na yung bin, 
  ////       mag oopen yung lid and magbu-buzz yung specific bin.
  if (freeSpace1 > 95) 
  {
    Serial.println("Opening lid 1 and buzz sound");
    openLid_rev(1);
    buzz(1);
    delay (5000);
    stopBuzz(1);
  } 
  //// Note: During calibration, change the value of 95, percentage to, meaning if greater than 95% na yung bin, 
  ////       mag oopen yung lid and magbu-buzz yung specific bin.
  else if (freeSpace2 > 95) 
  {
    Serial.println("Opening lid 2 and buzz sound");
    openLid(2);
    buzz(2);
    delay (5000);
    stopBuzz(2);

  } 
  //// Note: During calibration, change the value of 95, percentage to, meaning if greater than 95% na yung bin, 
  ////       mag oopen yung lid and magbu-buzz yung specific bin.
  else if (freeSpace3 > 95) 
  {
    Serial.println("Opening lid 3 and buzz sound");
    openLid(3);
    buzz(3);
    delay (5000);
    stopBuzz(3);
  } 
  //// Note: If yung bin niyo ay hindi pa puno, close lids pa and mags-stop yung buzz. 
  ////       You can adjust yung mga behavior if you want to, like yung delays.
  else 
  {
    Serial.println("All lids are close");
    closeLid_rev(1);
    closeLid(2);
    closeLid(3);
    stopBuzz(1);
    stopBuzz(2);
    stopBuzz(3);
  }
}

// -------------------------- Default Functions nothing to change --------------------------
void openLid(int servo) 
{
  Servo* lidServo;
  if (servo == 1) lidServo = &lidServo1;
  else if (servo == 2) lidServo = &lidServo2;
  else if (servo == 3) lidServo = &lidServo3;
  else return;

  for (int angle = LID_CLOSE_ANGLE; angle <= LID_OPEN_ANGLE; angle += ANGLE_STEP) 
  {
    lidServo->write(angle);
    delay(STEP_DELAY);
  }
  lidServo->write(LID_OPEN_ANGLE);
}

void closeLid(int servo) 
{
  Servo* lidServo;
  if (servo == 1) lidServo = &lidServo1;
  else if (servo == 2) lidServo = &lidServo2;
  else if (servo == 3) lidServo = &lidServo3;
  else return;

  for (int angle = LID_OPEN_ANGLE; angle >= LID_CLOSE_ANGLE; angle -= ANGLE_STEP) 
  {
    lidServo->write(angle);
    delay(STEP_DELAY);
  }
  lidServo->write(LID_CLOSE_ANGLE);
}

void openLid_rev(int servo) 
{
  Servo* lidServo;
  if (servo == 1) lidServo = &lidServo1;
  else if (servo == 2) lidServo = &lidServo2;
  else if (servo == 3) lidServo = &lidServo3;
  else return;

  for (int angle = LID_CLOSE_ANGLE_rev; angle <= LID_OPEN_ANGLE_rev; angle += ANGLE_STEP) 
  {
    lidServo->write(angle);
    delay(STEP_DELAY);
  }
  lidServo->write(LID_OPEN_ANGLE_rev);
}

void closeLid_rev(int servo) 
{
  Servo* lidServo;
  if (servo == 1) lidServo = &lidServo1;
  else if (servo == 2) lidServo = &lidServo2;
  else if (servo == 3) lidServo = &lidServo3;
  else return;

  for (int angle = LID_OPEN_ANGLE_rev; angle >= LID_CLOSE_ANGLE_rev; angle -= ANGLE_STEP) 
  {
    lidServo->write(angle);
    delay(STEP_DELAY);
  }
  lidServo->write(LID_CLOSE_ANGLE_rev);
}
float measureFreeSpace(int trigPin, int echoPin) 
{
    long duration;
    float distance_cm, freeSpacePercent;
    
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH, 30000);
    
    if (duration == 0) {
        return 0; // For Debugging purposes. Return 0 if no echo is received.
    }
    distance_cm = (duration * 0.0343) / 2;
    if (distance_cm < MIN_DISTANCE) {
        return 100; // For Debugging purposes. Return 100% if object is too close
    } else if (distance_cm > MAX_DISTANCE) {
        return 0; // For Debugging purposes. Return 0% if object is out of range
    }
    freeSpacePercent = map(distance_cm, MIN_DISTANCE, MAX_DISTANCE, 100, 0);
    return constrain(freeSpacePercent, 0, 100);
}

void buzz(int buzzer) 
{
  int buzzerPin;
  if (buzzer == 1) buzzerPin = buzzerPin1;
  else if (buzzer == 2) buzzerPin = buzzerPin2;
  else if (buzzer == 3) buzzerPin = buzzerPin3;
  else return;
  digitalWrite(buzzerPin, HIGH);
}

void stopBuzz(int buzzer) 
{
  int buzzerPin;
  if (buzzer == 1) buzzerPin = buzzerPin1;
  else if (buzzer == 2) buzzerPin = buzzerPin2;
  else if (buzzer == 3) buzzerPin = buzzerPin3;
  else return;
  digitalWrite(buzzerPin, LOW);
}
// --------------------------------------------------------------------------------- //