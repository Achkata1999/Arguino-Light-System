#include <IRremote.h>

const int RED_PIN = 5;
const int GREEN_PIN = 6;
const int BLUE_PIN = 9;

const int LDR_PIN = A0;
const int SWITCH_PIN = 2;

#define IR_RECEIVE_PIN 8

const int MAX_BRIGHTNESS = 255;
const int BRIGHTNESS_STEP = 25;

enum Colors {
  RED, GREEN, BLUE, YELLOW, PURPLE, WHITE, PINK, ORANGE
};

enum IRCode {
  RED_CODE = 4211404544,
  GREEN_CODE = 4194692864,
  BLUE_CODE = 4177981184,
  YELLOW_CODE = 3944017664,
  PURPLE_CODE = 4044287744,
  WHITE_CODE = 4161269504,
  PINK_CODE = 3910594304,
  ORANGE_CODE = 4010864384,
  LOW_BRIGHTNESS_CODE = 4261539584,
  UP_BRIGHTNESS_CODE = 4278251264,
  POWER_TOGGLE_CODE = 4228116224,
  AUTO_BRIGHTNESS_TOGGLE_CODE = 3893882624,
  RAINBOW_START_CODE = 4094422784
};

bool deviceOn = true;
bool rainbowMode = false;
bool autoBrightness = false;
int currentColor = YELLOW; 
int brightness = MAX_BRIGHTNESS;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  pinMode(SWITCH_PIN, INPUT_PULLUP);

  setColor(YELLOW);
}

void loop() {
  handleIRInput();

  if (!deviceOn) {
    setColor(0, 0, 0);  
    delay(100);
    return;
  }

  handleAutoBrightness();
  
  if (rainbowMode) {
    runRainbowEffect();
  } else {
    applyColor(currentColor);
  }

  delay(100);
}

void handleIRInput() {
  if (IrReceiver.decode()) {
    unsigned long irCode = IrReceiver.decodedIRData.decodedRawData;
    IrReceiver.resume();

    switch (irCode) {
      case RED_CODE: setColorMode(RED); break;
      case GREEN_CODE: setColorMode(GREEN); break;
      case BLUE_CODE: setColorMode(BLUE); break;
      case YELLOW_CODE: setColorMode(YELLOW); break;
      case PURPLE_CODE: setColorMode(PURPLE); break;
      case WHITE_CODE: setColorMode(WHITE); break;
      case PINK_CODE: setColorMode(PINK); break;
      case ORANGE_CODE: setColorMode(ORANGE); break;
      case RAINBOW_START_CODE: toggleRainbowMode(); break;
      case LOW_BRIGHTNESS_CODE: decreaseBrightness(); break;
      case UP_BRIGHTNESS_CODE: increaseBrightness(); break;
      case POWER_TOGGLE_CODE: togglePower(); break;
      case AUTO_BRIGHTNESS_TOGGLE_CODE: toggleAutoBrightness(); break;
    }
  }
}

void setColorMode(int color) {
  currentColor = color;
  rainbowMode = false;
}

void toggleRainbowMode() {
  rainbowMode = !rainbowMode;
  Serial.println(rainbowMode ? "Rainbow mode ON" : "Rainbow mode OFF");
}

void togglePower() {
  deviceOn = !deviceOn;
  Serial.println(deviceOn ? "Device turned ON" : "Device turned OFF");
}

void toggleAutoBrightness() {
  autoBrightness = !autoBrightness;
  Serial.println(autoBrightness ? "Auto brightness ON" : "Auto brightness OFF");
}

void handleAutoBrightness() {
  if (autoBrightness) {
    int ldrValue = analogRead(LDR_PIN);
    brightness = map(ldrValue, 1023, 0, 0, MAX_BRIGHTNESS);
    Serial.print("LDR Value: ");
    Serial.print(ldrValue);
    Serial.print(" - Adjusted Brightness: ");
    Serial.println(brightness);
  }
}

void decreaseBrightness() {
  brightness = max(0, brightness - BRIGHTNESS_STEP);
  Serial.println("Brightness decreased");
}

void increaseBrightness() {
  brightness = min(MAX_BRIGHTNESS, brightness + BRIGHTNESS_STEP);
  Serial.println("Brightness increased");
}

void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

void applyColor(int color) {
  switch (color) {
    case RED: setColor(brightness, 0, 0); break;
    case GREEN: setColor(0, brightness, 0); break;
    case BLUE: setColor(0, 0, brightness); break;
    case YELLOW: setColor(brightness, brightness, 0); break;
    case PURPLE: setColor(brightness, 0, brightness); break;
    case WHITE: setColor(brightness, brightness, brightness); break;
    case PINK: setColor(brightness, 0, brightness / 2); break;
    case ORANGE: setColor(brightness, brightness / 2, 0); break;
  }
}

void runRainbowEffect() {
  for (int i = 0; i < 256; i++) {
    int red = (i < 85) ? i * 3 : (i < 170) ? (170 - i) * 3 : 0;
    int green = (i < 85) ? 255 - i * 3 : (i < 170) ? 0 : (i - 170) * 3;
    int blue = (i < 85) ? 0 : (i < 170) ? (i - 85) * 3 : 255 - (i - 170) * 3;

    setColor(red, green, blue);
    delay(15);
  }
}
