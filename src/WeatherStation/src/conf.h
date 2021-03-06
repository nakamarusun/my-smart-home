
// Semua disini bebas di ubah user.
#define UPDATE_EVERY 60 // Dalam detik (Nilai default)
#define CONFIG_AVAILABLE_FROM 180 // Konfigurasi tersedia di awal mesin untuk 5 menit.
#define TOPIC_NAME "esp_weather"

// Detail AP
#define AP_SSID "WEATHER_STATION"
#define AP_PASS "password"

#define MDNS_NAME "w_station" // Nama mDNS yang bisa diakses dengan mudah

#define AP_TIMEOUT 60 // Timeout dalam detik membuka mode AP

#define CONNECT_TIMEOUT 30 // Timeout dalam detik untuk connect ke Wi-Fi

// Pins
#define DHT_PIN 1
#define WET_PIN 3

// #define SERIAL_DEBUG // Hanya untuk debug.