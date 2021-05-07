#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);    // set the LCD address to 0x27 for a 16x2

#define MAX_VOLTAGE 4.18    // Set the maximum voltage of the battery
#define MIN_VOLTAGE 3.10    // Set the minimum voltage of the battery

void setup() {
  Serial.begin(9600);    // Begin the serial monitor
  lcd.init();    // Initialize the lcd
}

void loop() {
  refreshDisplay();    // Print the values on the lcd
}

float volt() {
  int inputValue = analogRead(A0);    // Read the A0 pin value
  Serial.println(inputValue);
  float voltage = inputValue * (5.00 / 1023.00) * 1.82;    // Convert the value to a true voltage numbers.
  Serial.println(voltage);
  return voltage;
}

int percentageBattery(float voltage) {
  int percentage;
  int percentageCalculation = (((voltage - MIN_VOLTAGE) / (MAX_VOLTAGE - MIN_VOLTAGE)) * 100.00);    // Calculate the percentage

  // Return the correct value for the percentage
  if (voltage >= MAX_VOLTAGE) {
    percentage = 99;
  } else {
    percentage = percentageCalculation;
  }
  return percentage;
}

String bericht(int percentage) {
  String batteryMessage;

  // Check the percentage and give the correct message
  if (percentage >= 50) {
    batteryMessage = "Genoeg stroom";
  } else if (percentage >= 20) {
    batteryMessage = "  Bijna leeg ";
  } else {
    batteryMessage = "   Opladen   ";
  }
  return batteryMessage;
}

// Print function for the values
void lcdDisplay() {
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

void refreshDisplay() {
  unsigned long currentTime = millis();
  static unsigned long previousTime = 0;
  const unsigned long interval = 600; //    Every 1 minute
  if (currentTime - previousTime >= interval) {
    previousTime += interval;
    lcdDisplay();
  }
}
