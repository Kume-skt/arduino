#include <M5Stack.h>
#include <WiFi.h>
#include "AudioFileSourceSD.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

int half_Height, half_Width = 0;
String mode_nfc = "IN";
void setup()
{
  M5.begin();
  WiFi.mode(WIFI_OFF);
  delay(500);
  half_Height = M5.Lcd.height() / 3;
  half_Width = (M5.Lcd.width() / 2) - 50;
  M5.Lcd.setTextFont(4);

  M5.Lcd.printf("NFC SYSTEM\n");
  Serial.printf("Sample MP3 playback begins...\n");
}

void loop()
{

  M5.update();
  if (Serial.available() > 0)
  {
    String str = Serial.readStringUntil('\n');
    if (str == "IN")
    {
      playMP3("/3h.mp3");
    }
  }
  if (M5.BtnA.wasReleased()) {
    mode_nfc = "IN";
    M5.Lcd.clear(BLACK);
    M5.Lcd.drawString("  IN", half_Width, half_Height);
  } else if (M5.BtnB.wasReleased()) {
  } else if (M5.BtnC.wasReleased()) {
    mode_nfc = "OUT";
    M5.Lcd.clear(BLACK);
    M5.Lcd.drawString(" OUT", half_Width, half_Height);
  }
  Serial.print(mode_nfc);
  delay(10);

}

void playMP3(char *filename)
{
  file = new AudioFileSourceSD(filename);
  id3 = new AudioFileSourceID3(file);
  out = new AudioOutputI2S(0, 1); // Output to builtInDAC
  out->SetOutputModeMono(true);
  out->SetGain(1.0);
  mp3 = new AudioGeneratorMP3();
  mp3->begin(id3, out);
  while (mp3->isRunning())
  {
    if (!mp3->loop())
      mp3->stop();
  }
}
