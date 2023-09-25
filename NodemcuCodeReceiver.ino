// UPLOAD THIS CODE AFER UPLOADING ARDUINO CODE THEN CONNECT TX AND RX
// THIS CODE UPLOADS THE DATA TO FIREBASE ITS GETTING FROM ARDUINO UNO'S SENSORS
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <WiFiManager.h>

// Firebase credentials 
#define FIREBASE_HOST "psmwaterquality-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "7vOcbfwnov2M0IolVwcHpHiqegUzbcOCEppiS3pz"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(9600);

  // Initialize Wi-FiManager
  WiFiManager wifiManager;
  WiFi.disconnect(true);
  

  // Connect to Wi-Fi or configure credentials
  if (!wifiManager.autoConnect("Wi-Fi Manager", "wifi_manager")) {
    Serial.println("Failed to connect or configure. Restarting...");
    ESP.restart();
  }

  // Print connection status
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected: " + WiFi.SSID());
    
  } else {
    Serial.println("Not Connected");
  }

  // Configure Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth);
}

void loop() {
  while (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    if (data.startsWith("pH: ")) {
      float pHValue = data.substring(4).toFloat();
      // Send pH value to Firebase
      if (Firebase.RTDB.setFloat(&fbdo, "/sensors/pH", pHValue)) {
        Serial.println("pH Data sent");
      } else {
        Serial.println("Error sending pH data: " + fbdo.errorReason());
      }
    } else if (data.startsWith("Turbidity: ")) {
      int turbidity = data.substring(11).toInt();
      // Send turbidity value to Firebase
      if (Firebase.RTDB.setInt(&fbdo, "/sensors/turbidity", turbidity)) {
        Serial.println("Turbidity Data sent");
      } else {
        Serial.println("Error sending turbidity data: " + fbdo.errorReason());
      }
    }
  }

  // Handle authentication requests if needed
  if (Firebase.ready()) {
    // Handle authentication requests here if necessary
  }

  delay(1000);
}
