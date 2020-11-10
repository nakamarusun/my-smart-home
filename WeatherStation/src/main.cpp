#include <Arduino.h>

#include <Wire.h>

// Library ESP
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>

// For async web server
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <ESPAsyncWiFiManager.h> // Library Async Wi-Fi manager.

// Library sensor
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include <BH1750.h>

#include "conf.h"

// Variabel server
AsyncWebServer server(80);
DNSServer dns;

// Deklarasi obyek sensor.
Adafruit_BMP085 bmp;
BH1750 bh;
DHT dht(DHT_PIN, DHT22);
 
void setup() {

  // Jangan pakai serial untuk project ini, karena semua pin terpakai.
  Serial.end();

  Serial.println("Initializing WiFi");

  // WiFi.disconnect();

  // Bikin WiFiManager baru
  AsyncWiFiManager wifiManager(&server, &dns);

  // Disini kita bisa set timeout ketika ESP tidak di setting
  // dalam jangka waktu yang ditentukan, bisa coba connect ke wi-fi lagi.
  wifiManager.setConnectTimeout(CONNECT_TIMEOUT);
  wifiManager.setConfigPortalTimeout(AP_TIMEOUT);
  bool connected = false;

  // Disini kita bisa menentukan apa nama dan password device kita
  // untuk bisa membantu dia berhubung ke Wi-Fi yang kita inginkan
  // Lakukan loop sampai koneksi dibuat.
  do {

    connected = wifiManager.autoConnect(AP_SSID, AP_PASS);
    if (!connected) {
      Serial.println("Gagal koneksi dari memory, membuka mode AP");
      Serial.println("Belum terkoneksi... Mencoba lagi...");
    }

  } while (!connected);

  Serial.println("Koneksi dibuat!");

  // MDNS responder berguna supaya web servernya bisa langsung diakses
  // dari MDNS_NAME.local, dan tidak usah memakai IPnya 
  if (MDNS.begin(MDNS_NAME)) {
    MDNS.addService("http", "tcp", 80);
    Serial.print("mDNS responder dimulai di ");
    Serial.println(MDNS_NAME);
  } else {
    Serial.print("mDNS responder gagal dimulai di ");
    Serial.println(MDNS_NAME);
  }



  server.begin();

}
 
void loop() {
 
}