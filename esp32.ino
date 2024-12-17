#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid = "**";
const char *password = "**";
const char *serverName = "https://linux.plant-watch.com/data";
const int moistureSensorPin = 36;

const char *apiKey = "**";

// Timer variables
unsigned long previousMillis = 0;                  // Stores the last time the action was performed
const unsigned long interval = 6 * 60 * 60 * 1000; // 6 hours in milliseconds

void setup()
{
    Serial.begin(115200);

    // connect to wifi
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
}

void loop()
{
    unsigned long currentMillis = millis();

    // Check if 6 hours have passed
    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis; // Update the last run time

        if (WiFi.status() == WL_CONNECTED)
        {
            // Read the sensor value
            int sensorValue = analogRead(moistureSensorPin);
            Serial.printf("Moisture Level: %d\n", sensorValue);

            // Create the JSON object with the sensor value
            DynamicJsonDocument doc(64);
            doc["plant"] = "Calethea";
            doc["moisture"] = sensorValue;

            String jsonString;
            serializeJson(doc, jsonString);

            // Send the data to the server
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
            Serial.println("WiFi Disconnected. Reconnecting...");

            // Attempt to reconnect WiFi
            WiFi.disconnect();
            WiFi.begin(ssid, password);

            while (WiFi.status() != WL_CONNECTED)
            {
                delay(1000);
                Serial.println("Reconnecting to WiFi...");
            }
            Serial.println("Reconnected to WiFi");
        }
    }

    // Small delay to prevent busy looping
    delay(100);
}
