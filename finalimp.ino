#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "POCO X6 Pro";
const char* password = "123456789";

const char* spreadsheetId = "711948482270-tu73h09p8vbaro6k7crkg0sb0tg5usop.apps.googleusercontent.com";
const char* scriptId = "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";

const char* googleScriptUrl = "https://docs.google.com/spreadsheets/d/1hTPmr3VgI-2UNHdE3wWmZpNP09pLxY2IMZl8mIQx44A/edit#gid=0";
const char* range = "Sheet1!A1";

long duration;
int distance;

void setup() {
  Serial.begin(115200);
  delay(4000); // Delay to allow the serial monitor to open

  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize ultrasonic sensor
  pinMode(23, OUTPUT); // Trigger pin
  pinMode(22, INPUT);  // Echo pin
}

void loop() {
  // Trigger ultrasonic sensor
  digitalWrite(23, LOW);
  delayMicroseconds(2);
  digitalWrite(23, HIGH);
  delayMicroseconds(10);
  digitalWrite(23, LOW);

  // Measure echo duration
  duration = pulseIn(22, HIGH);

  // Calculate distance in centimeters
  distance = duration * 0.034 / 2;

  // Send data to Google Sheets
  sendDataToGoogleSheets();
  
  delay(5000); // Delay between readings
}

void sendDataToGoogleSheets() {
  // Create JSON payload
  DynamicJsonDocument json(1024);
  json["value1"] = distance;

  // Serialize JSON to string
  String payload;
  serializeJson(json, payload);

  // Create HTTP client
  HTTPClient http;

  // Set Google Sheets API endpoint
  http.begin(googleScriptUrl);

  // Set headers
  http.addHeader("Content-Type", "application/json");

  // Send POST request
  int httpResponseCode = http.POST(payload);

  // Check for successful response
  if (httpResponseCode == 200) {
    Serial.println("Data sent to Google Sheets successfully");
  } else {
    Serial.print("Error sending data to Google Sheets. HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Close connection
  http.end();
}
