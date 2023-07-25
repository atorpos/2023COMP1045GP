#include <LiquidCrystal.h>
#include <LCDKeypad.h>

char words[][10] = {"I","go","to","school","by", "bus"};
LCDKeypad lcd;
int numStrings;
void setup()
{
    Serial.begin(9600);
    lcd.begin(16,2);
    pinMode(13, OUTPUT);
}

void loop()
{
    // for(int j = 0; j <10; j++) {
    //   lcd.setCursor(1,1);
    //   lcd.print(j);
    //   delay(500);
    // }
    digitalWrite(13, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Time:");
    for(int i = 0; i < 6; i++){
        lcd.setCursor(0,0);
        lcd.print(words[i]);
        delay(1000);
        lcd.setCursor(0,0);
        lcd.print("                ");
    }
    unsigned long currentTimestamp = millis();

    lcd.setCursor(6, 1);
    lcd.print(currentTimestamp);
    digitalWrite(13, LOW);
    delay(1000);
}
