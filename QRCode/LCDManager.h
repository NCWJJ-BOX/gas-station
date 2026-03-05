// ============================================================
// LCD Manager - Handles all 5 LCD displays
// ============================================================

#ifndef LCD_MANAGER_H
#define LCD_MANAGER_H

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "config.h"


class LCDManager {
private:
    LiquidCrystal_I2C lcd1, lcd2, lcd3, lcd4, lcd5;

public:
    // Constructor - Initialize all 5 LCDs with their addresses
    LCDManager()
        : lcd1(LCD1_ADDR, LCD_COLS, LCD_ROWS)
        , lcd2(LCD2_ADDR, LCD_COLS, LCD_ROWS)
        , lcd3(LCD3_ADDR, LCD_COLS, LCD_ROWS)
        , lcd4(LCD4_ADDR, LCD_COLS, LCD_ROWS)
        , lcd5(LCD5_ADDR, LCD_COLS, LCD_ROWS)
    {}

    // Initialize all LCDs
    void begin() {
        lcd1.begin();
        lcd2.begin();
        lcd3.begin();
        lcd4.begin();
        lcd5.begin();
    }

    // Display current time and date on LCD1
    void displayTimeDate(time_t epochTime) {
        struct tm *timeInfo = localtime(&epochTime);
        
        lcd1.clear();
        lcd1.setCursor(0, 0);
        lcd1.print("Time: ");
        lcd1.print(timeInfo->tm_hour < 10 ? "0" : "");
        lcd1.print(timeInfo->tm_hour);
        lcd1.print(":");
        lcd1.print(timeInfo->tm_min < 10 ? "0" : "");
        lcd1.print(timeInfo->tm_min);
        lcd1.print(":");
        lcd1.print(timeInfo->tm_sec < 10 ? "0" : "");
        lcd1.print(timeInfo->tm_sec);
        
        lcd1.setCursor(0, 1);
        lcd1.print("Date: ");
        lcd1.print(timeInfo->tm_mday);
        lcd1.print("/");
        lcd1.print(timeInfo->tm_mon + 1);
        lcd1.print("/");
        lcd1.print(timeInfo->tm_year + 1900);
    }

    // Display fuel info on LCD2
    void displayFuelInfo(const String& fuelName, const String& unit) {
        lcd2.clear();
        lcd2.setCursor(0, 0);
        lcd2.print("Fuel: ");
        lcd2.print(fuelName);
        
        lcd2.setCursor(0, 1);
        lcd2.print("Unit: ");
        lcd2.print(unit);
    }

    // Display liters and amount on LCD3
    void displayOrder(float liters, float amount) {
        lcd3.clear();
        lcd3.setCursor(0, 0);
        lcd3.print("Liters: ");
        lcd3.print(liters);
        
        lcd3.setCursor(0, 1);
        lcd3.print("Amount: ");
        lcd3.print(amount);
    }

    // Display current fill progress on LCD4
    void displayFilling(float currentLiters) {
        lcd4.clear();
        lcd4.setCursor(0, 0);
        lcd4.print("Filled: ");
        lcd4.setCursor(8, 0);
        lcd4.print(currentLiters, 2);
        lcd4.print(" L     ");
    }

    // Display tank level on LCD5
    void displayTankLevel(float liters) {
        lcd5.clear();
        lcd5.setCursor(3, 0);
        lcd5.print("Oil in Tank");
        lcd5.setCursor(5, 1);
        lcd5.print(liters);
        lcd5.print(" L");
    }
};

#endif // LCD_MANAGER_H
