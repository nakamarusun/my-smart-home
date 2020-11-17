#include <SPIFFS.h> // HARUS PALING ATAS
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

#include <Wire.h>
#include <SSD1306Wire.h>

#include "conf.h"
#include "html.h"
#include "util.h"
#include "lcd.h"

Btn doorbell(BTN_PIN);
Btn proxSensor(PROX_PIN);

void publishDoorbell() {
  // Akan mengpublish message di topik
  // jika tombol bel ditekan.
  HttpServer::mqtt.publish(BTN_TOPIC, "1");
}

void setup() {

  #ifdef DEBUG_MODE
  // Mulai serial
  Serial.begin(9600);
  #endif

  using namespace LCD;
  // Mulai LED display
  display.init();
  display.flipScreenVertically();

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawStringMaxWidth(0, 0, 128, "Booting...");
  display.display();
  delay(2000);

  display.clear();
  display.drawXbm(0, 0, 128, 64, splashLogo);
  display.display();
  delay(2000);

  display.clear();
  display.drawStringMaxWidth(0, 0, 128, "Welcome to the system!");
  display.display();

  // Mulai SPIFFS;
  if (!SPIFFS.begin(true)) {
    Serial.println("Gagal menginisialisasi SPIFFS, mengformat...");
  }
  // Buat imagenya semua
  Util::createIfNotExist("/brokerhost");
  Util::createIfNotExist("/message");

  Serial.println("Initializing WiFi");

  // WiFi.disconnect();

  // Bikin WiFiManager baru
  WiFiManager wifiManager;
  wifiManager.setDebugOutput(false);

  // Disini kita bisa set timeout ketika ESP tidak di setting
  // dalam jangka waktu yang ditentukan, bisa coba connect ke wi-fi lagi.
  wifiManager.setConnectTimeout(CONNECT_TIMEOUT);
  wifiManager.setConfigPortalTimeout(AP_TIMEOUT);
  bool connected = false;

  // Disini kita bisa menentukan apa nama dan password device kita
  // untuk bisa membantu dia berhubung ke Wi-Fi yang kita inginkan
  // Lakukan loop sampai koneksi dibuat.
  WiFi.mode(WIFI_STA);
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

  loadLcdText();
  updateLcd();

  // Init kameranya dan beri konfigurasi yang sesuai.
  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(Resolution::find(800, 600));
    cfg.setBufferCount(3);
    cfg.setJpeg(95);

    HttpServer::cameraStatus = Camera.begin(cfg);
    Serial.println(HttpServer::cameraStatus ? "Kamera berhasil di aktifkan" : "Kamera gagal diaktifkan.");
  }

  // Mulai servernya
  HttpServer::initServer();
  
  // Buffer LCD disini jadi error gak tau kenapa...
  // Jadi harus di init lagi.
  // display.init();
  // display.flipScreenVertically();

  Serial.println("Semua sistem sukses di inisialisasi!");
  #ifndef DEBUG_MODE
  Serial.end();
  #endif
}

void loop() {
  // Lakukan setiap loop
  HttpServer::server.handleClient();

  // Untuk MQTT loop
  HttpServer::mqttLoop();

  // Cek tombolnya
  doorbell.updateButton();
  proxSensor.updateButton();
  if (doorbell.checkButtonOnce()) { 
    publishDoorbell(); 
    LCD::updateLcd();
  }

  // Update PIR proximity sensornya
  /*
  Snippet kode ini berguna untuk meng-publish pesan
  jika sensornya aktif selama X detik. Setelah mengpublish,
  Sensor akan direset agar tidak terjadi spam.
  */
  if (proxSensor.checkButtonOnce()) {
    LCD::updateLcd();
    Serial.println("Proximity triggered!");
    HttpServer::mqtt.publish(PROX_TOPIC, "1");
  }

  // Loop LCDnya.
  LCD::lcdLoop();
}