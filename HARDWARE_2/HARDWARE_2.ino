#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
#include "index.h" //Our HTML webpage contents with javascripts

#include <usbhid.h>
#include <hiduniversal.h>
#include <Usb.h>
#include <usbhub.h>
#include <hidboot.h>
USB Usb;
USBHub Hub(&Usb);
HIDUniversal Hid(&Usb);

String data = "";

//--------------------------------------------------
#define testLED1 26
#define testLED2 25
#define testLED3 33
#define testLED4 32

#define testLED5 12
#define testLED6 13
#define testLED7 14

#define sw_1 2
#define sw_2 4

String key = "";
//--------------------------------------------------
//SSID and Password of your WiFi router
const char* ssid = "IKKI_FT";
const char* password = "tocal_9310";
//-------------------------------------------------

/************************|
   ตั้งค่า Static IP ตรงนี้   |
 ************************/
WebServer server(80); //Server on port 80

//// it wil set the static IP address to 192, 168, 1, 184
//IPAddress local_IP(192, 168, 76, 1);
//it wil set the gateway static IP address to 192, 168, 1,1
//IPAddress gateway(192, 168, 76, 255);
//// Following three settings are optional
//IPAddress subnet(255, 255, 255, 0);
//IPAddress primaryDNS(8, 8, 8, 8);
//IPAddress secondaryDNS(8, 8, 4, 4);

StaticJsonDocument<250> jsonDocument;
char buffer[250];


String Scanner1 = "";
String Scanner2 = "";
String amp = "";

void SetScanner2() {
  
  Scanner1 = "";
  Scanner2 = "";
  Serial.println("Clear Scanner");
}


void SetScanner1() {
  Scanner1 = "10022739-001-001adsasd";
  Scanner2 = "";
  Serial.println("Set Scanner");
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

//-----------------------------------------------------------

String ledState1 = "ON";
String ledState2 = "ON";
String ledState3 = "ON";
String ledState4 = "ON";

String ledState5 = "NA";
String ledState6 = "NA";
String ledState7 = "NA";

void handleRoot() {
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}

//------------------------------------------------------
void handle_DHTx() {
  // float h = dht.getHumidity();
  // float t = dht.getTemperature();
  String tmpValue = "{  SW : " + (String)amp + " , Key1 : " + Scanner1 + " , Key2 : " + Scanner2 + " }";
  server.send(200, "text/plane", tmpValue); //Send value to client ajax request
}


class KbdRptParser : public KeyboardReportParser {
    void PrintKey(uint8_t mod, uint8_t key);
  protected:
    virtual void OnKeyDown  (uint8_t mod, uint8_t key);
    virtual void OnKeyPressed(uint8_t key);
};

KbdRptParser Prs;


void setup(void) {

  Serial.begin(115200);

  Usb.Init();
  delay(200);
  Hid.SetReportParser(0, (HIDReportParser*)&Prs);


  Serial.print("MOSI: ");
  Serial.println(MOSI);
  Serial.print("MISO: ");
  Serial.println(MISO);
  Serial.print("SCK: ");
  Serial.println(SCK);
  Serial.print("SS: ");
  Serial.println(SS);

  Serial.print("SDA: ");
  Serial.println(SDA);
  Serial.print("SCL: ");
  Serial.println(SCL);

  pinMode(testLED1, OUTPUT); digitalWrite(testLED1, HIGH);
  pinMode(testLED2, OUTPUT); digitalWrite(testLED2, HIGH);
  pinMode(testLED3, OUTPUT); digitalWrite(testLED3, HIGH);
  pinMode(testLED4, OUTPUT); digitalWrite(testLED4, HIGH);

  pinMode(testLED5, OUTPUT); digitalWrite(testLED5, LOW);
  pinMode(testLED6, OUTPUT); digitalWrite(testLED6, LOW);
  pinMode(testLED7, OUTPUT); digitalWrite(testLED7, LOW);

  pinMode(sw_1, INPUT_PULLUP);
  pinMode(sw_2, INPUT_PULLUP);

  Serial.print("\n\nConnect to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("ESP32.SCANNER.1E");
  lcd.setCursor(0, 1);
  lcd.print("Pair");
  delay(500);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
    lcd.print(".");
  }
  lcd.clear();
  Serial.print("\nConnected "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());

  server.on("/setL1", SetScanner1);
  server.on("/setL2", SetScanner2);
  server.on("/scanner", getData);
  server.on("/", handleRoot);
  server.on("/setLED", handleLED);
  server.on("/readDHTx", handle_DHTx);

  server.begin();
  Serial.println("HTTP server started");



  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS 1: ");
  Serial.println(WiFi.dnsIP(0));
  Serial.print("DNS 2: ");
  Serial.println(WiFi.dnsIP(1));
  Serial.println("Ready!!");

  lcd.setCursor(0, 0);
  lcd.print("ssid >"); lcd.print(ssid);
  lcd.setCursor(0, 1);
  lcd.print("ip   >"); lcd.print(WiFi.localIP());
  lcd.setCursor(0, 2);
  lcd.print("SubM >"); lcd.print(WiFi.subnetMask());
  lcd.setCursor(0, 3);
  lcd.print("Gate >"); lcd.print(WiFi.gatewayIP());
}

String key0 = "";
String key1 = "";
String key2 = "";
String key3 = "";

void handleLED() {
  String t_state = server.arg("LEDstate"); //Refer xhttp.open("GET", "setLED?LEDstate="+led, true);
  //Serial.println(t_state);
  if (t_state == "11") {
    digitalWrite(testLED1, HIGH);  //Feedback parameter
    ledState1 = "ON";
  }
  if (t_state == "10") {
    digitalWrite(testLED1, LOW);   //Feedback parameter
    ledState1 = "OFF";
  }
  if (t_state == "21") {
    digitalWrite(testLED2, HIGH);  //Feedback parameter
    ledState2 = "ON";
  }
  if (t_state == "20") {
    digitalWrite(testLED2, LOW);   //Feedback parameter
    ledState2 = "OFF";
  }
  if (t_state == "31") {
    digitalWrite(testLED3, HIGH);  //Feedback parameter
    ledState3 = "ON";
  }
  if (t_state == "30") {
    digitalWrite(testLED3, LOW);   //Feedback parameter
    ledState3 = "OFF";
  }
  if (t_state == "41") {
    digitalWrite(testLED4, HIGH);  //Feedback parameter
    ledState4 = "ON";
  }
  if (t_state == "40") {
    digitalWrite(testLED4, LOW);   //Feedback parameter
    ledState4 = "OFF";
  }

  if (t_state == "51") {
    digitalWrite(testLED5, HIGH);  //Feedback parameter
    ledState5 = "ON";
  }
  if (t_state == "50") {
    digitalWrite(testLED5, LOW);   //Feedback parameter
    ledState5 = "OFF";
  }
  if (t_state == "61") {
    digitalWrite(testLED6, HIGH);  //Feedback parameter
    ledState6 = "ON";
  }
  if (t_state == "60") {
    digitalWrite(testLED6, LOW);   //Feedback parameter
    ledState6 = "OFF";
  }
  if (t_state == "71") {
    digitalWrite(testLED7, HIGH);  //Feedback parameter
    ledState7 = "ON";
  }
  if (t_state == "70") {
    digitalWrite(testLED7, LOW);   //Feedback parameter
    ledState7 = "OFF";
  }

  server.send(200, "text/plane", ledState1 + ", " + ledState2 + ", " + ledState3 + ", " + ledState4 + ", " + ledState5 + ", " + ledState6 + ", " + ledState7); //Send web page
}


void loop(void) {
  server.handleClient();
  amp = digitalRead(sw_1);
  Usb.Task();
  //Serial.println(amp);



}


void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key) {
  uint8_t c = OemToAscii(mod, key);

  if (c)
    OnKeyPressed(c);
}

String s = "";

void KbdRptParser::OnKeyPressed(uint8_t key) {
  if (key == 13) {
    barcodeCallback(s);
    s = "";
  }



  //=================== KEY =================
  char c = (char)key;
  if (c == ')')c = '0';
  if (c == '!')c = '1';
  if (c == '@')c = '2';
  if (c == '#')c = '3';
  if (c == '$')c = '4';
  if (c == '%')c = '5';
  if (c == '^')c = '6';
  if (c == '&')c = '7';
  if (c == '*')c = '8';
  if (c == '(')c = '9';
  if (c == '_')c = '-';
  s = s + c;



}

void barcodeCallback(String data) {
  data.trim();
  
  Serial.println(data);
  Scanner1 = data;
  Scanner2 = data;
 
  key3 = key2;
  key2 = key1;
  key1 = key0;
  key0 = data;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> ");
  lcd.print(key0);
  lcd.setCursor(0, 1);
  lcd.print("> ");
  lcd.print(key1);
  lcd.setCursor(0, 2);
  lcd.print("> ");
  lcd.print(key2);
  lcd.setCursor(0, 3);
  lcd.print("> ");
  lcd.print(key3);
}
