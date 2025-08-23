//#include "func.hpp"
#include <Arduino.h>
#include "bluetooth.hpp"




void setup() {
  Serial.begin(115200);

  Bluetooth::init();

  Serial.println("BLE device is ready and advertising.");
}

void loop() {
  // Nothing needed here for now
}




/*
void setup() {
  Serial.begin(115200);  // USB CDC
  delay(2000);            // wait for host to be ready
  Serial.println("Serial is working!");
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  int result = Func::myFunction(2, 3);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  Serial.println("Tick" + String(result));
}
*/