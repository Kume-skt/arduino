#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// WiFi
const char ssid[] = "CPSLab_ngpp";
const char passwd[] = "evbskis5dtir7";

// Pub/Sub
const char* mqttHost = "192.168.1.11"; // MQTTのIPかホスト名
const int mqttPort = 1883;       // MQTTのポート
char pubMessage[512];
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const char* topic = "kumeta";     // 送信先のトピック名
    char* payload;                   // 送信するデータ
///////////////////////////////////////////////////////////////////
String buildJson() {
  String json = "";
  const int capacity = JSON_OBJECT_SIZE(20);
  StaticJsonDocument<capacity> doc;
  DynamicJsonDocument logs(64);
  doc["devices"] = "freeboard";
//  data.add(logs);
  serializeJson(doc,json);
  
//  serializeJson(doc,Serial);
  return json;
}

void setup() {
  Serial.begin(115200);

  // Connect WiFi
  connectWiFi();

  // Connect MQTT
  connectMqtt();
}

void loop() {

  // 送信処理 topic, payloadは適宜
  String jsonStr = buildJson();
  jsonStr.toCharArray(pubMessage,jsonStr.length()+1);
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

