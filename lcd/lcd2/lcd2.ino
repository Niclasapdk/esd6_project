#include <LCD-I2C.h>
#include <Wire.h>

LCD_I2C lcd(0x27, 16, 4);

const byte START_BYTE = 0xAA;
const byte STOP_BYTE = 0xBB;

enum State {
  WAIT_FOR_START,
  READ_PACKET
};

State currentState = WAIT_FOR_START;

byte buffer[4];
byte index = 0;

const String parameters[16] = {
  "DRIVE", "LEVEL", "HP", "LP", "THRES", "REL",
  "MIX", "RATE", "DELAY", "MIX", "RATE", "DELAY",
  "DEPTH", "RATE", "MIX", "TIME"
};

const String units[16] = {
  "", "%", " Hz", " Hz", " dB", " ms", "%", " Hz", " ms", "%", " Hz",
  " ms", "%", " Hz", "%", " s"
};

void setup() {
  Serial.begin(115200);
  Wire.begin();
  lcd.begin(&Wire);
  lcd.display();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("  FX UNIT  ");
  lcd.setCursor(0, 1);
  lcd.print("  BOOTING  ");
  lcd.setCursor(0, 2);
  lcd.print("[");
  for (int i = 0; i < 10; i++) {
    lcd.print("=");
    delay(150);
  }
  lcd.print("]");

  delay(300);
}

void loop() {
  while (Serial.available()) {
    byte incoming = Serial.read();

    if (currentState == WAIT_FOR_START) {
      if (incoming == START_BYTE) {
        index = 0;
        currentState = READ_PACKET;
      }
    } else if (currentState == READ_PACKET) {
      buffer[index++] = incoming;

      if (index >= sizeof(buffer)) {
        currentState = WAIT_FOR_START;

        if (buffer[3] == STOP_BYTE) {
          byte paramIdx = buffer[0];
          int16_t value = (buffer[1] << 8) | buffer[2];
          updateLCD(paramIdx, value);
        }
      }
    }
  }
}

float convertQ15ToFloat(int16_t q15, float min, float max) {
  return ceil(min + (q15 / 32767.00f) * (max - min));
}

float getFloatFromParam(byte paramIdx, int16_t q15_value) {
  switch (paramIdx) {
    case 1:
      return ceil((q15_value / 32767.00f) * 100);
    case 4:
      if (q15_value == 0) return -INFINITY;
      return 20 * log10((q15_value / 32767.0f));

    case 0:
    case 2:
    case 3:
      return static_cast<float>(q15_value);

    case 6:
    case 9:
    case 12:
    case 14:
      return convertQ15ToFloat(q15_value, 0.0f, 100.00f);

    case 7:
    case 10:
    case 13:
      return q15_value / 1000.00f;  // TREMOLO RATE

    case 5:
      return (static_cast<uint16_t>(q15_value)/44100.00f) * 1000.00f;
    case 8:
    case 11:
      return (q15_value / 44100.00f) * 1000.00f;  // ms

    case 15:
      return q15_value / 1000.000f;  // ms

    default:
      return static_cast<float>(q15_value);  // fallback
  }
}

void updateLCD(byte paramIdx, int16_t value) {
  lcd.clear();

  String effect;
  if (paramIdx < 6) effect = "FX:OVERDRIVE";
  else if (paramIdx < 9) effect = "FX:CHORUS";
  else if (paramIdx < 12) effect = "FX:FLANGER";
  else if (paramIdx < 14) effect = "FX:TREMOLO";
  else effect = "FX:REVERB";
  float displayValue = getFloatFromParam(paramIdx,value);
  lcd.setCursor(0, 0);
  lcd.print(effect);
  lcd.setCursor(0, 1);
  lcd.print("Param:" + parameters[paramIdx]);
  lcd.setCursor(0, 2);
  if (units[paramIdx] == "%" || units[paramIdx] == "" || paramIdx == 2 || paramIdx == 3 || paramIdx == 5 || paramIdx == 8) {
    lcd.print("Value:" + String((int)displayValue) + units[paramIdx]);
  } else {
    lcd.print("Value:" + String(displayValue, 1) + units[paramIdx]);
  }
}