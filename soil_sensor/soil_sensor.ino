#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "Adafruit_seesaw.h"
//l2c sensor
Adafruit_seesaw ss;
// WiFi
const char ssid[] = "CPSLAB_WLX";
const char passwd[] = "6bepa8ideapbu";

// Pub/Sub
const char* mqttHost = "192.168.12.5"; // MQTTのIPかホスト名
const int mqttPort = 1883;       // MQTTのポート
char pubMessage[512];
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const char* topic = "kumeta";     // 送信先のトピック名
char* payload;                   // 送信するデータ
///////////////////////////////////////////////////////////////////
String buildJson() {
  //センサー値取得
  float tempC = ss.getTemp();
  uint16_t capread = ss.touchRead(0);

  String json = "";
  const int capacity = JSON_OBJECT_SIZE(20);
  StaticJsonDocument<capacity> doc;
  DynamicJsonDocument logs(64);
  doc["capread"] = capread;
  doc["tempC"] = tempC;

  serializeJson(doc, json);

  //  serializeJson(doc,Serial);
  return json;
}

void setup() {
  Serial.begin(115200);

  // Connect WiFi
  connectWiFi();

  // Connect MQTT
  connectMqtt();
  /////////////////////////////////////////
  //l2cセンサー
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
    while (1);
  } else {
    Serial.print("seesaw started! version: ");
    Serial.println(ss.getVersion(), HEX);
  }

}

void loop() {

  // 送信処理 topic, payloadは適宜
  String jsonStr = buildJson();
  jsonStr.toCharArray(pubMessage, jsonStr.length() + 1);
  mqttClient.publish(topic, pubMessage);
  delay(3000);

  // WiFi
  if ( WiFi.status() == WL_DISCONNECTED ) {
    connectWiFi();
  }
  // MQTT
  if ( ! mqttClient.connected() ) {
    connectMqtt();
  }
  mqttClient.loop();

}

/**
   Connect WiFi
*/
void connectWiFi()
{
  WiFi.begin(ssid, passwd);
  Serial.print("WiFi connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.print(" connected. ");
  Serial.println(WiFi.localIP());
}
void connectMqtt()
{
  mqttClient.setServer(mqttHost, mqttPort);
  while ( ! mqttClient.connected() ) {
    Serial.println("Connecting to MQTT...");
    String clientId = "ESP32-" + String(random(0xffff), HEX);
    if ( mqttClient.connect(clientId.c_str()) ) {
      Serial.println("connected");
    }
    delay(1000);
    randomSeed(micros());
  }
}

