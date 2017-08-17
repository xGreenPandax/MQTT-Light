/*

  - подключаемся к брокеру
  - подписывается на топик "home/room1/light"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid =  "";    // подключение к wifi
const char *pass =  "";   

IPAddress server(136, 3, 123, 33); // указываем IP нашего брокера

#define BUFFER_SIZE 100

void callback(const MQTT::Publish& pub) {
  
  Serial.print(pub.topic());
  Serial.print(" => ");
  Serial.println(pub.payload_string());
    if(pub.payload_string()=="on"){
      Serial.println("ON");
      digitalWrite(D8, LOW);
    }
    if(pub.payload_string()=="off"){
      Serial.println("OFF");
      digitalWrite(D8, HIGH);
    }
  
}

WiFiClient wclient;
PubSubClient client(wclient, server);

void setup() {
  Serial.begin(115200);
  delay(50);
  pinMode(D8, OUTPUT); // реле на ногу D8
  Serial.println();
  Serial.println();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
  }

  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      if (client.connect("arduinoClient")) {
  client.set_callback(callback);
  client.subscribe("home/room1/light");
  
      }
    }

    if (client.connected())
      client.loop();
  }
}
