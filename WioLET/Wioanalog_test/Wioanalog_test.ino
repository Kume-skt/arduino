#include <WioLTEforArduino.h>
#include <WioLTEClient.h>

#define INTERVAL (60000)

#define sen0193_A4 (WIOLTE_A4)
#define sen0193_A5 (WIOLTE_A5)
#define sen0193_A6 (WIOLTE_A6)
#define sen0193_A7 (WIOLTE_A7)

WioLTE Wio;
void setup() {
  // put your setup code here, to run once:
  wio_setUP();

}

void loop() {
  // put your main code here, to run repeatedly:
  int val;
  val = analogRead(sen0193_A4);
  SerialUSB.print("A4:");
  SerialUSB.println(val);
  val = analogRead(sen0193_A5);
  SerialUSB.print("A5:");
  SerialUSB.println(val);

  val = analogRead(sen0193_A6);
  SerialUSB.print("A6:");
  SerialUSB.println(val);

  val = analogRead(sen0193_A7);
  SerialUSB.print("A7:");
  SerialUSB.println(val);

  SerialUSB.println("");
  delay(1000);
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
  delay(500);

}
