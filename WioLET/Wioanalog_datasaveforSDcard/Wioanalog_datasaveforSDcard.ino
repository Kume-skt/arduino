#include <WioLTEforArduino.h>
#include <WioLTEClient.h>
#if defined ARDUINO_ARCH_STM32F4
#include <SD.h>                    // https://github.com/Seeed-Studio/SD
#elif defined ARDUINO_ARCH_STM32
#include <SDforWioLTE.h>           // https://github.com/SeeedJP/SDforWioLTE
#endif
#define INTERVAL (60000)

#define sen0193_A4 (WIOLTE_A4)
#define sen0193_A5 (WIOLTE_A5)
#define sen0193_A6 (WIOLTE_A6)
#define sen0193_A7 (WIOLTE_A7)

WioLTE Wio;

#define FILE_NAME "test.txt"
File myFile;

void setup()
{
  // put your setup code here, to run once:
  wio_setUP();

  delay(200);

  SerialUSB.println("### Initialize SD card.");
  if (!SD.begin()) {
    SerialUSB.println("### ERROR! ###");
    return;
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  int val1, val2, val3, val4;
  val1 = analogRead(sen0193_A4);
  SerialUSB.print("A4:");
  SerialUSB.println(val1);
  val2 = analogRead(sen0193_A5);
  SerialUSB.print("A5:");
  SerialUSB.println(val2);

  val3 = analogRead(sen0193_A6);
  SerialUSB.print("A6:");
  SerialUSB.println(val3);

  val4 = analogRead(sen0193_A7);
  SerialUSB.print("A7:");
  SerialUSB.println(val4);

  SerialUSB.println("");

  Writ_sd(val1, val2, val3, val4);
  //  myFile.close();//  +"  A5:"+(String)val2+"  A6:"+(String)val3+"  A7:"+(String)val4
  delay(1000);
}

void Writ_sd(int a, int b, int c, int d) {
  SerialUSB.println("### Writing to "FILE_NAME".");
  myFile = SD.open(FILE_NAME, FILE_WRITE);
  if (!myFile) {
    SerialUSB.println("### ERROR! ###");
    return;
  }
  myFile.print("A4:");
  myFile.print(a);

  myFile.print(" A5:");
  myFile.print(b);

  myFile.print("A6:");
  myFile.print(c);

  myFile.print("A7:");
  myFile.println(d);
  myFile.close();
}
void read_sd() {
  SerialUSB.println("### Reading from "FILE_NAME".");
  myFile = SD.open(FILE_NAME);
  if (!myFile) {
    SerialUSB.println("### ERROR! ###");
    return;
  }
  SerialUSB.println(FILE_NAME":");
  while (myFile.available()) {
    SerialUSB.write(myFile.read());
  }
  myFile.close();
}
void wio_setUP()
{
  delay(200);
  SerialUSB.println("");
  SerialUSB.println("--- START ---------------------------------------------------");

  SerialUSB.println("### I/O Initialize.");
  Wio.Init();

  SerialUSB.println("### Power supply ON.");
  Wio.PowerSupplyGrove(true);
  //  Wio.PowerSupplySD(true);
  delay(500);
}
