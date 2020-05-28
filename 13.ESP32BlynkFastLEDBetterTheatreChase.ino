#include <FastLED.h>
#include <SPI.h>
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>

#define dPin 2
#define NUM_LEDS 50
CRGB leds [NUM_LEDS];
#define BLYNK_PRINT Serial

int R = 255;
int G = 255;
int B = 255;
int chaseSpeed = 20;
int gapSize = 5;
int Tail = 1;
long currentMillis = 0;
long lastMillis = 0;

const char* ssid = "ssid";
const char* password = "password";

//Setup
*************************************************************************
void setup()
{
//Begin Serial Monitor
Serial.begin(115200);
//Connect to WiFi
/*WiFi.begin(ssid,password);
Serial.print("Connecting to WiFi");

while (WiFi.status() != WL_CONNECTED) {
  Serial.print(".");
  delay(500);
  }

Serial.println("\nConnected to the WiFi network");
Serial.print("IP address: ");
Serial.println(WiFi.localIP());
Serial.println(WiFi.macAddress());u*/

FastLED.addLeds<WS2811, dPin> (leds, NUM_LEDS);

Blynk.begin("Blynk auth token", "ssid", "password");
}

//Blynk read values
*************************************************************************
BLYNK_WRITE(V1) {
  chaseSpeed = param.asInt();
}

BLYNK_WRITE(V2) {
  R = param[0].asInt();
  G = param[1].asInt();
  B = param[2].asInt();
  }

BLYNK_WRITE(V3) {
  gapSize = param.asInt();
}

BLYNK_WRITE(V4) {
  tail = param.asInt();
}

//Loop
*************************************************************************
void loop()
{
  Blynk.run();
  TheatreChase();
  //debugValues();
}

void TheatreChase() {
  static int T = 0;
  currentMillis = millis();
  if (T > gap) {
    T = 0;}

  if ((currentMillis - lastMillis) > ((1000/chaseSpeed+1))) {
      T = T+1;
      lastMillis = currentMillis;}
      
  for (int i=0; (i+T)<NUM_LEDS; i=i+gapSize){
      leds[i+T]=CRGB(R,G,B);}
  FastLED.show();

  EVERY_N_MILLISECONDS(50) {
  fadeToBlackBy(leds,NUM_LEDS,(256/(tail+1)));}
}

//Debugging
/*************************************************************************
void debugValues() {
  EVERY_N_MILLISECONDS(1000) {
    Serial.print("V1 Speed = ");
    Serial.println(chaseSpeed);
    Serial.print("V2 Red = ");
    Serial.println(R);
    Serial.print("V2 Green = ");
    Serial.println(G);
    Serial.print("V2 Blue = ");
    Serial.println(B);
    Serial.print("V3 Gap = ");
    Serial.println(gapSize);
    Serial.print("V4 Tail = ");
    Serial.println(Tail);
  }
}
*/
