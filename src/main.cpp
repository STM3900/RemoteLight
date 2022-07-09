#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = "Wii-Fit";          // Enter your WiFi name
const char *password = "Gnocking1234"; // Enter WiFi password

// MQTT Broker
const char *mqttServer = "192.168.1.49"; // mqttServer Ip
const int mqttPort = 1883;
const char *mqttUser = "homeassistant";                                                        // User
const char *mqttPassword = "Oghon2zae9em6ok7eixae4Quaeghahl6Kie5Diwio2eezoPh9Aedahghahkoh2ei"; // Password

const int buttonSwitchPin = 16;
const int buttonWhitePin = 5;
const int buttonRedPin = 14;
const int buttonGreenPin = 13;
const int buttonBluePin = 15;

const int buttonArrPin = 12;
const int ledPin = 4;

char *topicWhite = "white";
char *topicRed = "red";
char *topicGreen = "green";
char *topicBlue = "blue";
char *topicArr = "arr";

bool isPressed = false;
bool lightStatus = false;

// Initialisation du module Wifi et du service PubSub
WiFiClient espClient;
PubSubClient client(espClient);

// Fonction de callback : est utile si l'on est subscribed à un topic et se déclanche quand on reçoit un message
void callback(char *topic, byte *payload, unsigned int length)
{

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  // Faire des trucs

  String message;
  for (int i = 0; i < length; i++)
  {
    message = message + (char)payload[i]; // convert *byte to string
  }
  Serial.print(message);

  if (message == "arr")
  {
    digitalWrite(ledPin, HIGH);
  }

  Serial.println();
  Serial.println("-----------------------");
}

void setup()
{
  // Start serial communication for debugging purposes
  Serial.begin(9600);

  pinMode(buttonSwitchPin, INPUT);
  pinMode(buttonWhitePin, INPUT);
  pinMode(buttonRedPin, INPUT);
  pinMode(buttonGreenPin, INPUT);
  pinMode(buttonBluePin, INPUT);

  pinMode(buttonArrPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Initialisation du wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Reste dans cette boucle tant que l'on n'est pas connecté au wifi
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  // Initialisation de l'instance vers le serveur mqtt
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  // Tant qu'on est pas connecté au mqtt, on essaie
  while (!client.connected())
  {
    Serial.println("Connecting to MQTT...");

    if (client.connect("EspLamp", mqttUser, mqttPassword))
    {

      Serial.println("connected");
    }
    else
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

  client.subscribe("arr");
}

void testToggleButton()
{
  if (digitalRead(buttonSwitchPin) == HIGH && !isPressed)
  {
    isPressed = true;

    if (lightStatus)
    {
      client.publish("off", "Light off");
      lightStatus = false;
      Serial.println("off");
    }
    else
    {
      client.publish("on", "Light on");
      lightStatus = true;
      Serial.println("on");
    }

    delay(200);
  }
  else if (digitalRead(buttonSwitchPin) == LOW && isPressed)
  {
    isPressed = false;
  }
}

void testColorButton(int pin, char *topic)
{
  if (digitalRead(pin) == HIGH && !isPressed)
  {
    isPressed = true;

    client.publish(topic, topic);
    Serial.println(topic);

    lightStatus = true;
    delay(200);
  }
  else if (digitalRead(pin) == LOW && isPressed)
  {
    isPressed = false;
  }
}

void loop()
{
  client.loop();

  testToggleButton();

  testColorButton(buttonWhitePin, topicWhite);
  testColorButton(buttonRedPin, topicRed);
  testColorButton(buttonGreenPin, topicGreen);
  testColorButton(buttonBluePin, topicBlue);
  testColorButton(buttonArrPin, topicArr);
}