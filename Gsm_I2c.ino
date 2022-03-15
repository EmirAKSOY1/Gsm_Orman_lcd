#include <KapadokyaGSM.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
 
LiquidCrystal_I2C lcd(0x27, 16, 2);
KapadokyaGSM Kapadokya;
String smsMetni, smsNumarasi;
#define gonderilecekTelNumarasi  "+905426307649" 
#define LM35_pin A0

int ates_sensoru = 6;
int deger = 0;
int gaz_mesaj=0;
int ates_mesaj=0;

float voltaj_deger = 0;
float sicaklik = 0;
int gelen_veri = 0;

void setup() {
  Kapadokya.basla();
  lcd.begin();
  lcd.backlight();
  pinMode(ates_sensoru, INPUT);

  delay(2000);
  Serial.begin(9600);
  delay(2000);
  Kapadokya.tumSmsSil();
  delay(2000);
  Kapadokya.smsGonder(gonderilecekTelNumarasi, "Sistem acildi.");
 }

void loop() {
  gelen_veri = analogRead(LM35_pin);
  voltaj_deger = (gelen_veri / 1023.0) * 5000;
  sicaklik = voltaj_deger / 10.0;
  lcd.home();
  lcd.print(sicaklik);
  lcd.print("C");
  smsMetni = Kapadokya.smsOku(1);
  if (smsMetni.indexOf("OK") != -1) {
    if (smsMetni.length() > 7) {
      smsNumarasi = Kapadokya.numaraliSmsOku(1);
      smsMetni.toUpperCase();

      if (smsMetni.indexOf("HAVA KAC DERECE?") != -1) {

        String Sicaklik = String(analogRead(LM35_pin) * 0.48828125);
        String Sicaklik2 = "Hava " + Sicaklik + " derece.";
        char SicaklikSMS2[] = "";
        Sicaklik2.toCharArray(SicaklikSMS2, 30);

        Kapadokya.smsGonder(gonderilecekTelNumarasi, SicaklikSMS2);
      }
	  Kapadokya.tumSmsSil();
    }
   }
      lcd.setCursor(9,1);
      lcd.print("GAZ:");
      int sensorValue = analogRead(A1);
      

      if(sensorValue > 700)
      {
        
        gaz_mesaj++;
        
        if(gaz_mesaj == 1)
        {
          lcd.print("VAR");
          Kapadokya.smsGonder(gonderilecekTelNumarasi, "Duman Var");
        }
        }
      else
      {
        lcd.print("YOK");
        gaz_mesaj=0;
     
      }
    lcd.setCursor(0, 1);
    lcd.print("ATES:");
  deger = digitalRead(ates_sensoru);
  if (deger == 1)
  {
    ates_mesaj++;
    if(ates_mesaj==1)
    {
      lcd.print("YOK");
      Kapadokya.smsGonder(gonderilecekTelNumarasi,"Yangin Var");
    }
   
  }
  else
  {
    lcd.print("VAR");//TERS
    ates_mesaj=0; 
  }


  delay(500);
}
