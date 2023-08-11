#include <LiquidCrystal.h>
#include <LCDKeypad.h>
#include <dht.h>
#include <Wire.h>

dht DHT;

char words[][10] = { "I", "go", "to", "school", "by", "bus" };
String display_words = "Waiting the Signal";
LCDKeypad lcd;
int numStrings;
double temp_value = 0;
double humid_value = 0;
char buffer[20];
char mytext[128];
char value1Str[10];
char value2Str[10];
#define DHT11_PIN 3

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
    loopingrun(0,0, "Temp: ");
    loopingrun(1,0, "Humi: ");
    int chk = DHT.read11(DHT11_PIN);
    switch (chk) {
        case DHTLIB_OK:
            Serial.print("");
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
    if(chk == DHTLIB_OK) {
        humid_value = DHT.humidity;
        temp_value = DHT.temperature;


        dtostrf(temp_value,6,2,value1Str);
        dtostrf(humid_value,6,2,value2Str);
        sprintf(mytext, "Your room temperature is%s Degree Celsius, and Humidity is%s%%", value1Str, value2Str);
        loopingrun(0, 6, dtostrf(temp_value,6,2,buffer));
        loopingrun(1, 6, dtostrf(humid_value,6,2,buffer));
        // Serial.print(DHT.humidity, 1);
        // Serial.print(",\t");
        // Serial.print(DHT.temperature, 1);
        Serial.write(mytext, 128);
        delay(1000);
    }

    //digitalWrite(13, HIGH);

    // for (int i = 0; i < 6; i++) {
    //   lcd.setCursor(0, 0);
    //   lcd.print(words[i]);
    //   delay(1000);
    //   lcd.setCursor(0, 0);
    //   lcd.print("                ");
    // }
    unsigned long currentTimestamp = millis();

    // lcd.setCursor(6, 1);
    // lcd.print(currentTimestamp);
    // digitalWrite(13, LOW);

    delay(1000);
    // lcd.clear();
}
void loopingrun(int x, int y, String displayname) {

    lcd.setCursor(y,x);
    lcd.print(displayname);
    //lcd.setCursor(y,1);
    //lcd.print("Marker");
    delay(1000);

}
