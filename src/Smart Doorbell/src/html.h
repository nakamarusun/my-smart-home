// Web server
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <DNSServer.h>

// Kamera
#include <esp32cam.h>

// MQTT
#include <PubSubClient.h>

#include "conf.h"
#include "html_file.h"
#include "util.h"

namespace HttpServer {
    // Variable server
    WebServer server(80);
    DNSServer dns;

    // Untuk MQTT
    int reconTimeTry = 2000;
    long lastReconTime;
    WiFiClient espClient;
    PubSubClient mqtt(espClient);

    void handleCapture() {
        auto frame = esp32cam::capture();
            if (frame == nullptr) {
            Serial.println("CAPTURE FAIL");
            server.send(503, "", "");
            return;
        }
        Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),
                    static_cast<int>(frame->size()));

        server.setContentLength(frame->size());
        server.send(200, "image/jpeg");
        WiFiClient client = server.client();
        frame->writeTo(client);
    }

    void handleIndex() {
        server.send(200, "text/html", index_html);
    }

    void handleIndexPost() {
        // Ubah broker MQTT.
        if (server.hasArg("mqtt")) {
            File file = SPIFFS.open("/brokerhost", "w");
            file.print(server.arg("mqtt"));
            file.flush();
            file.close();
        }
        server.sendHeader("Location", String("/"), true);
        server.send (302, "text/plain", "");
    }

    void handleBrokerhost() {
        String host = Util::readFileAsString("/brokerhost");
        server.send(200, "text/plain", host);
    }
    
    void handleBrokerstatus() {
        
        if (mqtt.connected()) {
            server.send(200, "text/plain", "Connection successfully made!");
        } else {
            server.send(200, "text/plain", "Failed to make connection.");
        }
    }

    void fourOFour() {
        server.send(404, "text/plain", "No bueno lol");
    }

    void initServer() {
        // Setel root server
        server.on("/", HTTP_GET, handleIndex);
        server.on("/", HTTP_POST, handleIndexPost);
        server.on("/capture", HTTP_GET, handleCapture);
        server.on("/brokerhost", HTTP_GET, handleBrokerhost);
        server.on("/brokerstatus", HTTP_GET, handleBrokerstatus);
        server.onNotFound(fourOFour);
        
        // Setel server koneksi MQTT.
        mqtt.setServer(Util::readFileAsString("/brokerhost").c_str(), 1883);

        server.begin();
    }

    void mqttLoop() {
        if (!mqtt.connected()) {
            if (lastReconTime + reconTimeTry < millis()) {
                Serial.println("MQTT not connected.. Reconnecting..");
                mqtt.connect(MDNS_NAME);
                lastReconTime = millis();
            }
        } else {
            mqtt.loop();
        }
    }
}