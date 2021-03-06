#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);    // set the LCD address to 0x27 for a 16x2

#define MAX_VOLTAGE 7.2    // Set the maximum voltage of the battery

void setup() {
  Serial.begin(9600);    // Begin the serial monitor
  lcd.init();    // Initialize the lcd
}

void loop() {
  lcdScherm();    // Print the values on the lcd
  delay(600); // Check the battery every 1 minute
}
 
float volt() {
  int inputValue = analogRead(A0);    // Read the A0 pin value
  float voltage = inputValue * (5.00 / 1023.00) * 1.94;    // Convert the value to a true voltage numbers.
  return voltage;
}

int percentageBattery(float voltage) {
  int percentage;
  int percentageCalculation = ((voltage / MAX_VOLTAGE) * 100.0);    // Calculate the percentage
  
  if (voltage >= MAX_VOLTAGE) {
    percentage = 99;
  } else {
    percentage = percentageCalculation;
  }
  return percentage;
}

String bericht(int percentage) {
  String batteryMessage;
  if (percentage >= 50) {
    batteryMessage = "Genoeg stroom";
  } else if (percentage >= 20) {
    batteryMessage = "  Bijna leeg ";
  } else {
    batteryMessage = "   Opladen   ";
  }
  return batteryMessage;
}

void lcdScherm() {
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(volt());
  lcd.print("V");
  lcd.print("   ");
  lcd.setCursor(9, 0);
  lcd.print(percentageBattery(volt()));
  lcd.setCursor(11, 0);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print(bericht(percentageBattery(volt())));
}
