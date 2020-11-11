#include <Arduino.h>

// Library ESP
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Library sensor
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include <BH1750.h>

#include <ArduinoJson.h> // Arduino json library

#include "conf.h"

bool willSleep = false;

namespace Sensor {

    struct RainSense {
        private:
        int _pin;

        public:
        RainSense(int pin) {_pin = pin;}

        void begin() {
            pinMode(_pin, INPUT);
        }

        bool readIsWet() {
            return digitalRead(_pin) == LOW ? true : false;
        }
    };

    // Status sensornya.
    bool bmp_status;
    bool bh_status;

    // Deklarasi obyek sensor.
    Adafruit_BMP085 bmp;
    BH1750 bh;
    DHT dht(DHT_PIN, DHT22);
    RainSense rns(WET_PIN);

    // Nilai - nilai sensor
    int32_t pressure;
    float temp;
    float heatIndex;
    float humid;
    float light;
    bool rain;

    void update() {
        pressure = Sensor::bmp_status ? Sensor::bmp.readPressure() : -1;
        temp = Sensor::dht.readTemperature();
        heatIndex = Sensor::dht.computeHeatIndex(false);
        humid = Sensor::dht.readHumidity();
        light = Sensor::bh_status ? Sensor::bh.readLightLevel() : -1;
        rain = Sensor::rns.readIsWet();
    }
};

// Html Responder untuk semua permintaan HTTP dari
// user.
namespace HtmlResponder {

    void sleep(AsyncWebServerRequest* request) {
        request->send(200, "text/plain", "Sleeping...");
        willSleep = true;
    }

    void status(AsyncWebServerRequest* request) {

        String result = "BMP180 Active: ";
        result += Sensor::bmp_status ? "True" : "False";
        result += "\nBH1750 Active: ";
        result += Sensor::bh_status ? "True" : "False";

        AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", result);
        request->send(response);
    }

    void dataQuery(AsyncWebServerRequest* request) {
        
        // Make a new json document
        DynamicJsonDocument doc(1024);

        // Masukkan sensor data ke dokumen json
        // Karena kita punya 2 sensor yang bisa merekam temperatur,
        // kita ratakan mereka berdua.
        doc["pressure"] = Sensor::pressure;
        doc["temp"] = Sensor::temp;
        doc["hindex"] = Sensor::heatIndex;
        doc["humid"] = Sensor::humid;
        doc["light"] = Sensor::light;
        doc["rain"] = Sensor::rain;

        // Masukkan hasil json ke string.
        String result = "";
        serializeJson(doc, result);

        // Send ke site.
        AsyncWebServerResponse *response = request->beginResponse(200, "application/json", result);
        request->send(response);
    }
};