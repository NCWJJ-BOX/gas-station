// ============================================================
// Pump Controller - Manages fuel dispensing
// ============================================================

#ifndef PUMP_CONTROLLER_H
#define PUMP_CONTROLLER_H

#include <Arduino.h>
#include "config.h"
#include "LCDManager.h"


class PumpController {
private:
    float tankLevel;
    bool isRunning;

public:
    PumpController() : tankLevel(MAX_TANK_CAPACITY), isRunning(false) {}

    // Initialize pump pin
    void begin() {
        pinMode(SWITCH_PIN, INPUT_PULLUP);
        pinMode(PUMP_PIN, OUTPUT);
        digitalWrite(PUMP_PIN, PIN_OFF);
    }

    // Check if start button is pressed
    bool isButtonPressed() {
        return digitalRead(SWITCH_PIN) == LOW;
    }

    // Start fueling process
    void startFueling(float targetLiters, LCDManager& lcd) {
        if (targetLiters <= 0 || isRunning) {
            return;
        }

        // Calculate fill time: liters * seconds per liter
        unsigned long fillTimeMs = (unsigned long)(targetLiters * SECONDS_PER_LITER * 1000);
        
        Serial.printf("Starting pump: %.2f L, duration: %lu ms\n", 
                      targetLiters, fillTimeMs);

        digitalWrite(PUMP_PIN, PIN_ON);
        isRunning = true;

        // Track filling progress
        unsigned long startTime = millis();
        
        while (millis() - startTime < fillTimeMs) {
            // Calculate current liters dispensed
            float elapsedSeconds = (millis() - startTime) / 1000.0f;
            float dispensedLiters = elapsedSeconds / SECONDS_PER_LITER;
            float remainingLiters = targetLiters - dispensedLiters;
            
            // Ensure non-negative display
            remainingLiters = max(remainingLiters, 0.0f);
            
            // Update LCD with current progress
            lcd.displayFilling(dispensedLiters);
            
            delay(100);
        }

        // Stop pump
        stopFueling();
        
        // Update tank level
        tankLevel -= targetLiters;
        tankLevel = max(tankLevel, 0.0f);
        
        Serial.printf("Fueling complete. Tank level: %.2f L\n", tankLevel);
    }

    // Stop pump immediately
    void stopFueling() {
        digitalWrite(PUMP_PIN, PIN_OFF);
        isRunning = false;
    }

    // Get current tank level
    float getTankLevel() const {
        return tankLevel;
    }

    // Check if pump is currently running
    bool isPumpRunning() const {
        return isRunning;
    }

    // Reset tank to full (for testing)
    void resetTank() {
        tankLevel = MAX_TANK_CAPACITY;
    }
};

#endif // PUMP_CONTROLLER_H
