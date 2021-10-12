//#include <M5Stack.h>

//int half_Height, half_Width = 0;
void setup() {
  //  Serial1.begin(115200);
  Serial.begin(38400);
//  M5.begin();
//  half_Height = M5.Lcd.height() / 3;
//  half_Width = M5.Lcd.width() / 5;
//  M5.Lcd.setTextSize(5);
  //  M5.Lcd.setTextDatum(4);
  //    M5.Lcd.printf("hje");

}

void loop() {

  if ( Serial.available() > 0 ) {
    String str = Serial.readStringUntil('\n');
//    Serial.print("talk:");
    Serial.println(str);
//    M5.Lcd.drawString(str, half_Width, half_Height);
  }
  delay(1000);
}
