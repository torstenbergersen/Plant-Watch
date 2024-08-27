#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "esp_sleep.h"

const char *ssid = "**";
const char *password = "**";
const char *serverName = "**";
const int moistureSensorPin = 36;

// const char *apiKey =

const int sleepCycleMinutes = 30; // Sleep for 30 minutes per cycle
const int sleepCycles = 2;        // Number of cycles to accumulate to 60 minutes

void setup()
{
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");
    delay(5000); // give the sensor some time to stabilize

    if (WiFi.status() == WL_CONNECTED)
    {
        // get sensor value
        int sensorValue = analogRead(moistureSensorPin);

        // create the JSON object with the sensor value
        DynamicJsonDocument doc(64);
        doc["plant"] = "Spathiphyllum";
        doc["moisture"] = sensorValue;

        String jsonString;
        serializeJson(doc, jsonString);

        // send the data to the server
        HTTPClient http;
        http.begin(serverName);
        http.addHeader("Content-Type", "application/json");
        http.addHeader("x-api-key", apiKey);
        int httpResponseCode = http.POST(jsonString);

        if (httpResponseCode > 0)
        {
            Serial.printf("HTTP Response code: %d\n", httpResponseCode);
        }
        else
        {
            Serial.printf("Error code: %d\n", httpResponseCode);
        }

        // Print the response for debugging
        String response = http.getString();
        Serial.println("Response: " + response);

        http.end();
    }
    else
    {
        Serial.println("WiFi Disconnected");
    }

    for (int i = 0; i < sleepCycles; i++)
    {
        esp_sleep_enable_timer_wakeup(sleepCycleMinutes * 60 * 1000000ULL);
        esp_deep_sleep_start();
    }

    // put the ESP32 into deep sleep for 60 minutes
    // esp_sleep_enable_timer_wakeup(60 * 60 * 1000000);
    // esp_deep_sleep_start();
}

void loop()
{
    // Intentionally left empty
}
