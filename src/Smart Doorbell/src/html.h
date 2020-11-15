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
        WiFiClient espClient;
        PubSubClient client(espClient);
        const int REPEAT = 10;
        
        // Broker MQTT
        String str = Util::readFileAsString("/brokerhost");
        if (str.length() != 0) {
            client.setServer(Util::readFileAsString("/brokerhost").c_str(), 1883);

            for (int i = 0; i < REPEAT; i++) {
                if (client.connect(MDNS_NAME)) {
                    server.send(200, "text/plain", "Connection successfully made!");
                    return;
                }
                delay(100);
            }
        }
        server.send(200, "text/plain", "Failed to make connection.");
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

        server.begin();
    }
}