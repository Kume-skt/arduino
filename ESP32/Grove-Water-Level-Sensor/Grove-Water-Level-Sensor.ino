#include <Wire.h>


#define ATTINY1_HIGH_ADDR   0x78
#define ATTINY2_LOW_ADDR   0x77

int value = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
}

void loop() {
  value = 0;
  getHigh12SectionValue();
  getLow8SectionValue();
  Serial.print("value : ");
  Serial.println(map(value, 0, 4500, 0, 100));
}

void getHigh12SectionValue(void)
{
  Wire.requestFrom(ATTINY1_HIGH_ADDR, 12);
  while (12 != Wire.available());
  for (int i = 0; i < 12; i++) {
    value += Wire.read();
  }
  delay(1000);
}
void getLow8SectionValue(void)
{
  Wire.requestFrom(ATTINY2_LOW_ADDR, 8);
  while (8 != Wire.available());
  for (int i = 0; i < 8 ; i++) {
    value += Wire.read(); // receive a byte as character
  }
  delay(1000);
}
