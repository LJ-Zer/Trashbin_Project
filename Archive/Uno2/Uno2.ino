#define TRIG_PIN 16
#define ECHO_PIN 17

#define MIN_DISTANCE 2   // Closest distance (100%)
#define MAX_DISTANCE 200 // Farthest distance (0%)

void setup() {
    Serial.begin(115200);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

void loop() {
    float percentage = measureFreeSpace(TRIG_PIN, ECHO_PIN);

    Serial.print("Distance Percentage: ");
    Serial.print(percentage);
    Serial.println("%");

    delay(500);
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
        return 0; // Return 0 if no echo is received
    }

    distance_cm = (duration * 0.0343) / 2;

    if (distance_cm < MIN_DISTANCE) {
        return 100; // Return 100% if object is too close
    } else if (distance_cm > MAX_DISTANCE) {
        return 0; // Return 0% if object is out of range
    }

    freeSpacePercent = map(distance_cm, MIN_DISTANCE, MAX_DISTANCE, 100, 0);
    return constrain(freeSpacePercent, 0, 100);
}
