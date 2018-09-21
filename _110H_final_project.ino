const int buttonPin = 13;      // Initialize power switch
int newButton = 0;
int oldButton = 0;
const int transistorPin = 7;
int guitarState = 0;

const int sensorPin_1 = 14;     // Initialize 3 laser sensors
boolean sensor_1_state = false;
const int sensorPin_2 = 16;
boolean sensor_2_state = false;
const int sensorPin_3 = 17;
boolean sensor_3_state = false;

const int numReadings = 10;     // Set a range of readings
int sensor_1_readings[numReadings];
int sensor_2_readings[numReadings];
int sensor_3_readings[numReadings];
int readIndex = 0;

void setup() {
  pinMode(buttonPin, INPUT);    // Power switch
  pinMode(transistorPin, OUTPUT);
  pinMode(8,OUTPUT);      // Buzzer
  for (int i = 0; i < numReadings; i++) { // Initizlize the readings
    sensor_1_readings[i] = 0;
    sensor_2_readings[i] = 0;
    sensor_3_readings[i] = 0;
  }
}



void loop() {
  newButton = digitalRead(buttonPin);
  if (newButton && !oldButton)    // Button was off and now on
  {
    if(guitarState == 0) {      // If guitar was off
      guitarState = 1;        // Turn on guitar
      digitalWrite(transistorPin, HIGH);
    } else {          // If guitar was on
      guitarState = 0;        // Turn off guitar
      digitalWrite(transistorPin, LOW);
   }
   oldButton = 1;       // Update button state
  } else if (!newButton && oldButton) { // Button was on and now off
    oldButton = 0;        // Restore button state
  }
  
  if (guitarState == 1) {
    int sensor_1_effective_readings = 0;
    int sensor_2_effective_readings = 0;
    int sensor_3_effective_readings = 0;
    sensor_1_readings[readIndex] = analogRead(sensorPin_1); // Get reading from sensors
    sensor_2_readings[readIndex] = analogRead(sensorPin_2); // Add the reading into array
    sensor_3_readings[readIndex] = analogRead(sensorPin_3);
    
    for (int i = 0; i < numReadings; i++) {
      if (sensor_1_readings[i] > 10 && sensor_1_readings[i] < 800)  // If reading is in active range
        sensor_1_effective_readings++ ; // Increment corresponding effective reading
      if (sensor_2_readings[i] > 10 && sensor_2_readings[i] < 800) 
        sensor_2_effective_readings++ ;
      if (sensor_3_readings[i] > 10 && sensor_3_readings[i] < 800) 
        sensor_3_effective_readings++ ;
    }
    
    if(sensor_1_state == false && sensor_1_effective_readings < 2) {  // If laser is blocked
      sensor_1_state = true;
    } else if (sensor_1_state == true && sensor_1_effective_readings > 8) {
      // If laser was blocked and now hit the sensor
      tone(8, 1047, 250); // Ring the corresponding sound
      delay(10);
      sensor_1_state = false;
    }

    if(sensor_2_state == false && sensor_2_effective_readings < 2) {
      sensor_2_state = true;
    } else if (sensor_2_state == true && sensor_2_effective_readings > 8) {
      tone(8, 1397, 250);
      delay(10);
      sensor_2_state = false;
    }
    
    if(sensor_3_state == false && sensor_3_effective_readings < 2) {
      sensor_3_state = true;
    } else if (sensor_3_state == true && sensor_3_effective_readings > 8) {
      tone(8, 1568, 250);
      delay(10);
      sensor_3_state = false;
    }

    readIndex++;          // Increment read index
    if (readIndex >= numReadings) readIndex = 0;  // If the index reach the end, reset it
  }
}

