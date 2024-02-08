#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

const char* ssid     = "StarNet - vition.alexandru_EXT";
const char* password = "485754430522A3A2";

String ptr  = "<!DOCTYPE html>                                                   <html>                                             <head>        <title>masina</title>    </head>    <body>        <script>            function key_callback(){                document.addEventListener('keydown', function(event){                if (event.key == \"w\" || event.key == \"W\") {                    const xhr = new XMLHttpRequest();                    xhr.open(\"POST\", \"/W\");                    xhr.send(\"\");                }                if (event.key == \"s\" || event.key == \"S\") {                    const xhr = new XMLHttpRequest();                    xhr.open(\"POST\", \"/S\");                    xhr.send(\"\");                }                if (event.key == \"a\" || event.key == \"A\") {                    const xhr = new XMLHttpRequest();                    xhr.open(\"POST\", \"/A\");                    xhr.send(\"\");                }                if (event.key == \"d\" || event.key == \"D\") {                    const xhr = new XMLHttpRequest();                    xhr.open(\"POST\", \"/D\");                    xhr.send(\"\");                }                if (event.key == \"b\" || event.key == \"B\") {                    const xhr = new XMLHttpRequest();                    xhr.open(\"POST\", \"/B\");                    xhr.send(\"\");                }                if (event.key == \"n\" || event.key == \"N\") {                    const xhr = new XMLHttpRequest();                    xhr.open(\"POST\", \"/N\");                    xhr.send(\"\");                }                if (event.key == \"q\" || event.key == \"Q\") {                    const xhr = new XMLHttpRequest();                    xhr.open(\"POST\", \"/Q\");                    xhr.send(\"\");                }                if (event.key == \"e\" || event.key == \"E\") {                    const xhr = new XMLHttpRequest();                    xhr.open(\"POST\", \"/E\");                    xhr.send(\"\");                }                if (event.key == \"f\" || event.key == \"F\") {                    const xhr = new XMLHttpRequest();                    xhr.open(\"POST\", \"/F\");                    xhr.send(\"\");                }                }, true);            }            key_callback();        </script>    </body></html>";

ESP8266WebServer server(80);

int8_t cnt = 0;

int a = 0;

int p = 0;

bool f = false;

unsigned int tim = 0;

bool onoff = false;

void handleRoot() {
  server.send(200, "text/html", ptr);
}

void handleW() {
  digitalWrite(0, LOW);
  digitalWrite(2, HIGH);
  digitalWrite(12, LOW);
  Serial.println("W");
}

void handleS() {
  digitalWrite(0, HIGH);
  digitalWrite(2, LOW);
  digitalWrite(12, LOW);
  Serial.println("S");
}

void handleA() {
  if(cnt >= -3){
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    delay(5);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    Serial.println("A");
    cnt--;
    EEPROM.write(a, cnt);
  }
}

void handleD() {
  if(cnt <= 3){
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    delay(5);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    Serial.println("D");
    cnt++;
    EEPROM.write(a, cnt);
  }
}

void handleB() {
  digitalWrite(0, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(12, HIGH);
  Serial.println("B");
}

void handleN() {
  digitalWrite(0, LOW);
  digitalWrite(2, LOW);
  digitalWrite(12, LOW);
  Serial.println("N");
}

void handleQ() {
  switch(p){
    case 1:
      p = 0;
      break;
    default:
      p = 1;
      break;
  }
  Serial.println("Q");
}

void handleE() {
  switch(p){
    case 2:
      p = 0;
      break;
    default:
      p = 2;
      break;
  }
  Serial.println("E");
}

void handleF() {
  switch(f){
    case false:
      digitalWrite(13, HIGH);
      f = true;
      break;
    case true:
      digitalWrite(13, LOW);
      f = false;
      break;
  }
  Serial.println("F");
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found");
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  if (MDNS.begin("esp8266")) {
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }
  server.on("/W", HTTP_POST, handleW);
  server.on("/S", HTTP_POST, handleS);
  server.on("/A", HTTP_POST, handleD);
  server.on("/D", HTTP_POST, handleA);
  server.on("/B", HTTP_POST, handleB);
  server.on("/N", HTTP_POST, handleN);
  server.on("/F", HTTP_POST, handleF);  
  server.on("/Q", HTTP_POST, handleE);
  server.on("/E", HTTP_POST, handleQ);
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(16, OUTPUT);
  cnt = EEPROM.read(a);
}

void loop() {
  server.handleClient();
  tim++;
  if(onoff == true){
    switch(p){
      case 0:
        digitalWrite(14, LOW);
        digitalWrite(16, LOW);
        break;
      case 1:
        digitalWrite(14, HIGH);
        digitalWrite(16, LOW);
        break;
      case 2:
        digitalWrite(14, LOW);
        digitalWrite(16, HIGH);
        break;
    }
    if(tim == 10000){
      onoff = !onoff;
      tim = 0;
    }
  }else{
    digitalWrite(14, LOW);
    digitalWrite(16, LOW);
    if(tim == 10000){
      onoff = !onoff;
      tim = 0;
    }
  }
}
