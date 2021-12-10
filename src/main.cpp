#include <DallasTemperature.h>
#include <OneWire.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include "network_settings.h"

#define SERIAL_BAUD 9600
#define LED_PIN 2
#define ONEWIRE_BUS 4
#define WEBSERVER_PORT 80

AsyncWebServer server(WEBSERVER_PORT);
OneWire oneWire(ONEWIRE_BUS);
DallasTemperature sensor(&oneWire);

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(SERIAL_BAUD);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print('.');
  }

  Serial.println("\nIP Address: " + WiFi.localIP().toString());
  Serial.println("Hostname: " + WiFi.hostname());

  if (!MDNS.begin(mdns_hostname)) {
    Serial.println("ERROR: cannot set mDNS responder");
  }
  else {
    Serial.println("mDNS responder started");
    if (!MDNS.addService(mdns_service_name, "tcp", WEBSERVER_PORT)) {
      Serial.println("ERROR: cannot add mDNS service");
    } 
    else {
      Serial.println("mDNS service added");
    }
  }

  sensor.begin();
  sensor.requestTemperatures();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(LED_PIN, LOW);
    String str = "IP Address: <b>" + WiFi.localIP().toString() + "</b>";
    str += "<br>";
    str += "Hostname: <b>" + WiFi.hostname() + "</b>";
    str += "<br>";
    str += "Temperature: <b>" + String(sensor.getTempCByIndex(0)) + " C</b>";
    request->send(200, "text/html", str);
    digitalWrite(LED_PIN, HIGH);
  });

  server.on("/temp", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(LED_PIN, LOW);
    String str = String(sensor.getTempCByIndex(0));
    request->send(200, "text/plain", str);
    digitalWrite(LED_PIN, HIGH);
  });
  
  server.begin();

  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  MDNS.update();
  sensor.requestTemperatures();
  delay(5000);
}
