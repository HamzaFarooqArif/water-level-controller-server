#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
float ds18b20temperature;

Adafruit_BMP280   bmp;
float bmp280_temperature;
float bmp280_pressure;
float bmp280_altitude;


bool shouldReboot = false;
JSONVar responseToSend;

const char* ssid = "TP-Link_0B42";
const char* password = "23368156";
AsyncWebServer server(80);
int wifi_disconnected_count = 0;
int wifi_disconnected_timeout = 20;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int tl136_sensorPin = 34;
int tl136_sensorValue = 0;

void readValue() {
  tl136_sensorValue = analogRead(tl136_sensorPin);
  sensors.requestTemperatures();
  ds18b20temperature = sensors.getTempCByIndex(0);
  bmp280_temperature = bmp.readTemperature(); 
  bmp280_pressure = bmp.readPressure();
  bmp280_altitude = bmp.readAltitude(1019.66)*3.28084;

  responseToSend["TL-136_Val"] = tl136_sensorValue;
  responseToSend["TL-136_temp_C"] = ds18b20temperature;
  responseToSend["BMP280_pressure_Pa"] = bmp280_pressure;
  responseToSend["BMP280_altitude_ft"] = bmp280_altitude;
  responseToSend["BMP280_temperature_C"] = bmp280_temperature;
}

String getData() {
  return JSON.stringify(responseToSend);
}

String reboot() {
  shouldReboot = true;
  return "Reboot request submitted successfully";
}

void checkToReboot(){
  if(shouldReboot || wifi_disconnected_count > wifi_disconnected_timeout){
    Serial.println("Rebooting...");
    delay(1000);
    esp_restart();
  }
}

void drawOled() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(INVERSE);
  display.setCursor(0, 3);
  display.print("TL-136_Val: ");
  display.print(tl136_sensorValue);
  display.setCursor(0, 12);
  display.print("TL-136_temp_C: ");
  display.print(ds18b20temperature);
  display.setCursor(0, 21);
  display.print("Air_P: ");
  display.print(bmp280_pressure);
  display.print("Pa");
  display.setCursor(0, 30);
  display.print("Air_Alt: ");
  display.print(bmp280_altitude);
  display.print("ft");
  display.setCursor(0, 39);
  display.print("Air_Temp: ");
  display.print(bmp280_temperature);
  display.print(" C");
  display.setCursor(0, 48);
  if(WiFi.status() != WL_CONNECTED) {
    display.print("WiFi Disconnected");
  } else {
    display.print("IP: ");
    display.print(WiFi.localIP());
  }
  display.display();
}

void printToSerial() {
  Serial.print("TL-136: ");
  Serial.println(tl136_sensorValue);
  Serial.print("bmp280_Pressure: ");
  Serial.print(bmp280_pressure);
  Serial.println(" Pa");
  Serial.print("bmp280_Altitude: ");
  Serial.print(bmp280_altitude);
  Serial.println(" ft");
  Serial.print("bmp280_Temperature: ");
  Serial.print(bmp280_temperature);
  Serial.println(" C");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);

  sensors.begin();  // Start the DS18B20 sensor

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  server.on("/getData", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(2, HIGH);
    request->send_P(200, "text/plain", getData().c_str());
    digitalWrite(2, LOW);
  });
  server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", reboot().c_str());
  });
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.begin();
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  bmp.begin(0x76);
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /*   Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp.   oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure   oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering.   */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() {
  readValue();
  drawOled();
  printToSerial();

  if(WiFi.status() != WL_CONNECTED || WiFi.localIP().toString() == "0.0.0.0"){
    wifi_disconnected_count += 1;
  }
  
  checkToReboot();
  delay(500);
}
