// ============================================================
// Configuration - WiFi, Pins, and System Constants
// ============================================================

#ifndef CONFIG_H
#define CONFIG_H

// WiFi Credentials
constexpr const char* WIFI_SSID     = "NSSS";
constexpr const char* WIFI_PASSWORD = "Numchai1.";

// Server Configuration
constexpr const char* SERVER_URL    = "https://webpoo.ncwjjdx.work/data";
constexpr const uint16_t TIMEOUT_MS = 10000;

// NTP Server
constexpr const char* NTP_SERVER    = "pool.ntp.org";
constexpr const int TIMEZONE_OFFSET = 7;  // UTC+7 (Thailand)

// Hardware Pins
constexpr int SWITCH_PIN = D5;   // Button to start fueling
constexpr int PUMP_PIN   = D6;  // Pump relay control

// Pin States
constexpr int PIN_ON  = HIGH;
constexpr int PIN_OFF = LOW;

// Tank Configuration
constexpr float MAX_TANK_CAPACITY = 200.0f;  // liters

// Pump Timing (seconds per liter)
// 28 seconds per liter = ~2.14 L/min typical flow rate
constexpr float SECONDS_PER_LITER = 28.0f;

// LCD I2C Addresses
constexpr uint8_t LCD1_ADDR = 0x27;
constexpr uint8_t LCD2_ADDR = 0x26;
constexpr uint8_t LCD3_ADDR = 0x25;
constexpr uint8_t LCD4_ADDR = 0x24;
constexpr uint8_t LCD5_ADDR = 0x23;

constexpr uint8_t LCD_COLS = 16;
constexpr uint8_t LCD_ROWS = 2;

// WiFi Connection Timeout (milliseconds)
constexpr uint32_t WIFI_TIMEOUT_MS = 30000UL;

#endif // CONFIG_H
