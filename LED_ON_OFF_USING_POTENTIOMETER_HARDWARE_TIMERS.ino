#include <Wire.h>
#include <LiquidCrystal_I2C.h> 

const int ledPin = 7;
const int potPin = A0;
const float thresholdVoltage = 2.5;
const float referenceVoltage = 5.0;

LiquidCrystal_I2C MyLCD(0x27, 20, 4);

volatile bool updateFlag = false;

ISR(TIMER1_OVF_vect)
{
  TCNT1 = 34285; // Timer Preloading
  updateFlag = true; // Set the flag to indicate an update is needed
}

void setup() {
  Wire.begin();
  MyLCD.init();
  MyLCD.backlight();
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  TCCR1A = 0;           // Init Timer1A
  TCCR1B = 0;           // Init Timer1B
  TCCR1B |= B00000101;  // Prescaler = 1024
  TCNT1 = 34285;        // Timer Preloading
  TIMSK1 |= B00000001;  // Enable Timer Overflow Interrupt
  sei();                // Enable global interrupts
}

void loop() {
  if (updateFlag) {
    updateFlag = false; // Reset the flag

    int potValue = analogRead(potPin);
    float voltage = (potValue * referenceVoltage) / 1023.0;

    MyLCD.setCursor(0, 0);
    MyLCD.print("VOLTAGE : ");
    MyLCD.setCursor(10, 0);
    MyLCD.print(voltage);
    MyLCD.setCursor(14, 0);
    MyLCD.print("V");

    if (voltage >= thresholdVoltage) {
      digitalWrite(ledPin, HIGH);
      MyLCD.setCursor(0, 1);
      MyLCD.print("VOLTAGE ABOVE 2.5V");
    } else {
      digitalWrite(ledPin, LOW);
      MyLCD.setCursor(0, 1);
      MyLCD.print("VOLTAGE BELOW 2.5V");
    }
  }
}
