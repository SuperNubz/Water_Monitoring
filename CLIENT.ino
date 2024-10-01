#include <WiFi.h>
#include <ESPAsyncWebSrv.h>

const int trigPin = 5;
const int echoPin = 18;
#define SOUND_SPEED 0.034
long duration;
float distanceCm;

const char* ssid = "ForOctaPi";
const char* password = "^^0nlyR4allow3d()";

AsyncWebServer server(80);

const int sensor1Pin = 34;  // Sensor 1 pin
const int sensor2Pin = 35;  // Sensor 2 pin
const int sensor3Pin = 32;  // Sensor 3 pin
const int sensor4Pin = 33;  // Sensor 4 pin

int waterLevelSensor1 = 0;
int waterLevelSensor2 = 0;
int waterLevelSensor3 = 0;
int waterLevelSensor4 = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
  pinMode(sensor3Pin, INPUT);
  pinMode(sensor4Pin, INPUT);

//  WiFi.begin(ssid, password);
//  while(WiFi.status() != WL_CONNECTED) {
//    delay(1000);
//    Serial.println("Connecting to WiFi...");
//  }
//  Serial.println("Connected to WiFi");
//  Serial.println(WiFi.localIP());
//
//  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send_P(200, "text/html", index_html);
//  });
//
//  server.on("/distance", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send(200, "text/plain", readDistance().c_str());
//  });
//
//  server.on("/sensor1", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send(200, "text/plain", readSensor(sensor1Pin, 500, "Water Detected").c_str());
//  });
//
//  server.on("/sensor2", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send(200, "text/plain", readSensor(sensor2Pin, 500, "Water Detected").c_str());
//  });
//
//  server.on("/sensor3", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send(200, "text/plain", readSensor(sensor3Pin, 500, "Water Detected").c_str());
//  });
//
//  server.on("/sensor4", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send(200, "text/plain", readSensor(sensor4Pin, 500, "Water Detected").c_str());
//  });

  server.begin();
}

void loop() {
  waterLevelSensor1 = readSensor(sensor1Pin);
  waterLevelSensor2 = readSensor(sensor2Pin);
  waterLevelSensor3 = readSensor(sensor3Pin);
  waterLevelSensor4 = readSensor(sensor4Pin);

  // Printing to serial monitor
  Serial.print("WL Sens 1:  "); Serial.println(waterLevelSensor1);
  Serial.print("WL Sens 2:  "); Serial.println(waterLevelSensor2);
  Serial.print("WL Sens 3:  "); Serial.println(waterLevelSensor3);
  Serial.print("WL Sens 4:  "); Serial.println(waterLevelSensor4);

  sendGPRS(waterLevelSensor1, waterLevelSensor2, waterLevelSensor3, waterLevelSensor4);
}

void sendGPRS(int WL_sensor_1, int WL_sensor_2, int WL_sensor_3, int WL_sensor_4){
  Serial2.println("AT");
//  delay(500);
  ShowSerialData();
  
  Serial2.println("AT+CPIN?");        // Manage PIN
  delay(50);
  ShowSerialData();
  
  Serial2.println("AT+CREG?");        // Provides information abt the registration status of the device on the network
  delay(50);
  ShowSerialData();
  
  Serial2.println("AT+CGATT?");       // Set status of the GPRS
  delay(50);
  ShowSerialData();
  
  Serial2.println("AT+CIPSHUT");      // Shut down or close GPRS data connection or sesssion
  delay(50);
  ShowSerialData();
  
  Serial2.println("AT+CIPSTATUS");    // Quesry the status of the TCP/IP and the GPRS data connection
  delay(50);
  ShowSerialData();
  
  Serial2.println("AT+CIPMUX=0");     // Configure module's behaviour to manage only 1 connection
  delay(50);
  ShowSerialData();
 
  Serial2.println("AT+CSTT=\"airtelgprs.com\"");    // Configure network's APN
  delay(100);
  ShowSerialData();
  
  Serial2.println("AT+CIICR");        // Activate PDP and establish GPRS with cellular network
  delay(100);
  ShowSerialData();
  
  Serial2.println("AT+CIFSR");        // Query and obtain the local IP address assigned to the module by the cellular network when connected to a GPRS data network
  delay(50);
  ShowSerialData();
  
  Serial2.println("AT+CIPSPRT=0");    // Configure the printing of debug information
  delay(50);
  ShowSerialData();
  
  Serial2.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");   // Initiate a network connection to a remote server or host
  delay(200);
  ShowSerialData();
  
  Serial2.println("AT+CIPSEND");      // Prepare module for sending data over an established network connection.
  delay(100);
  ShowSerialData();

  String str = "GET https://api.thingspeak.com/update?api_key=HQMO1OCTL8YAFDCC&field1=" + String(WL_sensor_1) +"&field2="+String(WL_sensor_2) + "&field3=" + String(WL_sensor_3) + "&field3=" + String(WL_sensor_4);
  Serial.println(str);
  Serial2.println(str);
  delay(50);
  ShowSerialData();

  Serial2.println((char)26);
  delay(50);
  Serial2.println();
}

String readDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distanceCm = duration * SOUND_SPEED / 2;
  Serial.print("Distance: ");
  Serial.println(distanceCm);
  return String(distanceCm);
}

int readSensor(int sensorPin) {
  int sensorValue = analogRead(sensorPin);
  return sensorValue;
}

void ShowSerialData(){
  while(Serial2.available()!= 0)
    Serial.write(Serial2.read());
  delay(1000);
}
