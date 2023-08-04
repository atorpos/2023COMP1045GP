#include <Arduino.h>
#include <LiquidCrystal.h>
#include <LCDKeypad.h>
#include <dht.h>

dht DHT;

char words[][10] = { "I", "go", "to", "school", "by", "bus" };
String display_words = "Waiting the Signal";
LCDKeypad lcd;
int numStrings;
#define DHT11_PIN 3
#define DHT21_PIN 5
#define DHT22_PIN 6

void setup() {
    Serial.begin(9600);
    lcd.begin(16, 2);
    pinMode(13, OUTPUT);
}

void loop() {
    // for(int j = 0; j <10; j++) {
    //   lcd.setCursor(1,1);
    //   lcd.print(j);
    //   delay(500);
    // }
    int chk = DHT.read11(DHT11_PIN);
    switch (chk) {
        case DHTLIB_OK:
            Serial.print("OK,\t");
            break;
        case DHTLIB_ERROR_CHECKSUM:
            Serial.print("Checksum error,\t");
            break;
        case DHTLIB_ERROR_TIMEOUT:
            Serial.print("Time out error,\t");
            break;
        default:
            Serial.print("Unknown error,\t");
            break;
    }
    Serial.print(DHT.humidity, 1);
    Serial.print(",\t");
    Serial.print(DHT.temperature, 1);
    digitalWrite(13, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Time:");
    for (int i = 0; i < 6; i++) {
        lcd.setCursor(0, 0);
        lcd.print(words[i]);
        delay(1000);
        lcd.setCursor(0, 0);
        lcd.print("                ");
    }
    unsigned long currentTimestamp = millis();

    lcd.setCursor(6, 1);
    lcd.print(currentTimestamp);
    digitalWrite(13, LOW);
    delay(1000);
}
