#include <Servo.h>

Servo steering, throttle;
int posSteering = 90, posThrottle = 0;
int pos = 0, relative = 0;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int min = 0, max = 180; // Min and max servo angle

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  steering.attach(9);  // Pin 9
  throttle.attach(10); // Pin 10
}

int validNum(int val) {
  if(val < min)
    return min;
  if(val > max)
    return max;
  return val; 
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    
    // Convert to char Array
    char w[sizeof(inputString)];
    inputString.toCharArray(w, sizeof(w));
    pos = atoi(w);
    Serial.println(pos); 
    
    // Absolute angle    
    if(pos < 1000) {
      posSteering = pos;
      steering.write(validNum(posSteering));
    }
    // Absolute throttle
    else if(pos >= 1000 && pos < 2000) {
      posThrottle = pos - 1000;
      throttle.write(validNum(posThrottle));  
    }
    // Relative angle pivot 2500
    else if(pos >= 2000 && pos < 3000) {
      relative = 2500 - pos;
      relative = posSteering + relative;
      posSteering = validNum(relative);
      steering.write(posSteering);
    }
    // Relative throttle pivot 3500
    else if(pos >= 3000 && pos < 4000) {
      relative = 3500 - pos;
      relative = posThrottle + relative;
      posThrottle = validNum(relative);
      throttle.write(posThrottle); 
    }
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\0') {
      stringComplete = true;
      continue;
    }
    inputString += inChar;
  }
}

