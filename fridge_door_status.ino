#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>


// Connect to the WiFi
const char* ssid = "FabLab_NBG";
const char* password = "fablabnbg";
const char* mqtt_server = "172.16.16.1";

WiFiClient espClient;
PubSubClient client(espClient);

const byte ledPin = D1; // Pin with LED on Adafruit Huzzah

void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived [");
 Serial.print(topic);
 Serial.print("] ");
 for (int i=0;i<length;i++) {
  char receivedChar = (char)payload[i];
  Serial.print(receivedChar);
  if (receivedChar == '1')
  // ESP8266 Huzzah outputs are "reversed"
  digitalWrite(ledPin, HIGH);
  if (receivedChar == '0')
   digitalWrite(ledPin, LOW);
  }
  Serial.println();
}


void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("ESP8266 Client")) {
  Serial.println("connected");
  // ... and subscribe to topic
  client.subscribe("lab/sensor/fridge/door");
 } else {
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
  }
 }
}

void setup()
{
 Serial.begin(9600);

 
 WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("..");

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP


 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);

 pinMode(ledPin, OUTPUT);
}

void loop()
{
 if (!client.connected()) {
  reconnect();
 }
 client.loop();
}
