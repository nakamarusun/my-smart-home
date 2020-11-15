#pragma once

#include <SPIFFS.h>

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