// #include <Arduino.h>
// #include <Wire.h>

// int upperModuleAddress = 8;
// int lowerModuleAddress = 9;

// void setup() {
//   Wire.begin(); // Initialize I2C communication as Master
//   Serial.begin(9600); // Initialize Serial communication
// }

// void loop() {
//   // Example values to send
//   int var1 = 1;
//   int var2 = 0;
  
//   // Format the variables into a string
//   char dataToSend[3];
//   sprintf(dataToSend, "%d,%d", var1, var2);
  
//   // Send the formatted string to the first slave Arduino (address 8)
//   Wire.beginTransmission(upperModuleAddress);
//   Wire.write(dataToSend);
//   Wire.endTransmission();
//   Wire.requestFrom(8, 5); // Request 6 bytes of data from slave
//   while (Wire.available()) {
//     char c = Wire.read(); // Read the incoming byte
//     Serial.print(c);      // Print the character received
//   }
//   Serial.println();
   
//   delay(1000);
// }

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "MuntahaFYP"; // SSID for the ESP32 Access Point
const char* password = "123456789"; // Password for the ESP32 Access Point

// Static IP configuration
IPAddress staticIP(192, 168, 1, 1); // Static IP address
IPAddress gateway(192, 168, 1, 1); // Gateway IP address
IPAddress subnet(255, 255, 255, 0); // Subnet mask

int btn1Val, btn2Val, btn3Val, btn4Val, btn5Val;

const int output = 2;
const int pumpPin = 4;
const int valvePin = 5;

String sliderValue = "0";

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

const char* PARAM_INPUT = "value";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta http-equiv="Access-Control-Allow-Origin" content="*"/>
  <title>Muntaha FYP</title>
  <style>
    .btnPressed {
        background-color: blue;
    }
    .active {
        background-color: green;
        color: white;
    }
  </style>
</head>
<body>
  <h2>Muntaha FYP</h2>
  <div>
    <button id="btn1" onmousedown="buttonDown('btn1')" onmouseup="buttonUp('btn1')">btn1</button>
  </div>
  <div>
      <button id="btn2" onmousedown="buttonDown('btn2')" onmouseup="buttonUp('btn2')">btn2</button>
      <button id="btn3" onmousedown="buttonDown('btn3')" onmouseup="buttonUp('btn3')">btn3</button>
      <button id="btn4" onmousedown="buttonDown('btn4')" onmouseup="buttonUp('btn4')">btn4</button>
  </div>
  <div>
    Pump:
    <button id="btn5" onmousedown="buttonDown('btn5')" onmouseup="buttonUp('btn5')">OFF</button>
</div>
<script>
    document.addEventListener("keydown", keyDownTextField, false);
    document.addEventListener("keyup", keyUpTextField, false);

    var req = "";

    var btn1Val = 0;
    var btn2Val = 0;
    var btn3Val = 0;
    var btn4Val = 0;
    var btn5Val = 0;
    
    function buttonDown(btnId) {
        if(btnId == "btn1") {
            var element = document.getElementById("btn1");
            element.classList.add("btnPressed");
            btn1Val = 1;
            setupData();
        }
        if(btnId == "btn2") {
            var element = document.getElementById("btn2");
            element.classList.add("btnPressed");
            btn2Val = 1;
            setupData();
        }
        if(btnId == "btn3") {
            var element = document.getElementById("btn3");
            element.classList.add("btnPressed");
            btn3Val = 1;
            setupData();
        }
        if(btnId == "btn4") {
            var element = document.getElementById("btn4");
            element.classList.add("btnPressed");
            btn4Val = 1;
            setupData();
        }
    }

    function buttonUp(btnId) {
        if(btnId == "btn1") {
            var element = document.getElementById("btn1");
            element.classList.remove("btnPressed");
            btn1Val = 0;
            setupData();
        }
        if(btnId == "btn2") {
            var element = document.getElementById("btn2");
            element.classList.remove("btnPressed");
            btn2Val = 0;
            setupData();
        }
        if(btnId == "btn3") {
            var element = document.getElementById("btn3");
            element.classList.remove("btnPressed");
            btn3Val = 0;
            setupData();
        }
        if(btnId == "btn4") {
            var element = document.getElementById("btn4");
            element.classList.remove("btnPressed");
            btn4Val = 0;
            setupData();
        }
    }

    function keyDownTextField(e) {
        var keyCode = e.keyCode;
        if(keyCode == 38) {
            var element = document.getElementById("btn1");
            element.classList.add("btnPressed");
            btn1Val = 1;
            setupData();
        }
        if(keyCode == 40) {
            var element = document.getElementById("btn2");
            element.classList.add("btnPressed");
            btn2Val = 1;
            setupData();
        }
        if(keyCode == 37) {
            var element = document.getElementById("btn3");
            element.classList.add("btnPressed");
            btn3Val = 1;
            setupData();
        }
        if(keyCode == 39) {
            var element = document.getElementById("btn4");
            element.classList.add("btnPressed");
            btn4Val = 1;
            setupData();
        }
    }

    function keyUpTextField(e) {
        var keyCode = e.keyCode;
        if(keyCode == 38) {
            var element = document.getElementById("btn1");
            element.classList.remove("btnPressed");
            btn1Val = 0;
            setupData();
        }
        if(keyCode == 40) {
            var element = document.getElementById("btn2");
            element.classList.remove("btnPressed");
            btn2Val = 0;
            setupData();
        }
        if(keyCode == 37) {
            var element = document.getElementById("btn3");
            element.classList.remove("btnPressed");
            btn3Val = 0;
            setupData();
        }
        if(keyCode == 39) {
            var element = document.getElementById("btn4");
            element.classList.remove("btnPressed");
            btn4Val = 0;
            setupData();
        }
    }

    const button = document.getElementById("btn5");
    button.addEventListener("click", () => {
        if (button.classList.contains("active")) {
            button.classList.remove("active");
            button.textContent = "OFF";
            btn5Val = 0;
        } else {
            button.classList.add("active");
            button.textContent = "ON";
            btn5Val = 1;
        }
        setupData();
    });

    function setupData() {
        req = `${btn1Val},${btn2Val},${btn3Val},${btn4Val},${btn5Val}`;
    }

    setInterval(() => {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/run?value="+req, true);
        xhr.send();
        console.log(req);
    }, 500);

    
</script>
</body>
</html>

)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if (var == "SLIDERVALUE"){
    return sliderValue;
  }
  return String();
}

void sendDataI2C() {
  char dataToSend1[5];
  snprintf(dataToSend1, sizeof(dataToSend1), "%d,%d", btn3Val, btn4Val);
  Wire.beginTransmission(8);
  Wire.write(dataToSend1);
  Wire.endTransmission();
  Wire.requestFrom(8, 5, true); // Request 6 bytes of data from slave
  Serial.print("module1 data: ");
    while (Wire.available()) {
      char c = Wire.read(); // Read the incoming byte
      Serial.print(c);      // Print the character received
    }
  Serial.println();
  
  char dataToSend2[5];
  snprintf(dataToSend2, sizeof(dataToSend2), "%d,%d", btn1Val, btn2Val);
  Wire.beginTransmission(9);
  Wire.write(dataToSend2);
  Wire.endTransmission();
  Wire.requestFrom(9, 5); // Request 6 bytes of data from slave
  Serial.print("module2 data: ");
  while (Wire.available()) {
    char c = Wire.read(); // Read the incoming byte
    Serial.print(c);      // Print the character received
  }
  Serial.println();

}

void handlePump() {
  if(btn5Val > 0) {
    digitalWrite(valvePin, HIGH);
    digitalWrite(pumpPin, HIGH);
  } else {
    digitalWrite(valvePin, LOW);
    digitalWrite(pumpPin, LOW);
  }
}

void setup(){
  Wire.begin(); // Initialize I2C communication as Master
  Serial.begin(9600);
  pinMode(pumpPin, OUTPUT);
  pinMode(valvePin, OUTPUT);
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(output, ledChannel);
  ledcWrite(ledChannel, sliderValue.toInt());

  WiFi.softAPConfig(staticIP, gateway, subnet);
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/run", HTTP_GET, [] (AsyncWebServerRequest *request) {
    ledcWrite(ledChannel, 255);
    String inputMessage;
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      char *ptr = strtok(const_cast<char*>(inputMessage.c_str()), ",");
      int counter = 1;
      while (ptr != NULL) {
        int value = atoi(ptr); // Convert string to integer
        switch (counter) {
          case 1:
            btn1Val = value;
            break;
          case 2:
            btn2Val = value;
            break;
          case 3:
            btn3Val = value;
            break;
          case 4:
            btn4Val = value;
            break;
          case 5:
            btn5Val = value;
            break;
          default:
            break;
        }
        ptr = strtok(NULL, ",");
        counter++;
      }
    }

    sendDataI2C();
    handlePump();

    Serial.print(btn1Val);
    Serial.print(",");
    Serial.print(btn2Val);
    Serial.print(",");
    Serial.print(btn3Val);
    Serial.print(",");
    Serial.print(btn4Val);
    Serial.print(",");
    Serial.println(btn5Val);

    request->send(200, "text/plain", "OK");
    ledcWrite(ledChannel, 0);
  });
  
  server.begin();
}
  
void loop() {

}
