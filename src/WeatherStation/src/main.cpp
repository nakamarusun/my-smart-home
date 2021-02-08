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

// PubSub library
#include <PubSubClient.h>

#include "conf.h"
#include "html.h"

// Variabel server
AsyncWebServer server(80);
DNSServer dns;

// Variabel jika masih dalam config.
bool initConfig = true;

// Broker MQTTnya
String brokerMQTT = "";

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
  server.on("/", HTTP_POST, HtmlResponder::confPost);
  server.on("/", HTTP_GET, HtmlResponder::conf);
  server.on("/style", HTTP_GET, HtmlResponder::style);
  server.on("/mqtt", HTTP_GET, HtmlResponder::mqtt);

  server.onNotFound(HtmlResponder::fourOFour);

  server.begin();
}

void sleepWifi() {
  WiFi.disconnect();
  WiFi.mode( WIFI_OFF );
  WiFi.forceSleepBegin();
}

void lightSleep(long milli) {

  Serial.println("Going to sleep...");
  // Sleep code
  wifi_station_disconnect();
  wifi_set_opmode(NULL_MODE);
  wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
  wifi_fpm_open();
  wifi_fpm_do_sleep(milli * 1000);
  delay(milli + 1);

  // Wake up code
  willSleep = false;
  wifi_set_sleep_type(NONE_SLEEP_T);
  Serial.println("Waking up");
  initWifi();
}
 
void setup() {

  // Sangat penting. Ini karena seringkali penalokasi i2c tidak
  // otomatis di 0 dan 2.
  Wire.begin(0, 2);

  #ifdef SERIAL_DEBUG
  Serial.begin(9600); // Hanya untuk debug!
  #endif

  // Mulai LittleFS
  LittleFS.begin();
  Util::createIfNotExist("/mqtt");
  Util::createIfNotExist("/updateTime");
  if (LittleFS.exists("/updateTime")) {
    String time = Util::readFileAsString("/updateTime");
    updateEvery = time.length() == 0 ? UPDATE_EVERY : time.toInt();
  }

  // Init wifinya
  initWifi();

  // Mulai semua sensor disini.
  Sensor::dht.begin();
  Sensor::bmp_status = Sensor::bmp.begin();
  Sensor::bh_status = Sensor::bh.begin(BH1750::Mode::ONE_TIME_HIGH_RES_MODE);
  Sensor::rns.begin();

  // Jangan pakai serial untuk project ini, karena semua pin terpakai.
  #ifndef SERIAL_DEBUG
  Serial.end();
  #endif
  Serial.println("Server started!");
}

void publishMQTT() {
  WiFiClient espClient;
  PubSubClient client(espClient);
  const int REPEAT = 10;

  Serial.println("Initiating routine MQTT publishing");

  // Setel servernya.
  client.setServer(brokerMQTT.c_str(), 1883);

  for (int i = 0; i < REPEAT; i++) {

    Serial.println("Connecting to the MQTT broker");
    Serial.println(brokerMQTT);

    if (client.connect(MDNS_NAME)) {
      client.loop();
      // Koneksi dibuat. Lakukan sesuatu disini.
      Serial.println("Connected to broker!");
      DynamicJsonDocument doc(1024);
      
      doc["pressure"] = Sensor::pressure;
      doc["temp"] = Sensor::temp;
      doc["hindex"] = Sensor::heatIndex;
      doc["humid"] = Sensor::humid;
      doc["light"] = Sensor::light;
      doc["rain"] = Sensor::rain;

      // Masukkan hasil json ke string.
      String result = "";
      serializeJson(doc, result);
      Serial.println(result);

      // Kemudian send.
      client.publish(TOPIC_NAME, result.c_str());

      // Tunggu sampai semua datanya disend.
      delay(1000);
      client.loop();
      delay(1000);
      Serial.println("Done sending MQTT");

      // Kalau terkoneksi, keluar dari loop.
      break;
    }
    // Tunggu 2 detik sebelum mencoba lagi.
    delay(2000);
  }
}

void loop() {
  
  // Update semua sensornya.
  Sensor::update();

  if (initConfig) {
    // Masih didalam mode config
    if (exitConfigMarker < millis()) {
      initConfig = false;
      brokerMQTT = Util::readFileAsString("/mqtt");
    }
  } else {
    // Diluar mode config
    // Disini kita bisa update secara periodis ke server MQTT
    // Lakukan update MQTT
    publishMQTT();
    // Tidur.
    lightSleep(updateEvery * 1000);
  }

  delay(50);
}