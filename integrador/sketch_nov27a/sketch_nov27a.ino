#include <Arduino.h>
#include  <ESP8266WiFi.h>
#include "HTTPSRedirect.h"
 
//Enter network credentials
const char* ssid = "modesto1";
const char* password = "a1234567";
 
//Enter Google Script Deployment ID:
const char *GScriptId = "AKfycbwrVnpDQBfIRLpw43hxot2Ua5FvgGsJE740X8F0II7srCS-0ywCf79cFLenuhZLzvCg6A"; //this will be replaced  afterwards
 
//Enter command (insert_row or append_row) and your Google Sheets sheet name (default is Sheet1)
String payload_base = "{\"command\": \"insert_row\", \"sheet_name\": \"Sheet1\", \"values\": ";
String payload = "";
 
//Google sheets setup (do not edit)
const char* host = "script.google.com";
const int httpsPort = 443;
const char* fingerprint = "";
String url = String("/macros/s/") + GScriptId + "/exec";
HTTPSRedirect* client = nullptr;
 
//Declare variables that will be published to Google Sheets
int value0 = 0;
int value1 = 0;
int value2 = 0;
 
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(10);
  Serial.println('\n');
 
  //connect to wiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");
 
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }
  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP Address:\t");
  Serial.println(WiFi.localIP());
 
  //use HTTPSRedirect class to create a new TLS connection
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");
 
  Serial.print("Connecting to ");
  Serial.println(host);
 
  //try to connect for a maximum of 5 times
  bool flag = false;
  for(int i=0; i<5; i++){
    int retval = client->connect(host, httpsPort);
    if (retval == 1){
      flag = true;
      Serial.println("Connected");
      break;
    }
    else{
      Serial.println("Connection failed. Retrying...");
    }
  }
  if (!flag){
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    return;
  }
  delete client; //delete httpsRedirect object
  client = nullptr; //delete httpsRedirect object
}
 
void loop() {
  //create some fake data to publish
  value0 ++;
  value1 = random(0,1000);
  value2 = random(0, 100000);
 
  static bool flag = false;
  if(!flag){
    client = new HTTPSRedirect(httpsPort);
    client->setInsecure();
    flag = true;
    client->setPrintResponseBody(true);
    client->setContentTypeHeader("application/json");
  }
 
  if(client != nullptr){
    if(!client->connected()){
      client->connect(host, httpsPort);
      client->connect(host, httpsPort);
    }
  }
  else{
    Serial.println("Error creating  client object!");
  }
 
  //create json object string to send to Google Sheets
  payload = payload_base + "\"" + value0 + "," + value1 + "," + value2 + "\"}";
 
  //publish data to google sheets
  Serial.println("Publishing data...");
  Serial.println(payload);
  if(client->POST(url, host, payload)){
    //do stuff here if publish was successful
  }
  else{
    //do stuff here if publish was not successful
    Serial.println("Error while connecting");
  }
 
  //a delay for several seconds is required before publishing again
  delay(5000);
}
 