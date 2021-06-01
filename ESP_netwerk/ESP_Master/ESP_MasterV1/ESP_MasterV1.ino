#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Netwerk Configuratie
char * ssid_ap = "SenseTableAP";
char * password_ap = "test1234"; //minimaal 8 characters

// Server setup
ESP8266WebServer server(80);
IPAddress ip(192, 168, 11, 9); // arbitrary IP address (doesn't conflict w/ local network)
IPAddress gateway(192, 168, 11, 1);
IPAddress subnet(255, 255, 255, 0);

String sensor_type = "";
float sensor_value = 0.0;

void setup() {
  Serial.begin(115200);
  Serial.println();

  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(ssid_ap, password_ap, 1, false, 8);
  Serial.print("Setting AP (Access Point)…");

  IPAddress IP = WiFi.softAPIP();
  Serial.println();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", handleRoot);
  server.on("/update", handleUpdate);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/plain", "Sensor: " + String(sensor_type) + " Value: " + String(sensor_value));
}

void handleUpdate() {
  // The value will be passed as a URL argument
  sensor_type = server.arg("type");
  sensor_value = server.arg("value").toFloat();
  Serial.print(sensor_type); Serial.print("   "); Serial.println(sensor_value);
  server.send(200, "text/plain", "Updated");
}
