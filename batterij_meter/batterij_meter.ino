#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);    // set the LCD address to 0x27 for a 16

void setup() {
  Serial.begin(9600);
  lcd.init();    // initialize the lcd
}

void loop() {
  //volt();
  //procentBatterij(volt());
  //bericht(procentBatterij());
  lcdScherm();
  delay(1000);
}
 
float volt() {
  int sensorValue = analogRead(A0);    //read the A0 pin value
  float voltage = sensorValue * (5.00 / 1023.00) * 1.94;    //convert the value to a true voltage.
  return voltage;
}

int procentBatterij(float voltage) {
  int procent;
  float maxVoltage = 7.2;
  int procentCalculation = ((voltage / maxVoltage) * 100.0);
  
  if (voltage >= maxVoltage) {
    procent = 99;
  } else {
    procent = procentCalculation;
  }
  return procent;
}

String bericht(int procent) {
  String batterijBericht;
  if (procent >= 50) {
    batterijBericht = "Genoeg stroom";
  } else if (procent >= 20) {
    batterijBericht = "  Bijna leeg ";
  } else {
    batterijBericht = "   Opladen   ";
  }
  return batterijBericht;
}

void lcdScherm() {
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(volt());
  lcd.print("V");
  lcd.print("   ");
  lcd.setCursor(9, 0);
  lcd.print(procentBatterij(volt()));
  lcd.setCursor(11, 0);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print(bericht(procentBatterij(volt())));
}
