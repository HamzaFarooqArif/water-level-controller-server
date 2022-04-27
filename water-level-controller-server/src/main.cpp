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

// #include <NewPing.h>

// #define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
// #define ECHO_PIN     13  // Arduino pin tied to echo pin on the ultrasonic sensor.
// #define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

// void setup() {
//   Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
// }

// void loop() {
//   delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
//   Serial.print("Ping: ");
//   Serial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
//   Serial.println("cm");
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
// unsigned char flowsensor = 13; // Sensor Input
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