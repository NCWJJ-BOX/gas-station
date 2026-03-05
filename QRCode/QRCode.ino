// ============================================================
// Gas Station Simulator - Main Program
// Uses modular architecture with separate concerns
// ============================================================

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>

#include "config.h"
#include "LCDManager.h"
#include "DataFetcher.h"
#include "PumpController.h"


// Global Objects
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, TIMEZONE_OFFSET * 3600);

LCDManager lcd;
DataFetcher fetcher;
PumpController pump;


// ------------------------------------------------------------
// Setup
// ------------------------------------------------------------
void setup() {
    Serial.begin(115200);
    Serial.println("\n=== Gas Station Simulator ===\n");

    // Initialize LCD displays
    lcd.begin();
    Serial.println("LCDs initialized");

    // Initialize pump controller
    pump.begin();
    Serial.println("Pump initialized");

    // Connect to WiFi
    if (!fetcher.connectWiFi()) {
        Serial.println("WiFi connection failed!");
        while (true) {
            delay(1000);  // Halt if no WiFi
        }
    }

    // Start NTP client
    timeClient.begin();
    Serial.println("NTP client started");

    Serial.println("\n=== System Ready ===\n");
}


// ------------------------------------------------------------
// Main Loop
// ------------------------------------------------------------
void loop() {
    // Update time from NTP server
    timeClient.update();
    time_t currentTime = timeClient.getEpochTime();

    // Fetch latest fuel order from server (if not currently running)
    if (!pump.isPumpRunning()) {
        fetcher.fetch();
    }

    // Update all LCD displays
    lcd.displayTimeDate(currentTime);
    lcd.displayFuelInfo(fetcher.data.fuelName, fetcher.data.unit);
    lcd.displayOrder(fetcher.data.liters, fetcher.data.amount);
    lcd.displayFilling(0);  // Clear or show idle
    lcd.displayTankLevel(pump.getTankLevel());

    // Check for start button press
    if (fetcher.data.isValid() && pump.isButtonPressed() && !pump.isPumpRunning()) {
        Serial.println("Start button pressed - Beginning fueling...");
        
        pump.startFueling(fetcher.data.liters, lcd);
        
        // Clear data after fueling complete
        fetcher.data.liters = 0;
        fetcher.data.amount = 0;
    }

    delay(2000);
}
