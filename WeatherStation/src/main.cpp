#include <LittleFS.h>
#include <Arduino.h>

#include <Wire.h>

// Library ESP
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>

// Untuk async web server
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <ESPAsyncWiFiManager.h> // Library Async Wi-Fi manager.

#include "conf.h"
#include "html.h"

// Variabel server
AsyncWebServer server(80);
DNSServer dns;

// Variabel jika masih dalam config.
bool initConfig = true;

void initWifi() {

  // Reset semuanya dulu.
  server.reset();
  server.end();

  Serial.println("Initializing WiFi");

  // WiFi.disconnect();
  WiFi.mode( WIFI_STA );

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

  server.on("/json", HTTP_GET, HtmlResponder::dataQuery);
  server.on("/status", HTTP_GET, HtmlResponder::status);
  server.on("/sleep", HTTP_GET, HtmlResponder::sleep);
  server.on("/conf", HTTP_GET, HtmlResponder::conf);

  server.onNotFound(HtmlResponder::fourOFour);

  server.begin();
}

void sleepWifi() {
  WiFi.disconnect();
  WiFi.mode( WIFI_OFF );
  WiFi.forceSleepBegin();
}

void lightSleep(long milli) {

  // Sleep code
  wifi_set_opmode(NULL_MODE);
  wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
  wifi_fpm_open();
  wifi_fpm_do_sleep(milli * 1000);
  delay(milli + 1);

  // Wake up code
  willSleep = false;
  wifi_set_sleep_type(NONE_SLEEP_T);
  initWifi();
}
 
void setup() {

  // Sangat penting. Ini karena seringkali penalokasi i2c tidak
  // otomatis di 0 dan 2.
  Wire.begin(0, 2);

  // Mulai LittleFS
  LittleFS.begin();
  if (!LittleFS.exists("/mqtt")) {
    File file = LittleFS.open("/mqtt", "w");
    file.flush();
    file.close();
  }

  // Init wifinya
  initWifi();

  // Mulai semua sensor disini.
  Sensor::dht.begin();
  Sensor::bmp_status = Sensor::bmp.begin();
  Sensor::bh_status = Sensor::bh.begin(BH1750::Mode::ONE_TIME_HIGH_RES_MODE);
  Sensor::rns.begin();

  // Jangan pakai serial untuk project ini, karena semua pin terpakai.
  Serial.end();
}

void publishMQTT() {

}

void loop() {
  
  // Update semua sensornya.
  Sensor::update();

  if (initConfig) {
    // Masih didalam mode config
    if (exitConfigMarker < millis()) {
      initConfig = false;
    }
  } else {
    // Diluar mode config
    // Disini kita bisa update secara periodis ke server MQTT
  }

  delay(1);
}