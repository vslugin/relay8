/**

   This sketch debugged on the Wi-Fi Kit 8 with display 128x32 pix. IR pin -- d6 (gpio 12)

   HOWTO USE?: Just replace ESSID and ESSID_PASSWORD and flash sketch into your device :-)

*/

#include <Arduino.h> // common

// Display begin
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED
// Display end

// IR begin
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
const uint16_t RECV_PIN = 12; // d6
IRrecv irrecv(RECV_PIN);
decode_results results;
// IR end

// WiFi Client begin
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
ESP8266WiFiMulti WiFiMulti;
// WiFi Client end

// Autoclear message begin
int counter1 = 0;
int delay_counter1 = 6; // seconds/2
// Autoclear message end

// Relay states begin
boolean r1 = false;
boolean r2 = false;
boolean r3 = false;
boolean r4 = false;
boolean r5 = false;
boolean r6 = false;
boolean r7 = false;
boolean r8 = false;
// Relay states end

char *ESSID = "SET_YOUR_ESSID_HERE";
char *ESSID_PASSWORD = "SET_YOUR_ESSID_PASSWORD_HERE";
String RELAY8_URL = "http://192.168.1.2"; // <-- HERE IS IP OF YOUT relay8_esp8266 device

void setup(void) {

  Serial.begin(115200); // common for IR and WiFi

  // Display begin
  u8g2.begin();
  u8g2.enableUTF8Print();    // enable UTF8 support for the Arduino print() function
  // Display end

  // IR Begin
  irrecv.enableIRIn(); // Start the receiver
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  Serial.println();
  Serial.print("Waiting for IR message on Pin ");
  Serial.println(RECV_PIN);
  // IR end


  // WiFi Client begin
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ESSID, ESSID_PASSWORD);

  for (uint8_t t = 4; t > 0; t--) {
    show_message("Подкл. к WIFI...");
    delay(1000);
  }
  counter1 = 1;

  // WiFi Client end

}

void loop(void) {

  /*
    IR Remote codes:
    FFA25D 1
    FF629D 2
    FFE21D 3
    FF22DD 4
    FF02FD 5
    FFC23D 6
    FFE01F 7
    FFA857 8
    FF906F 9
    FF9867 0
    FF6897 *
    FFB04F #
    FF38C7 ok
    FF10EF left
    FF5AA5 right
    FF18E7 up
    FF4AB5 down
  */

  // listen IR Receiver...
  if (irrecv.decode(&results)) {


    if (WiFiMulti.run() != WL_CONNECTED) {
      show_message("Ош. подкл. к WIFI");
      return;
    }

    serialPrintUint64(results.value, HEX);
    Serial.println("");

    switch (results.value) {
      // pressed 1
      case 0xFFA25D:
        if (r1) {
          show_message("Реле 1 Откл");
          http_get(RELAY8_URL + "/r1e");
        } else {
          show_message("Реле 1 Вкл");
          http_get(RELAY8_URL + "/r1d");
        }
        r1 = !r1;
        counter1 = 1;
        break;

      // pressed 2
      case 0xFF629D:
        if (r2) {
          show_message("Реле 2 Откл");
          http_get(RELAY8_URL + "/r2e");
        } else {
          show_message("Реле 2 Вкл");
          http_get(RELAY8_URL + "/r2d");
        }
        r2 = !r2;
        counter1 = 1;
        break;

      // pressed 3
      case 0xFFE21D:
        if (r3) {
          show_message("Реле 3 Откл");
          http_get(RELAY8_URL + "/r3e");
        } else {
          show_message("Реле 3 Вкл");
          http_get(RELAY8_URL + "/r3d");
        }
        r3 = !r3;
        counter1 = 1;
        break;

      // pressed 4
      case 0xFF22DD:
        if (r4) {
          show_message("Реле 4 Откл");
          http_get(RELAY8_URL + "/r4e");
        } else {
          show_message("Реле 4 Вкл");
          http_get(RELAY8_URL + "/r4d");
        }
        r4 = !r4;
        counter1 = 1;
        break;

      // pressed 5
      case 0xFF02FD:
        if (r5) {
          show_message("Реле 5 Откл");
          http_get(RELAY8_URL + "/r5e");
        } else {
          show_message("Реле 5 Вкл");
          http_get(RELAY8_URL + "/r5d");
        }
        r5 = !r5;
        counter1 = 1;
        break;

      // pressed 6
      case 0xFFC23D:
        if (r6) {
          show_message("Реле 6 Откл");
          http_get(RELAY8_URL + "/r6e");
        } else {
          show_message("Реле 6 Вкл");
          http_get(RELAY8_URL + "/r6d");
        }
        r6 = !r6;
        counter1 = 1;
        break;

      // pressed 7
      case 0xFFE01F:
        if (r7) {
          show_message("Реле 7 Откл");
          http_get(RELAY8_URL + "/r7e");
        } else {
          show_message("Реле 7 Вкл");
          http_get(RELAY8_URL + "/r7d");
        }
        r7 = !r7;
        counter1 = 1;
        break;

      // pressed 8
      case 0xFFA857:
        if (r8) {
          show_message("Реле 8 Откл");
          http_get(RELAY8_URL + "/r8e");
        } else {
          show_message("Реле 8 Вкл");
          http_get(RELAY8_URL + "/r8d");
        }
        r8 = !r8;
        counter1 = 1;
        break;

      // pressed *
      case 0xFF6897:
        show_message("Всё вкл!");
        http_get(RELAY8_URL + "/r1d");
        http_get(RELAY8_URL + "/r2d");
        http_get(RELAY8_URL + "/r3d");
        http_get(RELAY8_URL + "/r4d");
        http_get(RELAY8_URL + "/r5d");
        http_get(RELAY8_URL + "/r6d");
        http_get(RELAY8_URL + "/r7d");
        http_get(RELAY8_URL + "/r8d");
        r1 = false;
        r2 = false;
        r3 = false;
        r4 = false;
        r5 = false;
        r6 = false;
        r7 = false;
        r8 = false;
        counter1 = 1;
        break;

      // pressed 0
      case 0xFF9867:
        show_message("Всё Откл!");
        http_get(RELAY8_URL + "/r1e");
        http_get(RELAY8_URL + "/r2e");
        http_get(RELAY8_URL + "/r3e");
        http_get(RELAY8_URL + "/r4e");
        http_get(RELAY8_URL + "/r5e");
        http_get(RELAY8_URL + "/r6e");
        http_get(RELAY8_URL + "/r7e");
        http_get(RELAY8_URL + "/r8e");
        r1 = true;
        r2 = true;
        r3 = true;
        r4 = true;
        r5 = true;
        r6 = true;
        r7 = true;
        r8 = true;
        counter1 = 1;
        break;
    }
    irrecv.resume();  // Receive the next value
  }

  delay(500); // delay 1/2 second

  // clear display aster delay_counter1 sec/2 (becouse loop delay is 1/2 second)
  if (counter1 > 0) {
    counter1++;
    if (counter1 == delay_counter1) {
      clear_message();
      counter1 = 0;
    }
  }

}

// show message
void show_message(char *message) {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_unifont_t_cyrillic);
  u8g2.drawUTF8(5, 18, message); // write something to the internal memory
  u8g2.drawRFrame(0, 0, 128, 32, 1);
  u8g2.sendBuffer();          // transfer internal memory to the display
}

// clear display
void clear_message() {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
}

// http_get
void http_get(String url) {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    Serial.print(url);
    if (http.begin(client, url)) {  // HTTP
      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
      show_message("Ошибка соединения");
      counter1 = 1;
    }
  }
}
