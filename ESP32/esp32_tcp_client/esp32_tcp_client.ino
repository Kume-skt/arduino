//This sketch sends a message to a TCP server
#include <WiFi.h>
#include <WiFiClient.h> 
/* Set these to your desired credentials. */
const char *ssid = "Buffalo-G-8F88";
const char *password = "amr3747pbdxhd";
WiFiClient client;
const uint16_t port = 9000;
const char * host = "192.168.11.3"; // ip or dns
void setup() {
  delay(10);
  WiFi.softAP(ssid, password);  
  while(!client.connect(host, port)) {
    delay(500);
  }
}

void loop() {
  client.println(String(random(1000)%1023));
  delay(500);
}
