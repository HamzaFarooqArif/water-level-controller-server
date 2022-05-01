// #include "WiFi.h"
// #include "ESPAsyncWebServer.h"
// #include "Arduino_JSON.h"

// // Set your network credentials
// const char* ssid = "TP-Link_0B42";
// const char* password =  "23368156";

// // Create AsyncWebServer object on port 80
// AsyncWebServer server(80);

// JSONVar responseToSend;

// bool shouldReboot = false;
// int rebootTimeMillis = 2000;

// void checkToReboot(){
//   if(shouldReboot){
//     if(rebootTimeMillis < 1){
//       Serial.println("Rebooting...");
//       delay(1000);
//       esp_restart();
//     }
//     rebootTimeMillis -= 1;
//     delay(1);
//   }
// }

// void setupJson(){
//   responseToSend["hello"] = "world";
//   responseToSend["true"] = true;
//   responseToSend["x"] = 42;
// }

// String getData() {
//   return JSON.stringify(responseToSend);
//   //return String(1.8 * bme.readTemperature() + 32);
// }

// String reboot() {
//   shouldReboot = true;
//   return "Reboot request submitted successfully";
// }



// void setup(){
//   // Serial port for debugging purposes
//   Serial.begin(115200);

//   Serial.print("Connecting to ");
//   Serial.println(ssid);
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("");
//   Serial.println("WiFi connected.");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
//   server.begin();

//   server.on("/getData", HTTP_GET, [](AsyncWebServerRequest *request){
//     request->send_P(200, "text/plain", getData().c_str());
//   });
//   server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request){
//     request->send_P(200, "text/plain", reboot().c_str());
//   });

//   server.begin();

//   setupJson();
// }
 
// void loop(){
//   checkToReboot();
  
// }

// ---------------------------------------------------------------------------

// #include "Arduino.h"

// /* Arduino example sketch to control a JSN-SR04T ultrasonic distance sensor with Arduino. No library needed. More info: https://www.makerguides.com */

// // Define Trig and Echo pin:
// #define trigPin 12
// #define echoPin 13

// // Define variables:
// long duration;
// int distance;

// void setup() {
//   // Define inputs and outputs
//   pinMode(trigPin, OUTPUT);
//   pinMode(echoPin, INPUT);
  
//   // Begin Serial communication at a baudrate of 9600:
//   Serial.begin(115200);
// }

// void loop() {
//   // Clear the trigPin by setting it LOW:
//   digitalWrite(trigPin, LOW);
  
//   delayMicroseconds(5);

//  // Trigger the sensor by setting the trigPin high for 10 microseconds:
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPin, LOW);
  
//   // Read the echoPin. pulseIn() returns the duration (length of the pulse) in microseconds:
//   duration = pulseIn(echoPin, HIGH);
  
//   // Calculate the distance:
//   distance = duration*0.034/2;
  
//   // Print the distance on the Serial Monitor (Ctrl+Shift+M):
//   Serial.print("Distance = ");
//   Serial.print(distance);
//   Serial.println(" cm");
  
//   delay(100);
// }

// ---------------------------------------------------------------------------

// #include "Arduino.h"
// /*
// YF‐ S201 Water Flow Sensor
// Water Flow Sensor output processed to read in litres/hour
// Adaptation Courtesy: hobbytronics.co.uk
// */
// volatile int flow_frequency; // Measures flow sensor pulses
// // Calculated litres/hour
// float vol = 0.0, l_minute;
// unsigned char flowsensor = 32; // Sensor Input
// unsigned long currentTime;
// unsigned long cloopTime;
// String l_minute_str = "";

// void flow () // Interrupt function
// {
//    flow_frequency++;
// }

// void setup()
// {
//    pinMode(flowsensor, INPUT);
//    digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
//    Serial.begin(115200);
//    attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING); // Setup Interrupt
//    currentTime = millis();
//    cloopTime = currentTime;
// }
// void loop ()
// {
//    currentTime = millis();
//    // Every second, calculate and print litres/hour
//    if(currentTime >= (cloopTime + 1000))
//    {
//     cloopTime = currentTime; // Updates cloopTime
//     if(flow_frequency != 0){
//       // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
//       l_minute = (flow_frequency / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
//       // l_minute = l_minute/60;
//       l_minute_str = "";
//       l_minute_str.concat(l_minute);
//       l_minute_str.substring(0, 3);
//       vol = vol + l_minute;
//       flow_frequency = 0; // Reset Counter
//       Serial.print(l_minute_str); // Print litres/hour
//       Serial.println(" L/min");
//     }
//     else {
//       Serial.println(" flow rate = 0 ");
//     }
//    }
// }

// ---------------------------------------------------------------------------

// #include "Arduino.h"

// int soilMoistureValue = 0;
// void setup() {
//   Serial.begin(115200); // open serial port, set the baud rate to 9600 bps
// }
// void loop() {
// soilMoistureValue = analogRead(34);  //put Sensor insert into soil
// Serial.println(soilMoistureValue);
//   delay(250);
// }

// ---------------------------------------------------------------------------

// #include "Arduino.h"
// /*********
//   Rui Santos
//   Complete project details at https://RandomNerdTutorials.com  
// *********/

// #include <OneWire.h>
// #include <DallasTemperature.h>

// // GPIO where the DS18B20 is connected to
// const int oneWireBus = 4;     

// // Setup a oneWire instance to communicate with any OneWire devices
// OneWire oneWire(oneWireBus);

// // Pass our oneWire reference to Dallas Temperature sensor 
// DallasTemperature sensors(&oneWire);

// void setup() {
//   // Start the Serial Monitor
//   Serial.begin(115200);
//   // Start the DS18B20 sensor
//   sensors.begin();
// }

// void loop() {
//   sensors.requestTemperatures(); 
//   float temperatureC = sensors.getTempCByIndex(0);
//   float temperatureF = sensors.getTempFByIndex(0);
//   Serial.print(temperatureC);
//   Serial.println("ºC");
//   Serial.print(temperatureF);
//   Serial.println("ºF");
//   delay(100);
// }

// ---------------------------------------------------------------------------

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>

// Network credentials
const char* ssid = "TP-Link_0B42";
const char* password =  "23368156";

// JSN-SR04T ultrasonic sensor configuration
#define trigPin 12
#define echoPin 13
long duration;
int distance;

// DS18B20 temperature sensor configuration
const int oneWireBus = 33;
OneWire oneWire(oneWireBus);
DallasTemperature TemperatureSensor(&oneWire);

// YF-S201C flow sensor configuration
unsigned char flowsensor = 32; // Sensor Input
volatile int flow_frequency; // Measures flow sensor pulses
float l_minute;
unsigned long currentTime;
unsigned long cloopTime;
String l_minute_str = "";
void flow () // Interrupt function
{
   flow_frequency++;
}

// Soil moisture sensor configuration
int pipeMoistureSensorPin = 34;
int fullTankMoistureSensorPin = 35;

AsyncWebServer server(80);
JSONVar responseToSend;

bool shouldReboot = false;


int measureWaterLevelCm(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
 // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Read the echoPin. pulseIn() returns the duration (length of the pulse) in microseconds:
  duration = pulseIn(echoPin, HIGH);
  // Calculate the distance:
  distance = duration*0.034/2;
  return distance;
}

float measureWaterTemperature(bool inCelsius){
  TemperatureSensor.requestTemperatures(); 
  float temperature = inCelsius ? TemperatureSensor.getTempCByIndex(0) : TemperatureSensor.getTempFByIndex(0);
  return temperature;
}

float measureWaterFlow(){
    cloopTime = millis(); // Updates cloopTime
    while(true){
      currentTime = millis();
      // Every second, calculate and print litres/hour
      if(currentTime >= (cloopTime + 100))
      {
        if(flow_frequency != 0){
          // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
          l_minute = flow_frequency / 7.5; // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
          // l_minute = l_minute/60;
          flow_frequency = 0; // Reset Counter
          return l_minute;
        }
        else{
          return 0;
        }
      }
    }
  return -1;
}

int measureWaterPresence(int sensorPin){
  return analogRead(sensorPin);
}

String getData() {
  return JSON.stringify(responseToSend);
  //return String(1.8 * bme.readTemperature() + 32);
}

void checkToReboot(){
  if(shouldReboot){
    Serial.println("Rebooting...");
    delay(1000);
    esp_restart();
  }
}

String reboot() {
  shouldReboot = true;
  return "Reboot request submitted successfully";
}

void inquireSensorData(){
  responseToSend["level"] = measureWaterLevelCm();
  responseToSend["temperature"] = measureWaterTemperature(true);
  responseToSend["waterIncoming"] = measureWaterPresence(pipeMoistureSensorPin);
  responseToSend["tankFull"] = measureWaterPresence(fullTankMoistureSensorPin);
  responseToSend["flowRate"] = measureWaterFlow();
}


void setup(){
  // JSN-SR04T ultrasonic sensor setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // DS18B20 Temperature sensor setup
  TemperatureSensor.begin();

  pinMode(flowsensor, INPUT);
  digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
  attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING); // Setup Interrupt
  currentTime = millis();
  cloopTime = currentTime;

  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  inquireSensorData();
  server.on("/getData", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getData().c_str());
  });
  server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", reboot().c_str());
  });

}

void loop(){
  checkToReboot();
  inquireSensorData();

  Serial.println(responseToSend);

  delay(1000);

}