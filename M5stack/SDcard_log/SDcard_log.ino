#include <M5Stack.h>

File file;
const char *logFile_name = "log.txt";
void setup()
{
    Serial.begin();
    M5.begin();
    M5.Lcd.fillScreen(Black);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.println("LOG Writer");
}

void loop()
{
    Serial.print("log");
    writeData("log");
    delay(1000);
}

void writeData(char *Str)
{
    file = SD.open(logFile_name, FILE_APPEND);
    file.println(Str);
    file.close();
}