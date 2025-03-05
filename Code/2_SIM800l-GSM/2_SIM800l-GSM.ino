// Note: Connect niyo yung pin ng ESP32 sa IO3 sa RX ng SIM800l module and IO1 ng ESP32 sa TX ng SIM800l module. 
//       Yung RST ng SIM800l module ay connected sa EN ng ESP32.
//       Connect niyo yung 5V and GND sa source na may 3.6 – 5.20 Volts and 2 Amperes.
//       Connect niyo yung VDD sa 5 Volts ng ESP32.
// Source: https://www.faranux.com/product/sim800l-v2-0-5v-wirelessgsm-gprs-module-quad-band/

// Sa baba may note ako doon na papalitan niyo yung cp number. 
// Make sure na nagana din yung sim card and may load yung ilalagay niyo sa module.

void setup() 
{
  Serial.begin(115200);
  Serial2.begin(9600);
  delay(3000);
  test_sim800_module();
  send_SMS();
}
void loop() 
{
  updateSerial();
}

void test_sim800_module()
{
  Serial2.println("AT");
  updateSerial();
  Serial.println();
  Serial2.println("AT+CSQ");
  updateSerial();
  Serial2.println("AT+CCID");
  updateSerial();
  Serial2.println("AT+CREG?");
  updateSerial();
  Serial2.println("ATI");
  updateSerial();
  Serial2.println("AT+CBC");
  updateSerial();
}
void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    Serial2.write(Serial.read());
  }
  while (Serial2.available())
  {
    Serial.write(Serial2.read());
  }
}

void send_SMS()
{
  Serial2.println("AT+CMGF=1"); 
  updateSerial();
// ----------- Change niyo yung +639062323004 ----------- //
  Serial2.println("AT+CMGS=\"+639062323004\"");
  updateSerial();
  Serial2.print("Hi, message test!"); 
  updateSerial();
  Serial.println();
  Serial.println("Message Sent! Check your phone.");
  Serial2.write(26);
}