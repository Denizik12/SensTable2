#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Netwerk configuratie
const char* ssid = "Z-ArcherC1200 2.4";
const char* password = "meekeren69420";
const char* host = "145.24.222.125";

// Client object
WiFiClient client;

// Tijd tussen metingen
const int sleepTimeMiliSeconds = 500;

String output_type = "1";
float sensor_value = 0;

#define output_pin 5  //D0

void outputSensor() {
  

  Serial.println(sensor_value);
}

void setupOutput() {
  pinMode(output_pin, OUTPUT);
}

void setup() {
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  setupOutput();

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
     if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http; //Declare object of class HTTPClient

    String url = "http://145.24.222.125";
    url += "/getSensorOutput.php";

    http.begin(url); //Specify request destination

    int httpCode = http.GET();   //Send the request
    String payload = http.getString();                  //Get the response payload
    
    Serial.println(payload);
    http.end();  //Close connection
     } else {
    Serial.println("Error in WiFi connection");
  }
  }
  delay(sleepTimeMiliSeconds);
}
