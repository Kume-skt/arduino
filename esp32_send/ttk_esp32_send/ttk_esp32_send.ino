#include <WiFi.h>
#include <WiFiUDP.h>

// 接続先(岩井研究室)のSSIDとパスワード
static const char * ssid = "Buffalo-G-8F88";
static const char * pass = "amr3747pbdxhd";

static WiFiUDP wifiUdp;

static const char *kRemoteIpadr = "192.168.11.3";
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
}

void loop()
{
  uint16_t send_data = 1015;
  wifiUdp.beginPacket(kRemoteIpadr, kRmoteUdpPort);
  wifiUdp.write((uint8_t*)&send_data, 5);
  wifiUdp.endPacket();

  delay(3000);
}
