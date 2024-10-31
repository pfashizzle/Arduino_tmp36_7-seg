// Arduino pin assignments for the 7-segment displays
const int sevsegA = 0;
const int sevsegB = 1;
const int sevsegC = 2;
const int sevsegD = 3;
const int sevsegE = 4;
const int sevsegF = 5;
const int sevsegG = 6;
const int sevsegCath1 = 7; // Cathode for display 1
const int sevsegCath2 = A3; // Cathode for display 2

// TMP36 sensor pin
const int sensorPin = A2;

// 7-segment digit encoding for 0-9 (common cathode)
const byte digitCodes[] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

int displayTemp = 0; // Variable to hold the current temperature

void setup() {
  // Initialize segment pins as outputs
  for (int i = 0; i < 7; i++) {
    pinMode(i, OUTPUT); // Set segment pins
  }
  pinMode(sevsegCath1, OUTPUT);
  pinMode(sevsegCath2, OUTPUT);
  pinMode(sensorPin, INPUT); // Initialize TMP36 pin

  // Ensure the displays are off at startup
  clearDisplay(sevsegCath1);
  clearDisplay(sevsegCath2);
}

void loop() {
  // Update temperature every 3000 milliseconds (3 seconds)
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 3000) { // Update every 3 seconds
    lastUpdate = millis();

    // Read the TMP36 sensor (analog pin A2)
    int sensorValue = analogRead(sensorPin);
    
    // Convert analog reading (0 - 1023) to voltage (0 - 5V)
    float voltage = sensorValue * (5.0 / 1023.0);
    
    // Convert voltage to temperature in Celsius
    float temperatureC = (voltage - 0.5) * 100.0;

    // Convert temperature to an integer to ignore decimals
    displayTemp = static_cast<int>(temperatureC);
  }

  // Get the tens and ones digits for the temperature
  int tens = displayTemp / 10;
  int ones = displayTemp % 10;

  // Display the numbers on both 7-segment displays
  displayNumbers(tens, ones);
}

// Function to display numbers on both 7-segment displays
void displayNumbers(int number1, int number2) {
  // Display first number
  digitalWrite(sevsegCath1, LOW); // Turn on the first display
  byte code1 = digitCodes[number1];
  setSegments(code1);
  delay(5); // Short delay for multiplexing

  // Clear first display briefly to switch to the second display
  clearDisplay(sevsegCath1); 

  // Display second number
  digitalWrite(sevsegCath2, LOW); // Turn on the second display
  byte code2 = digitCodes[number2];
  setSegments(code2);
  delay(5); // Short delay for multiplexing

  // Clear second display to switch back
  clearDisplay(sevsegCath2);
}

// Function to set the segments based on the digit code
void setSegments(byte code) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(i, (code >> i) & 0x01); // Set segments
  }
}

// Function to clear the display
void clearDisplay(int cathodePin) {
  digitalWrite(cathodePin, HIGH); // Turn off the display
}
