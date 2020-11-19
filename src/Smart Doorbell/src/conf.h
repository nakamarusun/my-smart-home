/*
Semua yang ada di file ini dapat diganti
ke kemauan hatimu~
*/

// Menyalakan mode serial.
// #define DEBUG_MODE

// Detail AP
#define AP_SSID "SMART_DOORBELL"
#define AP_PASS "password"

#define MDNS_NAME "smartdoor" // Nama mDNS yang bisa diakses dengan mudah

#define AP_TIMEOUT 60 // Timeout dalam detik membuka mode AP

#define CONNECT_TIMEOUT 30 // Timeout dalam detik untuk connect ke Wi-Fi

// Detail pins
#define SDA_PIN 3
#define SCL_PIN 1

#define PROX_PIN 16
#define BTN_PIN 15

// Pub/Sub names
#define BTN_TOPIC "doorbell/button"
#define PROX_TOPIC "doorbell/prox"