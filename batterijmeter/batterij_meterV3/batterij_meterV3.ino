#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);    // set the LCD address to 0x27 for a 16x2

#define NUM_SAMPLES 10    // Number of analog samples to take per reading
#define MIN_VOLTAGE 3.00   // Set the minimum work voltage of the battery
#define MAX_VOLTAGE 4.20   // Set the maximum work voltage of the battery

int sum = 0;    // Sum of samples taken
unsigned char sampleCount = 0;    // Sample number
float voltage = 0.0;    // Voltage

int percentageCalculation = 0;    // Percentage calculation formula
int percentage = 0;    // Percentage based on the voltage

String batteryMessage = "Geen batterij";    // Default message

#define CHARGE_PIN 3    // Charger pin
#define LED_PIN 2    // Pin for led

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(CHARGE_PIN, INPUT);
  lcd.init();    // Initialize the lcd
}

void loop() {

  // Check if the powerbank is charging
  if (digitalRead(CHARGE_PIN) == HIGH) {
    digitalWrite(LED_PIN, HIGH);
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Aan het opladen ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
  } else {
    digitalWrite(LED_PIN, LOW);
    refreshDisplay();    // Print the values on the lcd
    sampleCount = 0;
    sum = 0;
  }
}

float volt() {
  // Take the numbers of the analog samples and add them up
  while (sampleCount < NUM_SAMPLES) {
    sum += analogRead(A2);
    sampleCount++;
    delay(10);
  }
  voltage = ((float)sum / (float)NUM_SAMPLES * 5.0) / 1024.0;
  float result = voltage * 11.5;  // Input voltage to real voltage of the powerbank
  return result;
}

int percentageBattery(float voltage) {
  percentageCalculation = ((voltage - MIN_VOLTAGE) / (MAX_VOLTAGE - MIN_VOLTAGE)) * 100.00;    // Calculate the percentage

  // Return the correct value for the percentage
  if (voltage >= MAX_VOLTAGE) {
    percentage = 99;  // Safety for lcd screen
  } else if (voltage < MIN_VOLTAGE) {
    percentage = 0;
  } else {
    percentage = percentageCalculation;
  }
  return percentage;
}

String bericht(int percentage) {
  // Check the percentage and give the correct message
  if (percentage >= 50) {
    batteryMessage = "Genoeg spanning";
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
  lcd.print("    ");
  lcd.setCursor(9, 0);
  lcd.print(percentageBattery(volt()));
  lcd.print("%    ");
  lcd.setCursor(0, 1);
  lcd.print(bericht(percentageBattery(volt())));
}

// Refresh the values on the lcd
void refreshDisplay() {
  unsigned long currentTime = millis();
  static unsigned long previousTime = 0;
  const unsigned long interval = 1000; //    Every 1 minute
  if (currentTime - previousTime >= interval) {
    previousTime += interval;
    lcdDisplay();
  }
}
