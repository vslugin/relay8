#include <ESP8266WiFi.h>
#include <EEPROM.h>

#ifndef STASSID
#define STASSID "YOUR_ESSID"
#define STAPSK  "YOUR_PASSWORD"
#endif

#include <ESP8266WebServer.h>

/* Установите здесь свои SSID и пароль */
//const char* ssid = "RELAY8";       // SSID
//const char* password = "12345678";  // пароль

const char* ssid     = STASSID;
const char* password = STAPSK;

int addr = 0; // eeprom byte address
uint8_t state = 0; // one byte -- 8 relays

/* Настройки IP адреса */
//IPAddress local_ip(192,168,1,1);
//IPAddress gateway(192,168,1,1);
//IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

uint8_t LED1pin = 16;
uint8_t LED2pin = 5;
uint8_t LED3pin = 4;
uint8_t LED4pin = 0;
uint8_t LED5pin = 2;
uint8_t LED6pin = 14;
uint8_t LED7pin = 12;
uint8_t LED8pin = 13;

const byte RELAY_COUNT = 8;

byte relays[RELAY_COUNT] = {
  LED1pin,
  LED2pin,
  LED3pin,
  LED4pin,
  LED5pin,
  LED6pin,
  LED7pin,
  LED8pin
};

void setRelayBit(int ri, bool value) {
  if (value) {
    state = state | (1 << ri);
  } else {
    state = state ^ (1 << ri);
  }
  Serial.println(state, BIN);
}

bool getRelayBit(int ri) {
  return (state & (1 << ri)) > 0;
}

void saveState() {
  EEPROM.write(addr, state);
  if (EEPROM.commit()) {
    Serial.println("EEPROM successfully committed");
  } else {
    Serial.println("ERROR! EEPROM commit failed");
  }
}

void loadState() {
  state = EEPROM.read(addr);
}

void initRelays() {
  loadState();
  for (int idx = 0; idx < RELAY_COUNT; ++idx) {
    digitalWrite(relays[idx], getRelayBit(idx));
  }
}

void toggleAndSave(int ri, bool value) {
  digitalWrite(relays[ri], value);
  setRelayBit(ri, value);
  saveState();
};

void setup()
{
  Serial.begin(115200);
  EEPROM.begin(512);
  for (int idx = 0; idx < RELAY_COUNT; ++idx) {
    pinMode(relays[idx], OUTPUT);
  }

  // delay(10);
  initRelays();

  // WiFi.softAP(ssid, password);
  // WiFi.softAPConfig(local_ip, gateway, subnet);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);


  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(100);

  server.on("/", handle_OnConnect);
  server.on("/st", handle_state);
  server.on("/r1e", handle_led1on);
  server.on("/r1d", handle_led1off);
  server.on("/r2e", handle_led2on);
  server.on("/r2d", handle_led2off);
  server.on("/r3e", handle_led3on);
  server.on("/r3d", handle_led3off);
  server.on("/r4e", handle_led4on);
  server.on("/r4d", handle_led4off);
  server.on("/r5e", handle_led5on);
  server.on("/r5d", handle_led5off);
  server.on("/r6e", handle_led6on);
  server.on("/r6d", handle_led6off);
  server.on("/r7e", handle_led7on);
  server.on("/r7d", handle_led7off);
  server.on("/r8e", handle_led8on);
  server.on("/r8d", handle_led8off);

  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
}

void handle_OnConnect()
{
  Serial.println("");
  server.send(200, "text/html", SendHTML());
}

void handle_relay(int ri, bool value)
{

  if (value) {
    if (!getRelayBit(ri)) {
      toggleAndSave(ri, value);
    }
  } else {
    if (getRelayBit(ri)) {
      toggleAndSave(ri, value);
    }
  }

  Serial.println("relay" + String(ri) + " Status: " + (value ? "ON" : "OFF"));
  server.send(200, "application/json", "{\"relay\":\"" + String(ri) + "\", \"status\":\"" + (value ? "ON" : "OFF") + "\"}");
}

void handle_state() {
  String st = "[\"" + String(getRelayBit(0)) + "\",\"" + String(getRelayBit(1)) + "\",\"" + String(getRelayBit(2)) + "\",\"" + String(getRelayBit(3)) + "\",\"" + String(getRelayBit(4)) + "\",\"" + String(getRelayBit(5)) + "\",\"" + String(getRelayBit(6)) + "\",\"" + String(getRelayBit(7)) + "\"]";
  server.send(200, "application/json", st);
}

void handle_led1on()
{
  handle_relay(0, HIGH);
}

void handle_led1off()
{
  handle_relay(0, LOW);
}

void handle_led2on()
{
  handle_relay(1, HIGH);
}

void handle_led2off()
{
  handle_relay(1, LOW);
}


void handle_led3on()
{
  handle_relay(2, HIGH);
}

void handle_led3off()
{
  handle_relay(2, LOW);
}

void handle_led4on()
{
  handle_relay(3, HIGH);
}

void handle_led4off()
{
  handle_relay(3, LOW);
}

void handle_led5on()
{
  handle_relay(4, HIGH);
}

void handle_led5off()
{
  handle_relay(4, LOW);
}

void handle_led6on()
{
  handle_relay(5, HIGH);
}

void handle_led6off()
{
  handle_relay(5, LOW);
}

void handle_led7on()
{
  handle_relay(6, HIGH);
}

void handle_led7off()
{
  handle_relay(6, LOW);
}

void handle_led8on()
{
  handle_relay(7, HIGH);
}

void handle_led8off()
{
  handle_relay(7, LOW);
}

void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}

String SendHTML()
{
  String ptr = "<!DOCTYPE html><html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "    <meta name=\"theme-color\" content=\"#f8f8ff\">\n";
  ptr += "    <meta name=\"mobile-web-app-capable\" content=\"yes\">\n";
  ptr += "    <meta name=\"application-name\" content=\"RELAY8\">\n";
  ptr += "    <title>RELAY8</title>\n";
  ptr += "    <style>html{font-family:Helvetica;display:inline-block;margin:0px auto;text-align:center;}\n";
  ptr += "    body{margin-top:50px;background-color:ghostwhite;overscroll-behavior:contain;}\n";
  ptr += "    .b{display:block;height:160px;width:160px;background-color:lightblue;border:none;color:whitesmoke;\n";
  ptr += "        text-decoration:none;font-size: 50px;margin:0px auto 25px;cursor:pointer;border-radius: 10px;}\n";
  ptr += "    </style></head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"app\">\n";
  ptr += "</div>\n";
  ptr += "<script>let st;const c=new AbortController();const tId=setTimeout(()=>c.abort(),2000)\n";
  ptr += "async function r(){\n";
  ptr += "    try {\n";
  ptr += "        st = await (await fetch('/st',{signal:c.signal})).json();clearTimeout(tId);let m='';\n";
  ptr += "        document.querySelector('#app').innerHTML = '';\n";
  ptr += "        for (let i=0;i<st.length;i++) {\n";
  ptr += "            const eSt=st[i]==='1';const t=eSt?'/'+(i+1)+'\\\\':'\\\\'+(i+1)+'/';const eSt_=eSt?'d':'e'\n";
  ptr += "            document.querySelector('#app').insertAdjacentHTML('beforeEnd',`<button id=\"b${i}\" class=\"b\">${t}</button><br />`);\n";
  ptr += "            const cb=()=>{fetch(`/r${i+1}${eSt_}`).then(()=>{r()})}\n";
  ptr += "            document.getElementById(`b${i}`).removeEventListener('click',cb)\n";
  ptr += "            document.getElementById(`b${i}`).addEventListener('click',cb)\n";
  ptr += "        }\n";
  ptr += "    } catch(e){\n";
  ptr += "        st='CONNECTION ERROR';document.querySelector('#app').innerHTML=st + e.toString();\n";
  ptr += "    }\n";
  ptr += "}\n";
  ptr += "(async()=>{await r()})()</script></body></html>\n";
  return ptr;
}
