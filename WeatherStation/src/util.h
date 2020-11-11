#include <LittleFS.h>
#include <Arduino.h>

// Utility buat membaca file
namespace Util {
    String readFileAsString(String filename) {
        File file = LittleFS.open(filename, "r");

        String result = "";
        int num = file.read();
        while (num != -1) {
            result += char(num);
            num = file.read();
        }
        file.close();

        return result;
    }
}