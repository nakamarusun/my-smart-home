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
    
    // Sediakan array panjang IR_ARRAY_SIZE.
    uint16_t rawIRArr[IR_ARRAY_SIZE];
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
            "cp": "bruhbruh",
            "lg": "+",
            "cl": "#FFFFFF"
        }, ...]
        }
        */

        // Initial allocation
        DynamicJsonDocument doc(JSON_BUFFER);
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
                + val["cl"].as<String>()
                + ";\">"
                + val["lg"].as<String>()
                + "<div class=\"cp\">"
                + val["id"].as<String>() + ". " + val["cp"].as<String>()
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
                result += rawIRArr[i];
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

    void remoteFile (AsyncWebServerRequest* request) {
        // Akan menyediakan file JSON daftar remot jika tidak ada parameter GET.
        // Jika ada, sediakan file tombol remotnya.
        if (request->hasParam("id")) {

            request->send(LittleFS, "remote_" + request->getParam("id")->value(), "text/plain");
        } else {

            request->send(LittleFS, REMOTE_FILE, "application/json");
        }
    }

    void index(AsyncWebServerRequest* request) {

        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html, indexProc);
        request->send(response);
    }

    void notFound(AsyncWebServerRequest* request) {
        request->send(404, "text/plain", "no bueno lol");
    }

    void getTempIr(AsyncWebServerRequest* request) {
        String result = "";
        result += sizeIR;
        result += "\n\n";
        for (uint16_t i = 0; i < sizeIR; i++) {
            result += rawIRArr[i];
            result += "\n";
        }
        AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", result);
        request->send(response);
    }

    void clickRemote(AsyncWebServerRequest* request) {

        String strResult = "";

        // Ini adalah responder untuk request html GET jika user mengeklik sesuatu.
        if (request->hasParam("id")) {

            // Masukkan ke memory.
            DynamicJsonDocument doc(JSON_BUFFER);
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
                // Bakal baca dari file menuju ke array.
                file = LittleFS.open("remote_" + String(id), "r");
                uint16_t loadedIR[IR_ARRAY_SIZE];
                index = 0;
                String valBuf = "";
                while (true) {
                    int ch = file.read();
                    if (ch == -1) {
                        // Read until EOF
                        break;
                    } else if (ch == 10) {
                        // If newline is detected
                        loadedIR[index] = valBuf.toInt();
                        index++;
                        valBuf = "";
                    } else {
                        // Add string number to buffer
                        valBuf += char(ch);
                    }
                }
                // for (int i = 0; i < index; i++) Serial.println(loadedIR[i]);
                // Kirim array yang sudah dibuaat.
                if (index != 0) {
                    Serial.println("bruhhy?");
                    Serial.println(index);
                    irsend.sendRaw(loadedIR, index, IR_SEND_FREQ);
                    strResult += "Sinyal IR sukses dikirimkan!";
                } else {
                    strResult += "File IR kosong.";
                }
                file.close();
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
        sizeIR = 0;
        isReceivingIR = true;
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
        isReceivingIR = false;
    }

    void doneAddButtonPost(AsyncWebServerRequest* request) { 
        // Masukkan ke memori untuk di proses
        DynamicJsonDocument doc(JSON_BUFFER);
        File file = LittleFS.open(REMOTE_FILE, "r");
        deserializeJson(doc, file);

        JsonObject json = doc.as<JsonObject>();

        file.close();

        // Masukkan ke JsonArray
        JsonArray remotes = json["remotes"];

        // Dapatkan ID yang paling baru.
        int id;
        if (remotes.size() == 0) {
            id = 1;
        } else {
            id = remotes[remotes.size() - 1].as<JsonObject>()["id"].as<int>() + 1;
        }

        // Buat entri baru di JsonArray
        JsonObject newRemote = remotes.createNestedObject();

        // Masukkan data baru
        newRemote["id"] = String(id);
        newRemote["cp"] = request->getParam("caption", true)->value();
        newRemote["lg"] = request->getParam("symbol", true)->value();
        newRemote["cl"] = request->getParam("color", true)->value();

        file = LittleFS.open("remote_" + String(id), "w");

        for (uint16_t i = 0; i < sizeIR; i++) {
            file.print(rawIRArr[i]);
            file.print("\n");
        }
        file.close();

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
        DynamicJsonDocument doc(JSON_BUFFER);
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

            LittleFS.remove("remote_" + String(id));

            AsyncWebServerResponse *response = request->beginResponse(200, "text/html", F("<a href=\"/\">Sukses di dihapus! Klik untuk kembali.</a>"));
            request->send(response);
        } else {
            // Jika tidak, umumkan bahwa ID tidak ditemukan.
            AsyncWebServerResponse *response = request->beginResponse(200, "text/html", F("<a href=\"/\">ID tidak ditemukan! Klik untuk kembali.</a>"));
            request->send(response);
        }
    }

};