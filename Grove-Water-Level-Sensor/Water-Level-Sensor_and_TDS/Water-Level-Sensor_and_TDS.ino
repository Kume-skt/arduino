#include <Wire.h>

//水位センサー
#define ATTINY1_HIGH_ADDR   0x78
#define ATTINY2_LOW_ADDR   0x77

int value = 0;

//水質センサ
#define sensorPin A0

int sensorValue = 0;
float tdsValue = 0;
float Voltage = 0;


void setup() {
  Serial.begin(115200);
  Wire.begin();
}

void loop() {
  value = 0;
  getHigh12SectionValue();
  getLow8SectionValue();
  Serial.print(mymap(value, 0, 5000, 0, 10));
  Serial.println("cm");
  getTDS_Data();
  
  delay(1000);
}
float mymap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void getHigh12SectionValue(void)
{
  Wire.requestFrom(ATTINY1_HIGH_ADDR, 12);
  while (12 != Wire.available());
  for (int i = 0; i < 12; i++) {
    value += Wire.read();
  }
}
void getLow8SectionValue(void)
{
  Wire.requestFrom(ATTINY2_LOW_ADDR, 8);
  while (8 != Wire.available());
  for (int i = 0; i < 8 ; i++) {
    value += Wire.read(); // receive a byte as character
  }
}

void getTDS_Data() {
  sensorValue = analogRead(sensorPin);
  Voltage = sensorValue * 5 / 1024.0; //Convert analog reading to Voltage
  tdsValue = (133.42 * Voltage * Voltage * Voltage - 255.86 * Voltage * Voltage + 857.39 * Voltage) * 0.5; //Convert voltage value to TDS value
  Serial.print("TDS Value = ");
  Serial.print(tdsValue);
  Serial.println(" ppm");
 }
