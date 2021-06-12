#include <ESP8266WiFi.h>

// Netwerk configuratie
const char* ssid = "SenstableNetwork";
const char* password = "Senstable2";
const char* host = "192.168.4.1";

// Client object
WiFiClient client;

// Tijd tussen metingen
const int sleepTimeMiliSeconds = 300;

String sensorId = "8";
float sensorValue = 0;
String physicalQuantity = "Reed";
String unit = "";

#define sensorPin 16  //D0

void readSensor() {
  sensorValue = (float)!digitalRead(sensorPin);

  Serial.println(sensorValue);
}

void setupSensor() {
  pinMode(sensorPin, INPUT);
}

void setup() {
  WiFi.mode(WIFI_STA);
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
    readSensor();
    String url = "update.php?sensorId=";
    url += String(sensorId);
    url += "&sensorValue=";
    url += String(sensorValue);
    url += "&physicalQuantity=";
    url += String(physicalQuantity);
    url += "&unit=";
    url += String(unit);
    client.println(String("GET /") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n"); // minimum set of required URL headers
    client.flush();
    client.stop();
  } else {
    WiFi.disconnect();
    setup();
  }
  delay(sleepTimeMiliSeconds);
}
