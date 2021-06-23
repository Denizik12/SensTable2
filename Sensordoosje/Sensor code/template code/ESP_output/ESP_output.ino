#include <ESP8266WiFi.h>
#include <WebSocketClient.h>

// data to server
boolean handshakeFailed = 0;
String data[3] = {};
char path[] = "/";   //identifier of this device
const char* ssid     = "Ziwe";
const char* password = "testtest";
char* host = "145.24.222.125";  //replace this ip address with the ip address of your Node.Js server
const int espport = 8789;

WebSocketClient webSocketClient;
// Use WiFiClient class to create TCP connections
WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.println();

  //  send data to server
  // We start by connecting to a WiFi network
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
  String data;

  webSocketClient.getData(data);
  if (data.length() > 0) {
    Serial.print("Received data: ");
    Serial.println(data);
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
  
  // send outputId
    String json = F("{ \"client\":{ \"id\": 3} }");
    webSocketClient.sendData(json);
}
