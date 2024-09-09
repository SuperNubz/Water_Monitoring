/*
 * Rui Santos 
 * Complete Project Details https://randomnerdtutorials.com
 */

/*     ARDUINO      GPS      
    HWSerial RX -> TX
    HWSerial TX -> RX
*/

/*      ARDUINO     GSM
            RX -> 9
            TX -> 10
*/


//#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <NewPing.h>

// Buzzer Connection
const int buzzer = 9    

// Ultrasonic Sensor Connection
const int trig = X;
const int echo = X;
const float distance = 200.0;   // Max distance of what the sensor should read (MAX VALUE: 500cm)
NewPing sonar(trig, echo, distance)

// Distance of the container when it is empty
const float originalDistance = X;

// Distance of each levels
const float firstLevel = 0.0;
const float secondLevel = 0.0;
const float thirdLevel = 0.0;

/* ---------- WIFI Name & Password ---------- */
const char* ssid = "XXXXXXXXXXX";
const char* password = "XXXXXXXXXXXXx";


void setup(){  
  // Serial Monitor
  Serial.begin(9600);

  Serial1.begin(9600);

  // GSM Module Initialization
  Serial2.begin(9600);

  // Buzzer Initialization
  pinMode(buzzer, 1000);

  Serial.println("Setting up.....");
  delay(500);

  Serial2.println("AT"); //Once the handshake test is successful, it will back to OK
  delay(50);
  
  Serial2.println("AT+CMGF=1"); // Configuring TEXT mode
  delay(50);

  Serial2.println("AT+CMGS=\"XXXXXXXX\"\r");      // Input Cellphone Number here.
  delay(50);

  WiFi.mode(WIFI_STA);
  if (initWifi()){
    internet_connected = true;
    Serial.println("Internet Connected");
  }
}

void loop(){

  float waterLevel = 0.0;

  waterLevel = measureDistance();
  waterLevel = originalDistance - waterLevel;

  if(waterLevel > firstLevel && waterLevel < secondLevel - 20.0){
    sendSms(waterLevel);
  } else if(waterLevel > secondLevel){
    sendSmms(waterLevel);
  } else if (waterLevel >
  
  

}
   
void sendSms(float waterLevel){
  Serial2.listen();
  Serial2.println("AT");
  Serial2.println("AT+CMGF=1\r");
  delay(50);

  Serial2.println("AT+CMGS=\"XXXXXXXXXX\"\r");    // Enter Number Here
  delay(50);

  Serial2.print("Water Level is at: ");
  Serial2.println(waterLevel);
  
  Serial2.write(0x1A);
}

float measureDistance(){
  float duration = sonar.ping();
  float distance_cm = sonar.convert_cm(duration);
  distance = distance_cm / 2.54;
  return distance;
}

void checkGSMStatus(){
  while(gsm_signalCheck != 1){
    delay(500);
    Serial2.println("AT");
    delay(100);
    Serial2.println("AT+CMGF=1\r");
    delay(100);
    Serial2.println("AT+CSQ");
    delay(500);

    String response = readSerial();

    if(response.indexOf("+CSQ:") != 1){
      int rssi = parseRSSI(response);

      Serial.print("RSSI: ");
      Serial.println(rssi);

      // Check if RSSI has signal
      if(rssi >= 8){
        Serial.println("GSM HAS SIGNAL");
        gsm_signalCheck = true;
      } else {
        Serial.println("NO GSM SIGNAL");
      }
    } else {
      Serial.println("Failed to get signal Strength");
    }
  }
}

String readSerial(){
  String responseGSM = "";
  while(Serial2.available()){
    char c = Serial2.read();
    responseGSM += c;
  }

  return responseGSM;
}

bool initWifi(){
  Serial.println("\r\nConnecting to: " + String(ssid));
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  return true;
}
