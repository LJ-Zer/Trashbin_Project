#define SIM800_TX 17  // Connect to SIM800L TX
#define SIM800_RX 16  // Connect to SIM800L RX

void setup() {
  Serial.begin(115200);  // Monitor output
  Serial2.begin(9600, SERIAL_8N1, SIM800_TX, SIM800_RX);  // SIM800L communication

  delay(3000);  
  Serial.println("Initializing SIM800L...");

  test_SIM800L();
  send_SMS();
}

void loop() {
  updateSerial()
}

void test_SIM800L() {
  Serial2.println("AT");        // Check if SIM800L is responding
  updateSerial();
  Serial2.println("AT+CSQ");    // Check signal strength
  updateSerial();
  Serial2.println("AT+CCID");   // Check if SIM card is detected
  updateSerial();
  Serial2.println("AT+CREG?");  // Check network registration
  updateSerial();
}

void send_SMS() {
  Serial.println("Sending SMS...");
  Serial2.println("AT+CMGF=1"); // Set SMS to text mode
  delay(1000);
  
  Serial2.println("AT+CMGS=\"+639052323008\""); // Replace with your phone number
  delay(1000);
  
  Serial2.print("Hello from ESP32 + SIM800L!"); // Your message
  delay(1000);
  
  Serial2.write(26); // Send SMS (CTRL+Z)
  delay(5000);
  
  Serial.println("SMS Sent!");
}

void updateSerial() {
  while (Serial2.available()) {
    Serial.write(Serial2.read()); // Print SIM800L response
  }
}
