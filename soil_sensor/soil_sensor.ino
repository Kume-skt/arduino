#include <WiFi.h>
#include <WiFiUDP.h>
#include "Adafruit_seesaw.h"


Adafruit_seesaw ss;

// 接続先(岩井研究室)のSSIDとパスワード
//const char ssid[] = "CPSLab_WXR_ng";
//const char pass[] = "6bepa8ideapbu";
const char ssid[] = "cps-demo";
const char pass[] = "2019cpsdemo";

static WiFiUDP wifiUdp;
static const char *kRemoteIpadr = "192.168.13.3";
static const int kRmoteUdpPort = 9000; //送信先のポート

static void WiFi_setup()
{
  static const int kLocalPort = 7000;  //自身のポート

  WiFi.begin(ssid, pass);
  while ( WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  wifiUdp.begin(kLocalPort);
}

static void Serial_setup()
{
  Serial.begin(115200);
  Serial.println(""); // to separate line
}

void setup() {
  Serial_setup();
  WiFi_setup();

  Serial.println("seesaw Soil Sensor example!");

  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
    while (1);
  } else {
    Serial.print("seesaw started! version: ");
    Serial.println(ss.getVersion(), HEX);
  }
}

void loop()
{
  float tempC = ss.getTemp();
  uint16_t capread = ss.touchRead(0);

  Serial.print("Temperature: "); Serial.print(tempC); Serial.println("*C");
  Serial.print("Capacitive: "); Serial.println(capread);

  wifiUdp.beginPacket(kRemoteIpadr, kRmoteUdpPort);
//  char send_data = '' + capread;
//  wifiUdp.write((uint8_t*)send_data, 8);
//  Serial.print("send_data"); Serial.println((int)capread);

  wifiUdp.write((uint8_t*)&capread,5);
  wifiUdp.endPacket();

  delay(3000);
}
