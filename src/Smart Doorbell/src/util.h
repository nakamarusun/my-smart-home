#pragma once
#include <Arduino.h>
#include <SPIFFS.h>
#include <PubSubClient.h>
#include <WiFi.h>

namespace Util {
    String readFileAsString(String filename) {
        File file = SPIFFS.open(filename, "r");

        String result = "";
        int num = file.read();
        while (num != -1) {
            result += char(num);
            num = file.read();
        }
        file.close();

        return result;
    }

    void createIfNotExist(String filename) {
          if (!SPIFFS.exists(filename)) {
            File file = SPIFFS.open(filename, "w");
            file.flush();
            file.close();
        }
    }
}

struct Btn {
    bool curState;
    bool lastState;
    int pin;
    Btn(int pin): curState(false), lastState(false), pin(pin) {}

    // Harus ada di setiap loop.
    void updateButton() {
        lastState = curState;
        curState = digitalRead(pin);
    }

    // Cek tombolnya jika ditekan HANYA SEKALI.
    bool checkButtonOnce() {
        return (curState == true && lastState == false);
    }

    // Cek tombolnya jika ditekan.
    bool checkButton() {
        return curState;
    }
    
};