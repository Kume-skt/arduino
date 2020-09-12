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

const char* topic = "test";     // 送信先のトピック名
char* payload;                   // 送信するデータ


void setup() {
  Serial.begin(9600);

  // Connect WiFi
  connectWiFi();

  // Connect MQTT
  connectMqtt();
}

void loop() {

  if(!mqttClient.connected()) {
    if (mqttClient.connect(topic)) {
      Serial.println("Connected.");    
      mqttClient.subscribe(topic, 0);
      Serial.println("Subscribed.");
    }
    else {
      errorReport();
    }
  }
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
  mqttClient.setCallback(callback);
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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void errorReport(){
  Serial.print("Failed. Error state = ");

  switch (mqttClient.state()) {
    case MQTT_CONNECT_UNAUTHORIZED:
      Serial.println("MQTT_CONNECT_UNAUTHORIZED");
      break;
    case MQTT_CONNECT_BAD_CREDENTIALS:
      Serial.println("MQTT_CONNECT_BAD_CREDENTIALS");
      break;
    case MQTT_CONNECT_UNAVAILABLE:
      Serial.println("MQTT_CONNECT_UNAVAILABLE");
      break;
    case MQTT_CONNECT_BAD_CLIENT_ID:
      Serial.println("MQTT_CONNECT_BAD_CLIENT_ID");
      break;
    case MQTT_CONNECT_BAD_PROTOCOL:
      Serial.println("MQTT_CONNECT_BAD_PROTOCOL");
      break;
    case MQTT_CONNECTED:
      Serial.println("MQTT_CONNECTED");
      break;
    case MQTT_DISCONNECTED:
      Serial.println("MQTT_DISCONNECTED");
      break;
    case MQTT_CONNECT_FAILED:
      Serial.println("MQTT_CONNECT_FAILED");
      break;
    case MQTT_CONNECTION_LOST:
      Serial.println("MQTT_CONNECTION_LOST");
      break;
    case MQTT_CONNECTION_TIMEOUT:
      Serial.println("MQTT_CONNECTION_TIMEOUT");
      break;
  }

  delay(5000); // Wait 5 seconds before retrying
}
