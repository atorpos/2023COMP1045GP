#include <WiFiNINA.h>
#include <Arduino.h>
// #include <dht.h>
#include "arduino_secrets.h"
#include <SPI.h>
// dht DHT;

// #define DHT11_PIN 6

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
char* auToken = TWILIO_TOKEN;
int msg_send = 0;

const char *data[] = {
        "From", "+16132094044",
        "Body","Your room temperature is 26 Degree Celsius, and Humidity is 49%",
        "To","+14378484244"
};

int status = WL_IDLE_STATUS;
String website = "www.google.com";
int port = 443;
WiFiClient client;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to network: ");
        Serial.println(ssid);
        status = WiFi.begin(ssid, pass);

        delay(1000);
    }

    Serial.println("You are connected");
    Serial.println("-------------------");
    printData();
    Serial.println("-------------------");
}

void loop() {
    // put your main code here, to run repeatedly:
    // int chk = DHT.read11(DHT11_PIN);
    // Serial.print(DHT.humidity, 1);
    // Serial.print(",\t");
    // Serial.print(DHT.temperature, 1);
    // switch (chk) {
    //   case DHTLIB_OK:
    //     Serial.print("OK,\t");
    //     break;
    //   case DHTLIB_ERROR_CHECKSUM:
    //     Serial.print("Checksum error,\t");
    //     break;
    //   case DHTLIB_ERROR_TIMEOUT:
    //     Serial.print("Time out error,\t");
    //     break;
    //   default:
    //     Serial.print("Unknown error,\t");
    //     break;
    // }
    // Serial.print(DHT.humidity, 1);
    delay(1000);
    printData();
    afterconnect();
    Serial.println("----------------------------------------");

    if(WiFi.status() == WL_CONNECTED){

        size_t dataLength = sizeof(data) / sizeof(data[0]);

        String queryString = http_build_query(data, dataLength);

        String postData = queryString;
        String request = "POST /2010-04-01/Accounts/AC2a92f88b91fbdd37ac12e8f22c63019d/Messages.json HTTP/1.1\r\n";
        request += "Host: api.twilio.com\r\n";
        request += "Content-Type: application/x-www-form-urlencoded\r\n";
        request += "Content-Length: " + String(queryString.length()) + "\r\n";
        request += "Authorization: Basic " + base64Encode(String(auToken)) + "\r\n"; // Include the Basic Authentication header
        request += "Connection: close\r\n\r\n";
        request += postData + "\r\n";
        if(msg_send == 0) {
            if(client.connectSSL("api.twilio.com", 443)) {
                client.print(request);

                while(client.connected()){
                    if(client.available()) {
                        String statusLine = client.readStringUntil('\r');
                        int statusCode = statusLine.substring(9, 12).toInt(); // Extract the three-digit status code
                        Serial.print("Status Code: ");
                        Serial.println(statusCode);
                        if(statusCode == 201) {
                            msg_send = 1;
                            client.stop();
                        }

                        // Print the rest of the server response
                        Serial.println(client.readStringUntil('\n'));
                        // char c = client.read();
                        // Serial.print(c);
                    }
                }
                client.stop();
            } else {
                Serial.println("connection fail");
            }
        } else {
            Serial.println("Message sent");
        }

    } else {
        Serial.println("Wifi fail");
    }
    delay(10000);
}

void printData() {
    Serial.println("Board Information:");
    // print your board's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    Serial.println();
    Serial.println("Network Information:");
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.println(rssi);

    byte encryption = WiFi.encryptionType();
    Serial.print("Encryption Type:");
    Serial.println(encryption, HEX);
    Serial.println();
}

void afterconnect() {
    if (WiFi.firmwareVersion() < WIFI_FIRMWARE_LATEST_VERSION) {
        Serial.println("Firmware upgrade available");
    }
}

String base64Encode(String str) {
    char base64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    String encoded;
    int val = 0;
    int valb = -6;
    for (uint8_t i = 0; i < str.length(); i++) {
        char c = str.charAt(i);
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            encoded += base64Table[(val >> valb) & 0x3F];
            valb -= 6;
        }
    }
    if (valb > -6) {
        encoded += base64Table[((val << 8) >> (valb + 8)) & 0x3F];
    }
    while (encoded.length() % 4) {
        encoded += '=';
    }
    return encoded;
}

String http_build_query(const char* data[], size_t dataLength) {
    String query;
    for (size_t i = 0; i < dataLength; i += 2) {
        if (i > 0) {
            query += '&';
        }
        query += data[i];
        query += '=';
        query += urlEncode(data[i + 1]);
    }
    return query;
}

String urlEncode(const char* str) {
    String encoded;
    char c;
    while ((c = *str++)) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded += c;
        } else {
            encoded += '%';
            encoded += String(c, HEX);
        }
    }
    return encoded;
}
