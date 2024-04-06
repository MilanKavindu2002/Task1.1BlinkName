#include "arduino_secrets.h"
#include "thingProperties.h"
const int ledlight=13;
const String morseMilan = "-- .. .-.. .- -. ";
int morseIndex = 0;
bool blinking = false;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 
pinMode(ledlight,OUTPUT);
  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  // Your code here 
  
  
}



/*
  Since Led is READ_WRITE variable, onLedChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onLedChange()  {
  if (led) { // Cloud button pressed
    if (!blinking) { // If not already blinking, start blinking Milan in Morse code
      blinking = true;
      morseIndex = 0;
      blinkMorse();
    } else { // If already blinking, stop blinking
      blinking = false;
      digitalWrite(ledlight, LOW);
    }
    delay(300); // Debounce delay
  }
  // Add your code here to act upon Led change
}

void blinkMorse() {
  while (blinking) {
    char morseChar = morseMilan[morseIndex];
    if (morseChar == '.') {
      digitalWrite(ledlight, HIGH); // Dot
      delay(200); // Dot duration
      digitalWrite(ledlight, LOW);
      delay(200); // Inter-element gap
    } else if (morseChar == '-') {
      digitalWrite(ledlight, HIGH); // Dash
      delay(600); // Dash duration
      digitalWrite(ledlight, LOW);
      delay(200); // Inter-element gap
    } else if (morseChar == ' ') {
      digitalWrite(ledlight, LOW); // Inter-character gap
      delay(400); // Inter-character gap duration
    }
    
    morseIndex++;
    if (morseIndex >= morseMilan.length()) {
      morseIndex = 0; // Reset index to start blinking from beginning
      if (!blinking) // Exit the loop if blinking is turned off
        break;
    }
  }
}