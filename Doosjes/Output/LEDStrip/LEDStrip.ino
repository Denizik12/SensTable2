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

String outputId = "3";
String outputType = "LEDStrip";

#define greenLED 13    //D7
#define redLED 15      //D8

#define led1 16        //D0
#define led2 5         //D1
#define led3 4         //D2
#define led4 0         //D3
#define led5 2         //D4
#define led6 14        //D5

WebSocketClient webSocketClient;
// Use WiFiClient class to create TCP connections
WiFiClient client;

void setupOutput() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
  digitalWrite(led6, LOW);
}

void updateOutput(float sensorValue) {
  if (sensorValue == 0) {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
  } else if (sensorValue > 0 && sensorValue < 5) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
  } else if (sensorValue >= 5 && sensorValue < 10) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
  } else if (sensorValue >= 10 && sensorValue < 15) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
  } else if (sensorValue >= 15 && sensorValue < 20) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
  } else if (sensorValue >= 20 && sensorValue < 25) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led6, LOW);
  }
  else if (sensorValue >= 25 && sensorValue <= 30) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led6, HIGH);
  }
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
