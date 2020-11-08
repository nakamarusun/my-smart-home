#include <LittleFS.h> // Library file

#include <Arduino.h>

// Library ESP
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>

// For async web server
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Library ESP
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>

#include <ArduinoJson.h>
#include <AsyncJson.h>

#include <ESPAsyncWiFiManager.h> // Library Async Wi-Fi manager.

#include "html.h" // Dimana file file html kita hidup.
#include "conf.h"

// Variabel LED infra merah
IRrecv irrecv(RECV_PIN, 1024, 50, false);
IRsend irsend(LED_PIN);
decode_results irResult;

// Variable server
AsyncWebServer server(80);
DNSServer dns;

// Processor for htmls
// Ini akan menggantikan semua nilai yang ada %% nya di
// HTML dengan variable yang ada.


void setup() {

  Serial.begin(9600);
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

  // Mulai LittleFS
  LittleFS.begin();

  // Buat file json jika belum ada
  if (!LittleFS.exists(REMOTE_FILE)) {

    File file = LittleFS.open(REMOTE_FILE, "w");
    file.write("{\"remotes\":[]}");
    file.flush();
    file.close();
  } else {
    // Cek jika ada remote didalam.
    DynamicJsonDocument doc(1024);
    File file = LittleFS.open(REMOTE_FILE, "r");
    deserializeJson(doc, file);
    // Print any errors.
    JsonObject json = doc.as<JsonObject>();
    file.close();
    if (!json.containsKey("remotes")) {
        file = LittleFS.open(REMOTE_FILE, "w");
        file.write("{\"remotes\":[]}");
        file.flush();
        file.close();
      }
  }
    
  // Mulai inframerah
  irrecv.enableIRIn();
  irsend.begin();

  server.on("/", HTTP_GET, HtmlResponder::index);
  server.on("/remote/add", HTTP_GET, HtmlResponder::addButton);
  server.on("/remote/done", HTTP_GET, HtmlResponder::doneAddButton);
  server.on("/remote/done", HTTP_POST, HtmlResponder::doneAddButtonPost);
  server.on("/remote/file", HTTP_GET, HtmlResponder::remoteFile);
  server.onNotFound(HtmlResponder::notFound);

  // Mulai servernya
  server.begin();

  Serial.println("Semua sistem sukses di mulai!");
}

void loop() {
  // put your main code here, to run repeatedly:
  MDNS.update();

  if (HtmlResponder::isReceivingIR) {

    if (irrecv.decode(&irResult)) {
      Serial.println("RECEIVED IR SIGNAL");
      // HtmlResponder::rawArrIR = resultToRawArray(&irResult);
      HtmlResponder::sizeIR = getCorrectedRawLength(&irResult);
      
      irrecv.resume();
      HtmlResponder::isReceivingIR = false;
    }
  }
}