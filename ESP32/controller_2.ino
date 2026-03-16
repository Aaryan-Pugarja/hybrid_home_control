#include <WiFi.h>
#include <WebSocketsClient.h>

#define RELAY1 5
#define RELAY2 19

bool relay1State = false;
bool relay2State = false;

const char* ssid = "DIGISOL";
const char* password = "2uSR@ha1";

const char* host = "upscontroller.onrender.com";
const uint16_t port = 443;

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

  switch(type) {

    case WStype_CONNECTED:
      Serial.println("WebSocket Connected");
      webSocket.sendTXT("ESP_CONNECTED");
      break;

    case WStype_TEXT: {

      String msg = String((char*)payload);

      Serial.print("Message Received: ");
      Serial.println(msg);

      if(msg == "msg1") {

        relay1State = !relay1State;
        digitalWrite(RELAY1, relay1State);

        Serial.print("Relay1 toggled -> ");
        Serial.println(relay1State ? "ON" : "OFF");
      }

      else if(msg == "msg2") {

        relay2State = !relay2State;
        digitalWrite(RELAY2, relay2State);

        Serial.print("Relay2 toggled -> ");
        Serial.println(relay2State ? "ON" : "OFF");
      }

      break;
    }

    case WStype_DISCONNECTED:
      Serial.println("WebSocket Disconnected");
      break;

    case WStype_ERROR:
      Serial.println("WebSocket Error");
      break;

    default:
      break;
  }
}

void connectWiFi() {

  Serial.println("Connecting WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  Serial.println(WiFi.localIP());
}

void connectWebSocket() {

  webSocket.beginSSL(host, port, "/");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

void setup() {

  Serial.begin(115200);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);

  connectWiFi();
  connectWebSocket();
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  webSocket.loop();
}