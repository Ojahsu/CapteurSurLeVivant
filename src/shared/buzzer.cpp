// #include <Arduino.h>

// #define BUZZER_PIN 5

// void setup() {
//     pinMode(BUZZER_PIN, OUTPUT);
//     Serial.begin(115200);  // UNO R4 supporte des vitesses plus élevées
    
//     Serial.println("Test Buzzer - UNO R4 WiFi");
//     Serial.println("Pin D5 configuré");
    
//     // Test au démarrage
//     delay(500);
//     Serial.println("Test du buzzer...");
    
//     // 3 bips de test
//     for(int i = 0; i < 3; i++) {
//         tone(BUZZER_PIN, 1500, 500);
//         delay(200);
//     }
    
//     Serial.println("Test terminé ✓");
// }

// void loop() {
//     // Bip toutes les 2 secondes
//     tone(BUZZER_PIN, 1000, 1000);
//     Serial.println("Bip!");
//     delay(2000);
// }