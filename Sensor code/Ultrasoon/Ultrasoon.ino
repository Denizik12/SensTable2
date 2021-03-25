#include <ESP8266WiFi.h>

// Netwerk configuratie
const char* ssid = "ESP8266_SSID";
const char* password = "test1234";
const char* host = "192.168.11.4";

// Client object
WiFiClient client;

// Tijd tussen metingen
const int sleepTimeSeconds = 2;

String sensor_type = "Ultrasoon";
float sensor_value = 0;

#define trigPin 16 //D0
#define echoPin 5  //D1

void readSensor() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  sensor_value = (pulseIn(echoPin, HIGH)) * 0.0343 / 2;
}

void setupSensor() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void setup() {
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  setupSensor();

  // wacht totdat er een WiFi connectie is
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("IP Address (AP): "); Serial.println(WiFi.localIP());
}

void loop() {

  if (client.connect(host, 80)) {
    String url = "/update?type=";
    url += String(sensor_type);
    url += "&value=";
    url += String(sensor_value);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host +  "\r\n" +
                 "Connection: keep-alive\r\n\r\n"); // minimum set of required URL headers
    delay(10);
    // Read all the lines of the response and print them to Serial
    // Alleen gebruiken om connectie te testen
    /*    Serial.println("Response: ");
        while(client.available()){
          String line = client.readStringUntil('\r');
          Serial.print(line);
        }
        Serial.println();  */
  }
  delay(2000);
}
