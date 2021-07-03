#define BUZZER 9
#define RED_PIN 3
#define GREEN_PIN 5
#define BLUE_PIN 6

void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  int value = 0;
  checkIR(value);
}

void RGB(int redValue, int greenValue, int blueValue) {
  analogWrite(RED_PIN, redValue);
  analogWrite(GREEN_PIN, greenValue);
  analogWrite(BLUE_PIN, blueValue);
}

void buzzOn() {
  tone(BUZZER, 2000);
  RGB(255, 0, 255);
}

void buzzOff() {
  noTone(BUZZER);
  RGB(0, 255, 255);
}

void checkIR(int input) {
  if (input == 1) {
    buzzOn();
  } else {
    buzzOff();
  }
}
