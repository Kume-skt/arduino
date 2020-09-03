#include <WioLTEforArduino.h>
#include <WioLTEClient.h>
#include <PubSubClient.h> // https://github.com/SeeedJP/pubsubclient
#include <stdio.h>
#include <ArduinoJson.h>

#define APN "soracom.io"
#define USERNAME "sora"
#define PASSWORD "sora"

#define MQTT_SERVER_HOST "beam.soracom.io"
#define MQTT_SERVER_PORT (1883)

#define ID "AWS IoT"
#define OUT_TOPIC "test"
#define IN_TOPIC "inTopic"

#define INTERVAL (60000)

#define sen0193_A4 (WIOLTE_A4)
#define sen0193_A5 (WIOLTE_A5)
#define sen0193_A6 (WIOLTE_A6)
#define sen0193_A7 (WIOLTE_A7)

char pubMessage[512];

WioLTE Wio;
WioLTEClient WioClient(&Wio);
PubSubClient MqttClient;

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
  SerialUSB.println("### Setup completed.");
}

void loop()
{
  //  wio_setUP();
  //  connectMqtt();

  String data = buildJson();

  SerialUSB.print("Publish:");
  SerialUSB.print(data);
  SerialUSB.println("");

  data.toCharArray(pubMessage, data.length() + 1);
  MqttClient.publish(OUT_TOPIC, pubMessage);
  //  wio_down();
  Wio.Sleep();
err:

//  delay(INTERVAL);
    unsigned long next = millis();
    while (millis() < next + INTERVAL)
    {
//      MqttClient.loop();
    }  
  Wio.Wakeup();
  
//  connectMqtt();
//  delay(200);
}

String buildJson()
{
  String json = "";
  const int capacity = JSON_OBJECT_SIZE(20);
  StaticJsonDocument<capacity> doc;
  DynamicJsonDocument logs(64);
  int val;
  val = analogRead(sen0193_A4);
  doc["soil_valueA"] = val;

  val = analogRead(sen0193_A5);
  doc["soil_valueB"] = val;

  val = analogRead(sen0193_A6);
  doc["soil_valueC"] = val;

  val = analogRead(sen0193_A7);
  doc["soil_valueD"] = val;

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
  MqttClient.subscribe(IN_TOPIC);
}

void wio_setUP()
{
  delay(200);

  SerialUSB.println("");
  SerialUSB.println("--- START ---------------------------------------------------");

  SerialUSB.println("### I/O Initialize.");
  Wio.Init();

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
