#include <Arduino.h>

#include <LittleFS.h>

// Library ESP
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// IR Sender
#include <IRsend.h>

#include "conf.h"
#include "html_file.h"
#include "util.h"

namespace HtmlResponder {

    // Dibutuhkan jika mesin menerima sinyal IR untuk di save.
    bool isReceivingIR = false;

    IRsend irsend(LED_PIN);

    uint16_t *rawArrIR;
    uint16_t sizeIR;

    // Processors
    String indexProc(const String& var) {

      String result = "";
      
      if (var == "IP") {
        result += "`";
        result += MDNS_NAME;
        result += "/` atau `";
        result += WiFi.localIP().toString();
        result += "`";
      } else if (var == "REMOTES_LIST") {

        /*
        Struktur file:
        {
        "remotes": [{
            "id": 1,
            "caption": "bruhbruh",
            "logo": "+",
            "color": "#FFF",
            "data": []
        }, ...]
        }
        */

        // Initial allocation
        DynamicJsonDocument doc(1024);
        File file = LittleFS.open(REMOTE_FILE, "r");
        
        deserializeJson(doc, file);

        // the Json object to manipulate
        JsonObject obj = doc.as<JsonObject>();

        // Close the file
        file.close();

        // Dapatkan array remotenya.
        JsonArray remotes = obj["remotes"];

        for (auto val : remotes) {
            result += "<a onclick=\"btn("
                + val["id"].as<String>()
                + ")\" class=\"bu s_bu lg\" style=\"background: "
                + val["color"].as<String>()
                + ";\">"
                + val["logo"].as<String>()
                + "<div class=\"cp\">"
                + val["id"].as<String>() + ". " + val["caption"].as<String>()
                + "</div></a>";
        }
      }

      // Just for safety
      return result;
    }

    String doneAddButtonProc(const String& var) {
        if (var == "DATA") {
            String result = "";
            for (uint16_t i = 0; i < sizeIR; i++) {
                result += rawArrIR[i];
                result += " ";
            }
            return result;
        }
        return "";
    }

    // Responders
    void styleResponder(AsyncWebServerRequest* request) {

        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css", style_html);
        request->send(response);
    }

    void tempIR(AsyncWebServerRequest* request) {
        request->send(LittleFS, "/temp_ir", "text/plain");
    }


    void remoteFile (AsyncWebServerRequest* request) {
        request->send(LittleFS, REMOTE_FILE, "application/json");
    }

    void index(AsyncWebServerRequest* request) {

        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html, indexProc);
        request->send(response);
    }

    void notFound(AsyncWebServerRequest* request) {
        request->send(404, "text/plain", "no bueno lol");
    }

    void clickRemote(AsyncWebServerRequest* request) {

        String strResult = "";

        // Ini adalah responder untuk request html GET jika user mengeklik sesuatu.
        if (request->hasParam("id")) {

            // Masukkan ke memory.
            DynamicJsonDocument doc(1024);
            File file = LittleFS.open(REMOTE_FILE, "r");
            deserializeJson(doc, file);

            JsonObject json = doc.as<JsonObject>();

            file.close();

            // Masukkan ke JsonArray
            JsonArray remotes = json["remotes"];
            // Dapatkan dari POST request ID mana yang mau didelete.
            String idString = request->getParam("id")->value();

            // Coba untuk parse stringnya menjadi integer.
            int id = isStringDigit(idString) ? idString.toInt() : -1;
            
            // Cari IDnya secara linear.
            bool found = false;
            int index = 0;
            for (auto val : remotes) {
                if (val["id"].as<int>() == id) {
                    found = true;
                    break;
                }
                index++;
            }

            // Jika IDnya ditemukan di dalam array
            if (found) {
                JsonArray irData = remotes[index]["data"];
                // Inisialisasi data.
                int length = irData.size();
                if (length == 0) {
                    strResult += "Tidak ada sinyal di data json.";
                } else {
                    uint16_t rawIrData[length];
                    // Masukkan data ke array.
                    int irIndex = 0;
                    for (auto val : irData) {
                        rawIrData[irIndex] = val.as<int>();
                        irIndex++;
                    }
                    irsend.sendRaw(rawIrData, length, IR_SEND_FREQ);

                    strResult += "Sinyal sukses dikirim!";
                }
            } else {
                strResult += "id tidak ditemukan.";
            }

        } else {
            strResult += "Tidak ada parameter HTTP GET `id` dimasukkan!";
        }

        request->send(200, "text/plain", strResult);
    }

    void addButton(AsyncWebServerRequest* request) {

        // Reset semuanya, dan buat mesin
        // bisa menerima sinyal IR.
        isReceivingIR = true;
        sizeIR = 0;
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", add_button_html);
        request->send(response);
    }

    void doneAddButton(AsyncWebServerRequest* request) {
        if (sizeIR == 0) {
            // Kalau sizenya 0, berarti belum diterima sinyal baru.
            AsyncWebServerResponse *response = request->beginResponse(200, "text/html", F("<a href=\"/\">Sinyal IR tidak valid. Klik untuk kembali.</a>"));
            request->send(response);
        } else {
            AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", add_button_done_html, doneAddButtonProc);
            request->send(response);
        }
    }

    void doneAddButtonPost(AsyncWebServerRequest* request) { 
        // Masukkan ke memori untuk di proses
        DynamicJsonDocument doc(1024);
        File file = LittleFS.open(REMOTE_FILE, "r");
        deserializeJson(doc, file);

        JsonObject json = doc.as<JsonObject>();

        file.close();

        // Masukkan ke JsonArray
        JsonArray remotes = json["remotes"];

        // Buat entri baru di JsonArray
        JsonObject newRemote = remotes.createNestedObject();

        // Masukkan data baru
        newRemote["id"] = remotes[remotes.size() - 1]["id"].as<int>() + 2;
        newRemote["caption"] = request->getParam("caption", true)->value();
        newRemote["logo"] = request->getParam("symbol", true)->value();
        newRemote["color"] = request->getParam("color", true)->value();
        JsonArray data = newRemote.createNestedArray("data");

        for (uint16_t i = 0; i < sizeIR; i++) {
            data.add(rawArrIR[i]);
        }

        file = LittleFS.open(REMOTE_FILE, "w");

        // Setelah data baru sudah ditambah, kita bisa save kembali ke filenya
        serializeJson(doc, file);

        file.flush();
        file.close();

        AsyncWebServerResponse *response = request->beginResponse(200, "text/html", F("<a href=\"/\">Sukses di tambah! Klik untuk kembali.</a>"));
        request->send(response);
    }

    void delButtonPost(AsyncWebServerRequest* request) {

        // Masukkan ke memori untuk di proses
        DynamicJsonDocument doc(1024);
        File file = LittleFS.open(REMOTE_FILE, "r");
        deserializeJson(doc, file);

        JsonObject json = doc.as<JsonObject>();

        file.close();

        // Masukkan ke JsonArray
        JsonArray remotes = json["remotes"];
        // Dapatkan dari POST request ID mana yang mau didelete.
        String idString = request->getParam("del_id", true)->value();

        // Coba untuk parse stringnya menjadi integer.
        int id = isStringDigit(idString) ? idString.toInt() : -1;
        
        // Cari IDnya secara linear.
        bool found = false;
        int index = 0;

        for (auto val : remotes) {
            if (val["id"].as<int>() == id) {
                found = true;
                break;
            }
            index++;
        }

        // Hapus JSONarraynya jika ditemukan.
        if (found) {
            remotes.remove(index);

            file = LittleFS.open(REMOTE_FILE, "w");
            // Setelah data baru sudah dihapus, kita bisa save kembali ke filenya
            serializeJson(doc, file);
            file.flush();
            file.close();

            AsyncWebServerResponse *response = request->beginResponse(200, "text/html", F("<a href=\"/\">Sukses di dihapus! Klik untuk kembali.</a>"));
            request->send(response);
        } else {
            // Jika tidak, umumkan bahwa ID tidak ditemukan.
            AsyncWebServerResponse *response = request->beginResponse(200, "text/html", F("<a href=\"/\">ID tidak ditemukan! Klik untuk kembali.</a>"));
            request->send(response);
        }
    }

};

