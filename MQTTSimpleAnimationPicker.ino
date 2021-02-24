#include <WiFi.h>
#include <PubSubClient.h>
#include <FastLED.h>
#include <ArduinoJson.h>
#include "C_Animations.h"
#include "A_Globals.h"

// Replace the next variables with your SSID/Password combination
const char* ssid = "SSID";
const char* password = "Password";

// MQTT Settings
const char* mqtt_server = "XXX.XXX.XXX.XXX";
const char* mqttPass =    "Password";
const char* mqttUser =    "User";
#define mqttAnimation     "ESP32-1/animation"
#define mqttColour        "ESP32-1/colour"
//#define NUM_LEDS          50

// Create Wifi & MQTT Clients
WiFiClient espClient;
PubSubClient client(espClient);
//Timer variables
long lastMsg = 0;
char msg[50];
int value = 0;

// Pin definitions
const int ledPin = 4;
const int ledMQTTPin = 21;
const int ledPowerPin = 22;
const int ledWifiPin = 23;

/////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  // Setup Serial, Wifi, and MQTT server
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // Subscribe to topics
  client.subscribe(mqttColour);
  client.subscribe(mqttAnimation);

  pinMode(ledPowerPin, OUTPUT);
  pinMode(ledWifiPin, OUTPUT);
  pinMode(ledMQTTPin, OUTPUT);

  //Turn power LED on
  digitalWrite(ledPowerPin, HIGH);

  //Define the properties of the array 'Leds'
  FastLED.addLeds<WS2811, ledPin, RGB>(Leds, NUM_LEDS);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ledWifiPin, LOW);
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(ledWifiPin, HIGH);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageArray;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageArray += (char)message[i];
  }
  Serial.println();

  // Incoming Colour topic
  if (String(topic) == mqttColour) {
    Serial.print("New colour choice: ");
    Serial.println(messageArray);
    
    DynamicJsonDocument mess(256);
    deserializeJson(mess, messageArray);
    r = mess["r"];
    g = mess["g"];
    b = mess["b"];
  }
  // Incoming Animation topic
  if (String(topic) == mqttAnimation) {
    Serial.print("New Animation choice: ");
    Serial.print(messageArray);
    animation = messageArray.toInt();
    Serial.print(animation);
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client", mqttUser, mqttPass)) {
      Serial.println("connected");
      digitalWrite(ledMQTTPin, HIGH);
      client.subscribe(mqttColour);
      client.subscribe(mqttAnimation);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  if (!client.connected()) {
    digitalWrite(ledMQTTPin, LOW);
    reconnect();
  }
  client.loop();

  switch(animation) {
    case 0: fill_solid(Leds,NUM_LEDS,CRGB(r,g,b)); break;
    case 1: DrawMarquee(); break;
    case 2: DrawMarquee(); break;
    case 3: DrawComet(); break;
  }
    FastLED.show();
}
