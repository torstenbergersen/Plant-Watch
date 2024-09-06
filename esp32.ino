#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "esp_sleep.h"

const char *ssid = "**";
const char *password = "**";
const char *serverName = "**";
const int moistureSensorPin = 36;
const int sensorPowerPin = 15; // GPIO pin to control sensor power

const char *apiKey = "**";

void setup()
{
    Serial.begin(115200);

    // Initialize sensor power control pin
    pinMode(sensorPowerPin, OUTPUT);
    digitalWrite(sensorPowerPin, LOW); // Start with sensor off

    // Ensure no other wakeup sources are set
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);

    // connect to wifi
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    delay(500); // give the sensor some time to stabilize
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        // Power on the sensor
        digitalWrite(sensorPowerPin, HIGH);
        delay(500); // Wait for sensor to stabilize

        // get sensor value
        int sensorValue = analogRead(moistureSensorPin);
        Serial.printf("Moisture Level: %d\n", sensorValue);

        // Turn off the sensor after reading
        digitalWrite(sensorPowerPin, LOW);

        // create the JSON object with the average sensor value
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

        http.end();
    }
    else
    {
        Serial.println("WiFi Disconnected");
    }
    // Disconnect WiFi and prepare for deep sleep
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    esp_sleep_enable_timer_wakeup(60 * 60 * 1000000ULL); // Sleep for 1 hour
    esp_deep_sleep_start();
}
