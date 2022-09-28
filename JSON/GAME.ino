#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

const char *SSID = "IKKI_FT";
const char *PWD = "tocal_9310";

WebServer server(80);
 

StaticJsonDocument<250> jsonDocument;
char buffer[250];
 
void setup_routing() {   
  
  server.on("/setL1", SetScanner1); 
  server.on("/setL2", SetScanner2);  
  server.on("/scanner", getData);
  
  server.begin();    
}

const char *Scanner1 = "";
const char *Scanner2 = "";
const char *amp = "";

void SetScanner2(){
  Scanner1="";
  Scanner2 = "";
  Serial.print("Clear Scanner");
}


void SetScanner1(){
  Scanner1="10022739-001-001adsasd";
  Scanner2 = "";
  Serial.print("Set Scanner");
}


void create_json(String tag, String value, String unit) {  
  jsonDocument.clear();  
  jsonDocument["scanner_1"] = Scanner1;
  jsonDocument["scanner_2"] = Scanner2;
  jsonDocument["amp_current"] = amp;
  serializeJson(jsonDocument, buffer);
}

void getData() {
  jsonDocument.clear();
  create_json(Scanner1, Scanner2, amp);
  server.send(200, "application/json", buffer);
}

void setup() {     
  Serial.begin(115200); 

  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  Serial.print("Connected! IP Address: ");
  Serial.println(WiFi.localIP());
  setup_routing();     
   
}    
       
void loop() {    
  server.handleClient();
       
}
