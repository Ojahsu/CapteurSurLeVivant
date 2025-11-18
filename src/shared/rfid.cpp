// #include <Arduino.h>

// #include <SPI.h>
// #include <MFRC522.h>

// #define RST_PIN 9
// #define SS_PIN 10
// MFRC522 mfrc522(SS_PIN, RST_PIN);


// void setup()
// {
// 	// Initialization of the RFID module
// 	Serial.begin(9600);
// 	while (!Serial);
// 	SPI.begin();
// 	mfrc522.PCD_Init();
// 	mfrc522.PCD_DumpVersionToSerial(); // Output details of the reader
// 	Serial.println(F("Scan PICC to see UID, type, and data blocks..."));

//     // map<int, int[]> dict1;
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

// void login()
// {
//     // Fonction de connexion (à implémenter)
//     Serial.println("Tentative de connexion...");
//     if(true) // Remplacer par la condition réelle de succès
//     {
//         Serial.println("Connexion réussie !");
//     }
//     else
//     {
//         Serial.println("Échec de la connexion.");
//     }
// }