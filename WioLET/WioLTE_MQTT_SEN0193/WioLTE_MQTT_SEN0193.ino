#include <WioLTEforArduino.h>
#include <WioLTEClient.h>
#include <PubSubClient.h> // https://github.com/SeeedJP/pubsubclient
#include <stdio.h>
#include <ArduinoJson.h>
#include <WioRTC.h>

#if defined ARDUINO_ARCH_STM32F4
#include <SD.h> // https://github.com/Seeed-Studio/SD
#elif defined ARDUINO_ARCH_STM32
#include <SDforWioLTE.h> // https://github.com/SeeedJP/SDforWioLTE
#endif

#define APN "soracom.io"
#define USERNAME "sora"
#define PASSWORD "sora"

#define MQTT_SERVER_HOST "beam.soracom.io"
#define MQTT_SERVER_PORT (1883)

#define ID "AWS IoT"
#define OUT_TOPIC "test"
#define IN_TOPIC "inTopic"

#define BOOT_INTERVAL (30) // [sec.]

#define sen0193_A4 (WIOLTE_A4)
#define sen0193_A5 (WIOLTE_A5)
#define sen0193_A6 (WIOLTE_A6)
#define sen0193_A7 (WIOLTE_A7)

char pubMessage[512];

WioLTE Wio;
WioRTC Rtc;

WioLTEClient WioClient(&Wio);
PubSubClient MqttClient;

#define FILE_NAME "log.txt"
File myFile;

void callback(char *topic, byte *payload, unsigned int length)
{
  SerialUSB.print("Subscribe:");
  for (int i = 0; i < length; i++)
    SerialUSB.print((char)payload[i]);
  SerialUSB.println("");
}

void setup()
{
  wio_setUP();
  connectMqtt();

//  SerialUSB.println("### Initialize SD card.");
//  if (!SD.begin())
//  {
//    SerialUSB.println("### ERROR! ###");
//    return;
//  }
  SerialUSB.println("### Setup completed.");
  String data = buildJson();

  SerialUSB.print("Publish:");
  SerialUSB.print(data);
  SerialUSB.println("");

  data.toCharArray(pubMessage, data.length() + 1);
  MqttClient.publish(OUT_TOPIC, pubMessage);
err:

  delay(2000);

  SerialUSB.println("Shutdown.");
  Rtc.SetWakeupPeriod(BOOT_INTERVAL);
  Rtc.Shutdown();
}

void loop()
{
}

String buildJson()
{
  // データ作成
  String json = "";
  const int capacity = JSON_OBJECT_SIZE(20);
  StaticJsonDocument<capacity> doc;
  DynamicJsonDocument logs(64);
  int val1, val2, val3, val4;
  val1 = analogRead(sen0193_A4);
  doc["soil_valueA"] = val1;

  val2 = analogRead(sen0193_A5);
  doc["soil_valueB"] = val2;

  val3 = analogRead(sen0193_A6);
  doc["soil_valueC"] = val3;

  val4 = analogRead(sen0193_A7);
  doc["soil_valueD"] = val4;

//  Writ_sd(val1, val2, val3, val4);
  serializeJson(doc, json);

  return json;
}
void connectMqtt()
{
  SerialUSB.println("### Connecting to MQTT server \"" MQTT_SERVER_HOST "\"");
  MqttClient.setServer(MQTT_SERVER_HOST, MQTT_SERVER_PORT);
  MqttClient.setCallback(callback);
  MqttClient.setClient(WioClient);
  if (!MqttClient.connect(ID))
  {
    SerialUSB.println("### ERROR! ###");
    return;
  }
  delay(1000);
}

void Writ_sd(int a, int b, int c, int d)
{
  //sdcardに書き込み
  SerialUSB.println("### Writing to " FILE_NAME ".");
  myFile = SD.open(FILE_NAME, FILE_WRITE);
  if (!myFile)
  {
    SerialUSB.println("### ERROR! ###");
    return;
  }
  myFile.print("A4:");
  myFile.print(a);

  myFile.print(" A5:");
  myFile.print(b);

  myFile.print(" A6:");
  myFile.print(c);

  myFile.print(" A7:");
  myFile.println(d);
  myFile.close();
}

void read_sd()
{
  //sdcard読み込み
  SerialUSB.println("### Reading from " FILE_NAME ".");
  myFile = SD.open(FILE_NAME);
  if (!myFile)
  {
    SerialUSB.println("### ERROR! ###");
    return;
  }
  SerialUSB.println(FILE_NAME ":");
  while (myFile.available())
  {
    SerialUSB.write(myFile.read());
  }
  myFile.close();
}

void wio_setUP()
{
  delay(200);

  SerialUSB.println("### I/O Initialize.");
  Wio.Init();

  Wire.begin();
  Rtc.begin();

  SerialUSB.println("### Power supply ON.");
  Wio.PowerSupplyLTE(true);
  Wio.PowerSupplyGrove(true);
  delay(500);

  SerialUSB.println("### Turn on or reset.");
  if (!Wio.TurnOnOrReset())
  {
    SerialUSB.println("### ERROR! ###");
    return;
  }

  SerialUSB.println("### Connecting to \"" APN "\".");
  if (!Wio.Activate(APN, USERNAME, PASSWORD))
  {
    SerialUSB.println("### ERROR! ###");
    return;
  }
}
void wio_down()
{
  delay(200);

  SerialUSB.println("### Power supply OFF.");
  Wio.PowerSupplyLTE(false);
  Wio.PowerSupplyGrove(false);
}
