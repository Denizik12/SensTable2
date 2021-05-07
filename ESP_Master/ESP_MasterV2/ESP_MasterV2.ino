#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketClient.h>

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

// data to server
boolean handshakeFailed = 0;
String data[3] = {};
char path[] = "/";   //identifier of this device
const char* ssid     = "Z-ArcherC1200 2.4";
const char* password = "meekeren69420";
char* host = "145.24.222.125";  //replace this ip address with the ip address of your Node.Js server
const int espport = 8080;

WebSocketClient webSocketClient;
unsigned long previousMillis = 0;
unsigned long currentMillis;
unsigned long interval = 300; //interval for sending data to the websocket server in ms
// Use WiFiClient class to create TCP connections
WiFiClient client;




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

  //  send data to server
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);

  wsconnect();
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
  //  Serial.print(sensor_type); Serial.print("   "); Serial.println(sensor_value);
  server.send(200, "text/plain", "Updated");

  // sends data to server
  if (client.connected()) {
      String json = F("{ \"sensor\":{ \"id\": ");
      String jsonMiddle = ", \"value\": ";
      String jsonEnd = "} }";


      data[0] = sensor_value; //read adc values, this will give random value, since no sensor is connected.


      String finalJson = json + sensor_type + jsonMiddle + data[0] + jsonEnd;
      Serial.println(finalJson);
      //For this project we are pretending that these random values are sensor values
      webSocketClient.sendData(finalJson);//send sensor data to websocket server
    }
  }



//***************function definitions**********************************************************************************
void wsconnect() {
  // Connect to the websocket server
  if (client.connect(host, espport)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
    delay(1000);

    if (handshakeFailed) {
      handshakeFailed = 0;
      ESP.restart();
    }
    handshakeFailed = 1;
  }
  // Handshake with the server
  webSocketClient.path = path;
  webSocketClient.host = host;
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {

    Serial.println("Handshake failed.");
    delay(4000);

    if (handshakeFailed) {
      handshakeFailed = 0;
      ESP.restart();
    }
    handshakeFailed = 1;
  }
}
