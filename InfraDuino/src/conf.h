/*
Semua yang ada di file ini dapat diganti
ke kemauan hatimu~
*/

// Bahaya untuk diganti
// Rujuk kesini https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
// #define ENABLE_SERIAL

#define RECV_PIN 2 // Pin receive IR
#define LED_PIN 3 // Pin send IR

#define MARKER_LED_PIN 0 // pin LED yang akan nyala jika alat bisa menerima sesuatu.

#define IR_SEND_FREQ 38000 // IR send frequency (default: 38KHz)
#define IR_ARRAY_SIZE 1024 // The size of the IR array buffer.

#define AP_TIMEOUT 60 // Timeout dalam detik membuka mode AP

#define CONNECT_TIMEOUT 30 // Timeout dalam detik untuk connect ke Wi-Fi

#define JSON_BUFFER 2048 // Buffer for the loaded json file.

// Detail AP
#define AP_SSID "IR_STATION"
#define AP_PASS "password"

#define MDNS_NAME "wifi_ir" // Nama mDNS yang bisa diakses dengan mudah

#define REMOTE_FILE "/remotes.json" // Nama file remote yang tersimpan di ESP.