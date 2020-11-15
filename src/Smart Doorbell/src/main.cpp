#include <Arduino.h>
#include <esp32cam.h>

// Library ESP
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiClient.h>

// For Web server
// Manager wifi connection
#include <WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>

#include "conf.h"
#include "html.h"
void setup() {

  // Mulai serial
  Serial.begin(9600);

  Serial.println("Initializing WiFi");

  // WiFi.disconnect();

  // Bikin WiFiManager baru
  WiFiManager wifiManager;

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

  // Init kameranya dan beri konfigurasi yang sesuai.
  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(Resolution::find(800, 600));
    cfg.setBufferCount(3);
    cfg.setJpeg(95);

    bool ok = Camera.begin(cfg);
    Serial.println(ok ? "Kamera berhasil di aktifkan" : "Kamera gagal diaktifkan.");
    Serial.println("Bruh?");
  }

  // Mulai servernya
  HttpServer::initServer();

  Serial.println("Semua sistem sukses di inisialisasi!");
}

void loop() {
  HttpServer::server.handleClient();
}