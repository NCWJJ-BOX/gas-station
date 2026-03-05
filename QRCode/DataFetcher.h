// ============================================================
// Data Fetcher - Handles HTTP requests to server
// ============================================================

#ifndef DATA_FETCHER_H
#define DATA_FETCHER_H

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"


struct FuelData {
    String fuelName;
    String unit;
    float liters;
    float amount;
    
    FuelData() : fuelName(""), unit(""), liters(0), amount(0) {}
    
    bool isValid() const {
        return liters > 0;
    }
};


class DataFetcher {
private:
    WiFiClientSecure secureClient;
    HTTPClient http;

public:
    FuelData data;

    // Initialize WiFi connection
    bool connectWiFi() {
        Serial.println("Connecting to WiFi...");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        
        unsigned long startTime = millis();
        while (WiFi.status() != WL_CONNECTED && 
               millis() - startTime < WIFI_TIMEOUT_MS) {
            delay(1000);
            Serial.print(".");
        }
        
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nWiFi Connected!");
            Serial.print("IP: ");
            Serial.println(WiFi.localIP());
            return true;
        }
        
        Serial.println("\nFailed to connect to WiFi");
        return false;
    }

    // Fetch fuel data from server
    bool fetch() {
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("WiFi not connected");
            return false;
        }

        secureClient.setInsecure();
        http.begin(secureClient, SERVER_URL);
        http.addHeader("Content-Type", "application/json");

        int httpCode = http.GET();

        if (httpCode > 0) {
            String response = http.getString();
            Serial.printf("HTTP %d: %s\n", httpCode, response.c_str());
            
            return parseResponse(response);
        } else {
            Serial.printf("HTTP Error: %d\n", httpCode);
            http.end();
            return false;
        }
    }

private:
    bool parseResponse(const String& response) {
        DynamicJsonDocument doc(1024);
        auto error = deserializeJson(doc, response);

        if (error) {
            Serial.printf("JSON Error: %s\n", error.c_str());
            return false;
        }

        data.fuelName = doc["fuel_name"].isNull() ? "" : doc["fuel_name"].as<String>();
        data.unit     = doc["unit"].isNull() ? "" : doc["unit"].as<String>();
        data.liters   = doc["liters"].isNull() ? 0.0f : doc["liters"].as<float>();
        data.amount   = doc["amount"].isNull() ? 0.0f : doc["amount"].as<float>();

        Serial.printf("Fuel: %s, Unit: %s, Liters: %.2f, Amount: %.2f\n",
                      data.fuelName.c_str(), data.unit.c_str(), 
                      data.liters, data.amount);

        return true;
    }
};

#endif // DATA_FETCHER_H
