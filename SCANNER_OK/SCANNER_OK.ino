#include <usbhid.h>
#include <hiduniversal.h>
#include <Usb.h>
#include <usbhub.h>
#include <hidboot.h>
USB Usb;
USBHub Hub(&Usb);
HIDUniversal Hid(&Usb);

String data = "";

class KbdRptParser : public KeyboardReportParser {
    void PrintKey(uint8_t mod, uint8_t key);
  protected:
    virtual void OnKeyDown  (uint8_t mod, uint8_t key);
    virtual void OnKeyPressed(uint8_t key);
};

KbdRptParser Prs;

void setup() {
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

}
void loop() {
  Usb.Task();
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
  if(c==')')c='0';
  if(c=='!')c='1';
  if(c=='@')c='2';
  if(c=='#')c='3';
  if(c=='$')c='4';
  if(c=='%')c='5';
  if(c=='^')c='6';
  if(c=='&')c='7';
  if(c=='*')c='8';
  if(c=='(')c='9';
  if(c=='_')c='-';
  s = s + c;
}

void barcodeCallback(String data) {
  Serial.println(data);
}
