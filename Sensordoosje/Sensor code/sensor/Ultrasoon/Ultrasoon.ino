#include <ESP8266WiFi.h>

// Netwerk configuratie
const char* ssid = "SenstableNetwork";
const char* password = "Senstable2";
const char* host = "192.168.4.1";

// Client object
WiFiClient client;

// Tijd tussen metingen
const int sleepTimeMiliSeconds = 300;

String sensorId = "3";
String sensorType = "Ultrasoon";
float sensorValue = 0;
String physicalQuantity = "Distance";
String unit = "cm";

#define echoPin 16    //D0
#define trigPin 5     //D1
#define greenLED 4    //D2
#define redLED 0      //D3

void readSensor() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  sensorValue = (pulseIn(echoPin, HIGH)) * 0.0343 / 2;

  // set limit on 30cm
  if (sensorValue > 30) {
    sensorValue = 0;
  }

  Serial.println(sensorValue);
}

void setupSensor() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  setupSensor();
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);

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
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
    readSensor();
    String url = "update.php?sensorId=";
    url += String(sensorId);
    url += "&sensorType=";
    url += String(sensorType);
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
    ESP.restart();
  }
  delay(sleepTimeMiliSeconds);
}
