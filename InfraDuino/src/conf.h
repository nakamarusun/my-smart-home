/*
Semua yang ada di file ini dapat diganti
ke kemauan hatimu~
*/

#define RECV_PIN 0 // Pin receive IR
#define LED_PIN 2 // Pin send IR

#define IR_SEND_FREQ 38000 // IR send frequency (default: 38KHz)

#define AP_TIMEOUT 60 // Timeout dalam detik membuka mode AP

#define CONNECT_TIMEOUT 30 //  Timeout dalam detik untuk connect ke Wi-Fi

// Detail AP
#define AP_SSID "IR_STATION"
#define AP_PASS "password"

#define MDNS_NAME "wifi_ir" // Nama mDNS yang bisa diakses dengan mudah

#define REMOTE_FILE "/remotes.json" // Nama file remote yang tersimpan di ESP.