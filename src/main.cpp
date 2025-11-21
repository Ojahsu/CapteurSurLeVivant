#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <PubSubClient.h>
#include <WiFi.h>

// RFID pins
#define RST_PIN 9
#define SS_PIN 10
#define LED_VERT_PIN 3
#define LED_ROUGE_PIN 2
#define BUZZER_PIN 5

MFRC522 mfrc522(SS_PIN, RST_PIN);

// WiFi settings
const char *ssid = "Chaton";
const char *password = "Ratatouille";

// MQTT Broker settings
const char *mqtt_broker = "broker.emqx.io";
const char *mqtt_topic1 = "homeTrainerCastres/Group1-C/MAC";
const char *mqtt_topic2 = "homeTrainerCastres/Group1-C/RFID";
const int mqtt_port = 1883;
String client_id = "ArduinoClient-";
String MAC_address = "";

// Other global variables
static unsigned long lastPublishTimeRFID = 0;
static unsigned long lastPublishTimeMAC = 0;

// Leds
static unsigned long lastLedVert = 0;
static unsigned long lastLedRouge = 0;
const bool ledStateLedVert = false;
const bool ledStateLedRouge = false;
WiFiClient espClient;
PubSubClient mqtt_client(espClient);

// Prototypes
void connectToWiFi();
void connectToMQTTBroker();
void mqttCallback(char *topic, byte *payload, unsigned int length);
String getUID();
void printMacAddress();

void setup()
{
    Serial.begin(9600);

    // INITIALISATION DES PINS
    pinMode(LED_VERT_PIN, OUTPUT);
    pinMode(LED_ROUGE_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    
    // WiFi et MQTT
    connectToWiFi();
    mqtt_client.setServer(mqtt_broker, mqtt_port);
    mqtt_client.setCallback(mqttCallback);
    connectToMQTTBroker();
    
    // RFID
    SPI.begin();
    mfrc522.PCD_Init();
    mfrc522.PCD_DumpVersionToSerial();
    Serial.println(F("Scan PICC to read UID and send to MQTT..."));

    digitalWrite(LED_VERT_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_VERT_PIN, LOW);

    digitalWrite(LED_ROUGE_PIN, HIGH);

    // 3 bips de test
    for(int i = 0; i < 2; i++) {
        tone(BUZZER_PIN, 500, 100);
        delay(200);
    }
    
}

void printMacAddress()
{
    byte mac[6];
    Serial.print("MAC Address: ");
    WiFi.macAddress(mac);
    for (int i = 0; i < 6; i++)
    {
        MAC_address += String(mac[i], HEX);
        if (i < 5)
            MAC_address += ":";
        if (mac[i] < 16)
        {
            client_id += "0";
        }
        client_id += String(mac[i], HEX);
    }
    Serial.println(MAC_address);
}

void connectToWiFi()
{
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    delay(3000);
    printMacAddress();
    Serial.println("Connected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
}

void connectToMQTTBroker()
{
    while (!mqtt_client.connected())
    {
        Serial.print("Connecting to MQTT Broker as ");
        Serial.print(client_id.c_str());
        Serial.println(".....");
        if (mqtt_client.connect(client_id.c_str()))
        {
            Serial.println("Connected to MQTT broker");
            mqtt_client.subscribe(mqtt_topic1);
            mqtt_client.subscribe(mqtt_topic2);
            // Publish message upon successful connection
            String message = "Hello EMQX I'm " + MAC_address;
            mqtt_client.publish(mqtt_topic1, message.c_str());
        }
        else
        {
            Serial.print("Failed to connect to MQTT broker, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message received on topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    String messageTemp;
    for (int i = 0; i < length; i++)
    {
        messageTemp += (char)payload[i];
    }
    Serial.println(messageTemp);
    Serial.println("-----------------------");
}

// Fonction pour récupérer l'UID sous forme de String
String getUID()
{
    String content = "";
    
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
        if (mfrc522.uid.uidByte[i] < 0x10)
            content += "0";
        content += String(mfrc522.uid.uidByte[i], HEX);
    }
    
    content.toUpperCase();
    return content;
}

// void loop()
// {
//     // Vérifier connexion MQTT
//     if (!mqtt_client.connected())
//     {
//         connectToMQTTBroker();
//     }
//     mqtt_client.loop();

//     unsigned long currentTime = millis();

//     // Lecture RFID
//     if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
//     {
//         String uidString = getUID();

//         Serial.print("UID détecté: ");
//         Serial.println(uidString);

//         // Envoi de l'UID sur le topic MQTT 2
//         String message = "{\"uid\":\"" + uidString + "\",\"timestamp\":" + String(millis()) + "}";

//         if (mqtt_client.publish(mqtt_topic2, message.c_str()))
//         {
//             Serial.println("UID envoyé sur MQTT topic 2: " + message);
//         }
//         else
//         {
//             Serial.println("Échec de l'envoi de l'UID");
//         }
//         digitalWrite(LED_ROUGE_PIN, LOW);
//         digitalWrite(LED_VERT_PIN, HIGH);
//         tone(BUZZER_PIN, 2000, 150);
//         delay(1000);
//         digitalWrite(LED_VERT_PIN, LOW);
//         digitalWrite(LED_ROUGE_PIN, HIGH);

//         // Halt PICC (à faire après chaque lecture)
//         mfrc522.PICC_HaltA();
//         mfrc522.PCD_StopCrypto1();
//     }

//     // Publication périodique de la MAC address (toutes les 5 secondes)
//     if (currentTime - lastPublishTimeMAC >= 5000)
//     {
//         String message = "MAC Address is " + MAC_address;
//         mqtt_client.publish(mqtt_topic1, message.c_str());
//         Serial.println("Published message: " + message);
//         lastPublishTimeMAC = currentTime;
//         lastLedRouge = currentTime;
//     }
// }

void loop()
{
    // Vérifier connexion MQTT
    if (!mqtt_client.connected())
    {
        connectToMQTTBroker();
    }
    mqtt_client.loop();

    unsigned long currentTime = millis();

    // ✅ ANTI-REBOND : Lecture RFID seulement si 1 seconde s'est écoulée
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
    {
        // Vérifier qu'au moins 1 seconde s'est écoulée depuis la dernière lecture
        if (currentTime - lastPublishTimeRFID >= 1000)
        {
            String uidString = getUID();

            Serial.print("UID détecté: ");
            Serial.println(uidString);

            // Envoi de l'UID sur le topic MQTT 2
            String message = "{\"uid\":\"" + uidString + "\",\"timestamp\":" + String(millis()) + "}";

            if (mqtt_client.publish(mqtt_topic2, message.c_str()))
            {
                Serial.println("✅ UID envoyé sur MQTT: " + message);
                
                // LED verte + buzzer = succès
                digitalWrite(LED_ROUGE_PIN, LOW);
                digitalWrite(LED_VERT_PIN, HIGH);
                tone(BUZZER_PIN, 2000, 150);
                delay(200);
                digitalWrite(LED_VERT_PIN, LOW);
            }
            else
            {
                Serial.println("❌ Échec de l'envoi de l'UID");
                
                // LED rouge clignotante = erreur
                for(int i = 0; i < 3; i++)
                {
                    digitalWrite(LED_ROUGE_PIN, HIGH);
                    tone(BUZZER_PIN, 500, 100);
                    delay(100);
                    digitalWrite(LED_ROUGE_PIN, LOW);
                    delay(100);
                }
            }

            // ✅ METTRE À JOUR LE TIMESTAMP
            lastPublishTimeRFID = currentTime;
        }
        else
        {
            // Carte détectée trop tôt (moins de 1 seconde)
            Serial.println("⏸️  Anti-rebond actif - carte ignorée");
        }

        // Halt PICC (à faire après chaque lecture)
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
    }
    
    // Remettre LED rouge si aucune carte détectée
    if (currentTime - lastPublishTimeRFID > 1500)
    {
        digitalWrite(LED_VERT_PIN, LOW);
        digitalWrite(LED_ROUGE_PIN, HIGH);
    }
}
