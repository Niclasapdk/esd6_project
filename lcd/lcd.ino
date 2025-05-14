/*https://github.com/hasenradball/LCD-I2C*/

#include <LCD-I2C.h>
#include <Wire.h>

LCD_I2C lcd(0x27, 16, 4);  // Default address of most PCF8574 modules, change according
// Protocol byte constants
const byte START_BYTE = 0xAA;
const byte STOP_BYTE = 0xBB;

enum State {
  WAIT_FOR_START,
  READ_PACKET
};

State currentState = WAIT_FOR_START;

byte buffer[5];  // PARAM_IDX, VAL_HI, VAL_LO, STOP
byte index = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lcd.begin(&Wire);
  lcd.display();
  lcd.backlight();
  lcd.print("Ready");
}
String parameters[16] = {
  "DRIVE",
  "LEVEL",
  "HP",
  "LP",
  "THRES",
  "REL",
  "MIX",
  "RATE",
  "DELAY",
  "MIX",
  "RATE",
  "DELAY",
  "DEPTH",
  "RATE",
  "MIX",
  "TIME"
};

void loop() {
  if (Serial.available()) {
    byte incoming = Serial.read();

    switch (currentState) {
      case WAIT_FOR_START:
        if (incoming == START_BYTE) {
          index = 0;
          currentState = READ_PACKET;
        }
        break;

      case READ_PACKET:
        buffer[index++] = incoming;
        if (index >= 4) {
          currentState = WAIT_FOR_START;

          byte stopByte = buffer[3];
          if (stopByte == STOP_BYTE) {
            byte paramIdx = buffer[0];
            int16_t value = ((uint16_t)buffer[1] << 8) | buffer[2];
            updateLCD(paramIdx, value);
          } else {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Invalid STOP byte");
          }
        }
        break;
    }
  }
}

void updateLCD(byte paramIdx, int16_t value) {
  lcd.clear();
  String effect = "";
  if (paramIdx < 6) {
    effect = "FX: Overdrive";
  } else if (paramIdx < 9) {
    effect = "FX: Chorus";
  } else if (paramIdx < 12) {
    effect = "FX: Flanger";
  } else if (paramIdx < 15) {
    effect = "FX: Tremolo";
  } else {
    effect = "FX: Reverb";
  }
  lcd.setCursor(0, 0);
  lcd.print(effect);
  lcd.setCursor(0, 1);
  lcd.print("Param: ");
  lcd.print(parameters[paramIdx]);
  lcd.setCursor(0, 2);
  lcd.print("Value: ");
  lcd.print(value);
}
