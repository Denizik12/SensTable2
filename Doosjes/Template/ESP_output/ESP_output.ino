#include <ESP8266WiFi.h>
#include <WebSocketClient.h>
#include <ArduinoJson.h>

// data to server
boolean handshakeFailed = 0;
String data[3] = {};
char path[] = "/";   //identifier of this device
const char* ssid     = "SenstableNetwork";
const char* password = "Senstable2";
char* host = "192.168.4.1";  //replace this ip address with the ip address of your Node.Js server
const int espport = 5050;

String outputId = "OutputId";
String outputType = "OutputType";

#define outputPin 16  //D0
#define greenLED 4    //D2
#define redLED 0      //D3

WebSocketClient webSocketClient;
// Use WiFiClient class to create TCP connections
WiFiClient client;

void setupOutput() {
  pinMode(outputPin, OUTPUT);
}

void updateOutput(int sensorValue) {
//  Do something with sensorValue
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);

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

  setupOutput();
  wsconnect();
}

void loop() {
  String Data;

  webSocketClient.getData(Data);

  if (Data.length() > 0) {
    Serial.print("Received data: ");
    Serial.println(Data.toInt());

    updateOutput(Data.toInt());
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
  String json = "{ \"client\":{ \"id\": ";
  json += outputId;
  json += ", \"outputType\": \"";
  json += outputType + "\"}}";
  Serial.println(json);
  webSocketClient.sendData(json);
  
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, HIGH);
}
