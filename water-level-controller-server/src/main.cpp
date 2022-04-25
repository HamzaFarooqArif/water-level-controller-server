#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "Arduino_JSON.h"

// Set your network credentials
const char* ssid = "TP-Link_0B42";
const char* password =  "23368156";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

JSONVar responseToSend;

bool shouldReboot = false;
int rebootTimeMillis = 2000;

void checkToReboot(){
  if(shouldReboot){
    if(rebootTimeMillis < 1){
      Serial.println("Rebooting...");
      delay(1000);
      esp_restart();
    }
    rebootTimeMillis -= 1;
    delay(1);
  }
}

void setupJson(){
  responseToSend["hello"] = "world";
  responseToSend["true"] = true;
  responseToSend["x"] = 42;
}

String getData() {
  return JSON.stringify(responseToSend);
  //return String(1.8 * bme.readTemperature() + 32);
}

String reboot() {
  shouldReboot = true;
  return "Reboot request submitted successfully";
}



void setup(){
  // Serial port for debugging purposes
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

  server.on("/getData", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getData().c_str());
  });
  server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", reboot().c_str());
  });

  server.begin();

  setupJson();
}
 
void loop(){
  checkToReboot();
  
}