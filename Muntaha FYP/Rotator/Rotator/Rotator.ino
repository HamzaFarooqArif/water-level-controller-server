#include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>

// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 32 // OLED display height, in pixels

int rotateLeftButtonPin = 7;
int rotateRightButtonPin = 6;
int haltButtonPin = 8;
int leftLimitSwitchPin = A1;
int rightLimitSwitchPin = A2;
int stepsCounterPin = A0;
int motorRotateLeftPin = 3;
int motorRotateRightPin = 5;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
int rotateLeftButtonVal = 0;
int rotateRightButtonVal = 0;
int haltButtonVal = 0;
int leftLimitSwitchVal = 0;
int rightLimitSwitchVal = 0;
int stepsCounterVal = 0;

int rotateLeftSignal = 0;
int rotateRightSignal = 0;

int motorDrivingDirection = 0; //0: no-rotation, 1: left, 2: right
bool halted = false;
bool counterPrevVal = false;
int stepsCounted = 0;

// void initializeDisplay() {
//   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
//     Serial.println(F("SSD1306 allocation failed"));
//     for(;;);
//   }
//   display.setTextSize(1);
// }

void initializePins() {
  pinMode(rotateLeftButtonPin, INPUT);
  pinMode(rotateRightButtonPin, INPUT);
  pinMode(haltButtonPin, INPUT);
  pinMode(motorRotateLeftPin, INPUT);
  pinMode(motorRotateRightPin, INPUT);
}

void readValues() {
    rotateLeftButtonVal = digitalRead(rotateLeftButtonPin);
    rotateRightButtonVal = digitalRead(rotateRightButtonPin);
    haltButtonVal = digitalRead(haltButtonPin);
    leftLimitSwitchVal = digitalRead(leftLimitSwitchPin);
    rightLimitSwitchVal = digitalRead(rightLimitSwitchPin);
    stepsCounterVal = digitalRead(stepsCounterPin);
    
    if(haltButtonVal) halted = true;
    
    if(leftLimitSwitchVal) stepsCounted = 0;
    if(counterPrevVal != stepsCounterVal) {
      if(motorDrivingDirection == 2) {
        stepsCounted++;
      } else if(motorDrivingDirection == 1) {
        stepsCounted--;
      }
      counterPrevVal = stepsCounterVal;
    }
}

void writeToMotor() {
  if((rotateLeftButtonVal || rotateLeftSignal) && !leftLimitSwitchVal) {
    digitalWrite(motorRotateLeftPin, HIGH);
    digitalWrite(motorRotateRightPin, LOW);
    motorDrivingDirection = 1;
  }
  else if((rotateRightButtonVal || rotateRightSignal) && !rightLimitSwitchVal) {
    digitalWrite(motorRotateLeftPin, LOW);
    digitalWrite(motorRotateRightPin, HIGH);
    motorDrivingDirection = 2;
  }
  else {
    digitalWrite(motorRotateLeftPin, LOW);
    digitalWrite(motorRotateRightPin, LOW);
    motorDrivingDirection = 0;
  }
}

void haltAll() {
  digitalWrite(motorRotateLeftPin, LOW);
  digitalWrite(motorRotateRightPin, LOW);
  motorDrivingDirection = 0;
}

// void displayIntroText() {
//   display.clearDisplay();
//   display.setTextColor(WHITE);
//   display.setCursor(0, 10);
//   display.println("Muntaha - 2020PID21");
//   display.display();
//   display.setCursor(0, 20);
//   display.println("Final Year Project");
//   display.display();
//   delay(2000);
//   display.clearDisplay();
//   display.display();
// }

void sendDataOnI2C() {
  const char* dataTemplate = "%d,%d,%d";
  char dataToSend[5]; // Buffer to hold the data
  sprintf(dataToSend, dataTemplate, leftLimitSwitchVal, rightLimitSwitchVal, motorDrivingDirection); // Format the data
  Wire.write(dataToSend); // Send the formatted data back to master
  Serial.println(dataToSend);
}

void receiveEventOnI2C(int bytes) {
  char receivedData[3]; // Buffer to hold the received data
  int i = 0;
  // Read the received data byte by byte
  while (Wire.available()) {
    receivedData[i] = Wire.read();
    i++;
  }
  receivedData[i] = '\0'; // Null-terminate the string
  sscanf(receivedData, "%d,%d", &rotateLeftSignal, &rotateRightSignal);
}

void setup() {
  Wire.begin(9);  // 8 for upper tracker and 9 for lower
  Wire.onReceive(receiveEventOnI2C); // Register event
  Wire.onRequest(sendDataOnI2C); // Register callback function for data requests
  Serial.begin(9600);
  // initializeDisplay();
  // displayIntroText();
}

void loop() {
  if(halted) {
    haltAll();
    Serial.println("System Halted!");
    delay(500);
  }
  else {
    readValues();
    writeToMotor();
    
    // Serial.print("rotateLeftButtonVal:  ");
    // Serial.println(rotateLeftButtonVal);
    // Serial.print("rotateRightButtonVal:  ");
    // Serial.println(rotateRightButtonVal);
    // Serial.print("haltButtonVal:  ");
    // Serial.println(haltButtonVal);
    // Serial.print("leftLimitSwitchVal:  ");
    // Serial.println(leftLimitSwitchVal);
    // Serial.print("rightLimitSwitchVal:  ");
    // Serial.println(rightLimitSwitchVal);
    // Serial.print("stepsCounterVal:  ");
    // Serial.println(stepsCounterVal);
    
    delay(100);
  }
}