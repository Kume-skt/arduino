#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// WiFi
const char ssid[] = "Buffalo-G-8F88";
const char passwd[] = "amr3747pbdxhd";

// Pub/Sub
const char* mqttHost = "192.168.11.12"; // MQTTのIPかホスト名
const int mqttPort = 1883;       // MQTTのポート
char pubMessage[512];
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const char* topic = "soil_value/SEN0193";     // 送信先のトピック名
char* payload;                   // 送信するデータ

int val;
///////////////////////////////////////////////////////////////////
String buildJson() {
  String json = "";
  const int capacity = JSON_OBJECT_SIZE(20);
  StaticJsonDocument<capacity> doc;
  DynamicJsonDocument logs(64);
  int val;
  val = analogRead(A6);
  Serial.println(val);
  doc["soil_value"] = val;

  serializeJson(doc, json);

  return json;
}

void setup() {
  Serial.begin(9600);

  // Connect WiFi
  connectWiFi();

  // Connect MQTT
  connectMqtt();
}

void loop() {

  // 送信処理 topic, payloadは適宜
  String jsonStr = buildJson();
  jsonStr.toCharArray(pubMessage, jsonStr.length() + 1);
  mqttClient.publish(topic, pubMessage);
  delay(1000);

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
/**
   Connect MQTT
*/
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
