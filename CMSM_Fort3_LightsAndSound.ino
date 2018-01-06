// Set up constants for the pins being used
#define SENSEPIN A7
#define SOUNDTRIGPIN 10

// I'd suggest naming these with the animals that they correspond to so it's easier to put them in the right order below
#define LIGHTCTRLPIN1 3
#define LIGHTCTRLPIN2 2

// Set up timing constants
#define TRIGGERSPACINGTIME_SEC 30
#define LIGHTFADETIME_MS 5000L
#define LIGHTONTIME_SEC 60

#define SOUNDSPACINGTIME_SEC 60

// Enter the number of lights/sounds being used
const int numLights = 2;
int lightCtrlPins[numLights] = {LIGHTCTRLPIN2, LIGHTCTRLPIN1}; // add comma separated pin numbers/constants. Must have "numLights" number of entries!
int lightIdx = 0; // start with the first light

void setup() { 
  // Set up all the pins to the proper mode
  pinMode(SOUNDTRIGPIN, OUTPUT);
  pinMode(LIGHTCTRLPIN1, OUTPUT);
  pinMode(LIGHTCTRLPIN2, OUTPUT);
  //Serial.begin(9600);
  digitalWrite(SOUNDTRIGPIN, HIGH);
}

void loop() {
  //Serial.println(readSensorIn()); // for debugging if needed

  // look for a "trigger", specified by the function below
  if (isTriggered()) {

    // ramp the lights from off to on over a period of time, and use the specified pin to control the lights. MUST BE ANALOG WRITE CAPABLE PINS FOR THE DIMMING TO WORK!
    rampLights(lightCtrlPins[lightIdx]);

    // send a signal to the sound box to play a sound
    triggerSound();

    // leave the lights on for LIGHTSONTIME_SEC
    delay(LIGHTONTIME_SEC*1000L);

    // dim the light on the specified pin down to off
    dimLights(lightCtrlPins[lightIdx]);

    // move on to the next light
    lightIdx++;

    // uncomment this next block to enable additional sound/light combos, which run after the delay specified by SOUNDSPACINGTIME_SEC
    /*
    while (lightIdx < numLights) {
      delay(SOUNDSPACINGTIME_SEC*1000L);
      rampLights(lightCtrlPins[lightIdx]);
      triggerSound();
      delay(LIGHTONTIME_SEC*1000L);
      dimLights(lightCtrlPins[lightIdx]);
      lightIdx++;
    }
    lightIdx = 0;
    */

    // if we've gotten to the last light, start over
    if (lightIdx >= numLights) {
      lightIdx = 0;
    }

    // Wait some period of time after the lights turn off before looking for the next trigger
    delay(TRIGGERSPACINGTIME_SEC*1000L);
  }
  // don't overdo the sampling rate, this will be around 9 Hz when the delay between sensor readings is taken into account
  delay(10);
}

int readSensorIn() {
  int numSamples = 20;
  int sumSamples = 0;
  for (int i = 0; i < numSamples; i++) {
    sumSamples += analogRead(SENSEPIN);
    delay(5);
  }
  //Serial.println(sumSamples); // for debugging if needed
  float invDist = map(sumSamples/numSamples, 266, 634, 50, 150); // estimated from https://www.exploringarduino.com/wp-content/uploads/2013/06/GP2Y0A-datasheet.pdf page 5 using 1/(10*mm)
  return 254.0/(invDist); // convert to inches and return
}

void rampLights(int ctrlPin) {
  long msPerStep = LIGHTFADETIME_MS/255L;
  for (int i = 0; i <= 255; i++) {
    analogWrite(ctrlPin, i);
    delay(msPerStep);
  }
}

void dimLights(int ctrlPin) {
  long msPerStep = LIGHTFADETIME_MS/255L;
  for (int i = 255; i >= 0; i--) {
    analogWrite(ctrlPin, i);
    delay(msPerStep);
  }
}

void triggerSound() {
  // the audio box is active low
  digitalWrite(SOUNDTRIGPIN, LOW);
  delay(10);
  digitalWrite(SOUNDTRIGPIN, HIGH);
}

bool isTriggered() {
  int distance = readSensorIn();
  return (distance < 30) && (distance > 0); // look for something closer than 30 inches. The sensor response curve returns < 0 for distances outside the range, so ignore those too
}

