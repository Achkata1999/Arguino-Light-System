#include <IRremote.h>

// Define pins for RGB LED
const int redPin = 5;
const int greenPin = 6;
const int bluePin = 9;

// Define pins for sensors
const int ldrPin = A0;
const int switchPin = 2; 

// IR remote codes
#define IR_RECEIVE_PIN 8
const unsigned long redIRCode = 4211404544;
const unsigned long greenIRCode = 4194692864;
const unsigned long blueIRCode = 4177981184;
const unsigned long yellowIRCode = 3944017664;
const unsigned long purpleIRCode = 4044287744;
const unsigned long whiteIRCode = 4161269504;  
const unsigned long pinkIRCode = 3910594304;  
const unsigned long orangeIRCode = 4010864384; 
const unsigned long lowBrightnessIRCode = 4261539584;
const unsigned long upBrightnessIRCode = 4278251264;
const unsigned long powerToggleIRCode = 4228116224;
const unsigned long autoBrightnessToggleIRCode = 3893882624; 
const unsigned long rainbowStartIRCode = 4094422784;

int currentColor = 3; 
bool rainbowMode = false; 
bool deviceOn = true; 
bool autoBrightness = false; 
int brightness = 255; 

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(switchPin, INPUT_PULLUP);

  setColor(255, 255, 0);
}

void loop() {
  if (IrReceiver.decode()) {
    unsigned long irCode = IrReceiver.decodedIRData.decodedRawData;
    IrReceiver.resume(); 

    if (irCode == redIRCode) {
      currentColor = 0; // Red
      rainbowMode = false; // Turn off rainbow mode
    } else if (irCode == greenIRCode) {
      currentColor = 1; // Green
      rainbowMode = false;
    } else if (irCode == blueIRCode) {
      currentColor = 2; // Blue
      rainbowMode = false;
    } else if (irCode == yellowIRCode) {
      currentColor = 3; // Yellow
      rainbowMode = false;
    } else if (irCode == purpleIRCode) {
      currentColor = 4; // Purple
      rainbowMode = false;
    } else if (irCode == whiteIRCode) {
      currentColor = 5; // White
      rainbowMode = false;
    } else if (irCode == pinkIRCode) {
      currentColor = 6; // Pink
      rainbowMode = false;
    } else if (irCode == orangeIRCode) {
      currentColor = 7; // Orange
      rainbowMode = false;
    } else if (irCode == rainbowStartIRCode) {
      rainbowMode = true; // Start rainbow mode
    } else if (irCode == lowBrightnessIRCode) {
      decreaseBrightness();
    } else if (irCode == upBrightnessIRCode) {
      increaseBrightness();
    } else if (irCode == powerToggleIRCode) {
      togglePower();
    } else if (irCode == autoBrightnessToggleIRCode) {
      toggleAutoBrightness(); // Manual toggle for auto brightness
    }
  }

  // Check if the device is turned off
  if (!deviceOn) {
    setColor(0, 0, 0); 
    delay(100);
    return; 
  }

  // Check switch state for automatic brightness mode
  bool isAutomatic = (digitalRead(switchPin) == LOW);

  if (rainbowMode) {
    runRainbowEffect();
  } else {
    if (autoBrightness) {
      int ldrValue = analogRead(ldrPin);
      
      brightness = map(ldrValue, 1023, 0, 0, 255);
      Serial.print("LDR Value: ");
      Serial.print(ldrValue);
      Serial.print(" - Adjusted Brightness: ");
      Serial.println(brightness);
    }

    int redValue = 0;
    int greenValue = 0;
    int blueValue = 0;

    switch (currentColor) {
      case 0: // Red
        redValue = brightness;
        break;
      case 1: // Green
        greenValue = brightness;
        break;
      case 2: // Blue
        blueValue = brightness;
        break;
      case 3: // Yellow
        redValue = brightness;
        greenValue = brightness;
        break;
      case 4: // Purple
        redValue = brightness;
        blueValue = brightness;
        break;
      case 5: // White
        redValue = brightness;
        greenValue = brightness;
        blueValue = brightness;
        break;
      case 6: // Pink
        redValue = brightness;
        blueValue = brightness / 2;
        break;
      case 7: // Orange
        redValue = brightness;
        greenValue = brightness / 2;
        break;
    }

    setColor(redValue, greenValue, blueValue);
  }

  delay(100);
}

void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
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

void decreaseBrightness() {
  brightness = max(0, brightness - 25);
  Serial.println("Brightness decreased");
}

void increaseBrightness() {
  brightness = min(255, brightness + 25);
  Serial.println("Brightness increased");
}

void togglePower() {
  deviceOn = !deviceOn; 
  Serial.println(deviceOn ? "Device turned ON" : "Device turned OFF");
}

void toggleAutoBrightness() {
  autoBrightness = !autoBrightness;
  Serial.println(autoBrightness ? "Automatic brightness ON" : "Automatic brightness OFF");
}
