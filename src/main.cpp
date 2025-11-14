#include <Arduino.h>

// #include <SPI.h>
// #include <MFRC522.h>

// #define RST_PIN 9
// #define SS_PIN 10
// MFRC522 mfrc522(SS_PIN, RST_PIN);

// void setup()
// {
// 	// Initialization of the RFID module
// 	Serial.begin(9600);
// 	while (!Serial)
// 		;
// 	SPI.begin();
// 	mfrc522.PCD_Init();
// 	mfrc522.PCD_DumpVersionToSerial(); // Output details of the reader
// 	Serial.println(F("Scan PICC to see UID, type, and data blocks..."));
// }

// void loop()
// {
// 	// Search for new cards
// 	if (!mfrc522.PICC_IsNewCardPresent())
// 	{
// 		return;
// 	}
// 	if (!mfrc522.PICC_ReadCardSerial())
// 	{
// 		return;
// 	}
// 	// Information retrieval of the RFID device
// 	mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
// }

#define BUZZER_PIN 5  // ou D5, les deux marchent

void setup() {
    pinMode(BUZZER_PIN, OUTPUT);
    Serial.begin(115200);  // UNO R4 supporte des vitesses plus élevées
    
    Serial.println("Test Buzzer - UNO R4 WiFi");
    Serial.println("Pin D5 configuré");
    
    // Test au démarrage
    delay(500);
    Serial.println("Test du buzzer...");
    
    // 3 bips de test
    for(int i = 0; i < 3; i++) {
        tone(BUZZER_PIN, 1500, 500);
        delay(200);
    }
    
    Serial.println("Test terminé ✓");
}

void loop() {
    // Bip toutes les 2 secondes
    tone(BUZZER_PIN, 1000, 1000);
    Serial.println("Bip!");
    delay(2000);
}

