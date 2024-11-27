#include <ESP8266WiFi.h>
 
char ssid[] = "redesiot";
char senha[] = "a1234567";
 
WiFiServer server(80);
 
void conectarWiFi(char SSID[], char SENHA[]) {
  Serial.print("Conectando a rede ");
  Serial.println(SSID);
 
  WiFi.begin(SSID, SENHA);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Conectado");
  Serial.println("Endereço de IP: ");
  Serial.println(WiFi.localIP());
}
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  conectarWiFi(ssid, senha);
  server.begin();
}
 
void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  while (!client.available()) {
    delay(1);
  }
 
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><meta charset='UTF-8'><meta http-equiv='refresh' content='2'></head>");
  client.println("<body>");
  client.println("<h1>Introdução ao IoT com ESP8266</h1>");
  client.println("<h2>Seja Bem Vindo (a) ao SENAI...!</h2>");
  client.println("<h2>kelvin lacerda Viana</h2>");
  client.println("</body>");
  client.println("</html>");
}