// Web server
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <DNSServer.h>

// Kamera
#include <esp32cam.h>

namespace HttpServer {
    // Variable server
    WebServer server(80);
    DNSServer dns;

    void initServer() {
        // Setel root server
        server.on("/", HTTP_GET, [](){
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
        });

        server.begin();
    }
}