
SoftwareSerial mySerial(10, 11);  // RX = 10, TX = 11

float receivedMaterial = 0.0;

void setup() {
    Serial.begin(115200);
    mySerial.begin(115200);  // Match ESP32 baud rate
}

void loop() {
    if (mySerial.available() >= sizeof(receivedMaterial)) {  
        mySerial.readBytes((char*)&receivedMaterial, sizeof(receivedMaterial)); // Read float as bytes

        String materialType;
        if (receivedMaterial <= -12000) {
          materialType = "Metal";
        }
        else if (receivedMaterial >= -1500 && receivedMaterial <= -1000){
          materialType = "Plastic"; 
        }
        else if (receivedMaterial >= -4800 && receivedMaterial <= -4000) {
          materialType = "Paper";
        }
        else {
            materialType = "Unknown"; 
        }
        Serial.println();
        Serial.print("Received Material Type: ");
        Serial.print(materialType);
        Serial.println();
        Serial.print("Material Value: ");
        Serial.print(receivedMaterial, 2); // Print float correctly
        delay(1000);
    }
}

// @ 0 = 144756944.0, 139515552.0

// // @ 1 (Metal = negative large number)
// // @ 2 (Plasitc = )
// 472410208.0
// 5849425.0
// 5329229.0

// // @ 3(Paper = 3 digits number negative )
// -14.5
// 6384019.0
// 147902672.0
// 144756960.0
// -18.7
// -70.8

// //////////
// 0 None = -0.00 
// 1 Metal = -132898.25
// 2 Plastic = -1159.13
// 3 Paper = -4638.57
