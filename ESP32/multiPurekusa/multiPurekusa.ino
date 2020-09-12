#include "Adafruit_seesaw.h"

Adafruit_seesaw ss;

// Pin Definitions
const int selectPins[1] = {0}; // S0~2, S1~3, S2~4
const int zInput = A0; // Connect common (Z) to A0 (analog input)

void setup() {
  Serial.begin(115200); // Initialize the serial port
  // Set up the select pins as outputs:
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  pinMode(zInput, INPUT);
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
    while (1);
  } else {
    Serial.print("seesaw started! version: ");
    Serial.println(ss.getVersion(), HEX);
  }
}

void loop()
{
  // Loop through all eight pins.

  //    int inputValue = analogRead(A0);
  //    Serial.print(String(inputValue) + "\t");
  uint16_t capread = ss.touchRead(0);
  Serial.println(capread);

}
