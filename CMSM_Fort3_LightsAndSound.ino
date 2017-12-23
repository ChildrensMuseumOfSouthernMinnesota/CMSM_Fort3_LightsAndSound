#define SENSEPIN A0
#define SOUNDTRIGPIN 10
#define LIGHTCTRLPIN 9
#define SPACINGTIME_SEC 60
#define LIGHTFADETIME_MS 500

void setup() {
  pinMode(SOUNDTRIGPIN, OUTPUT);
  pinMode(LIGHTCTRLPIN, OUTPUT);
}

void loop() {
  if (readSensorIn() < 30) {
    rampLights();
    triggerSound();
    dimLights();
  }
  delay(SPACINGTIME_SEC*1000L);
}

float readSensorIn() {
  // estimated from https://www.exploringarduino.com/wp-content/uploads/2013/06/GP2Y0A-datasheet.pdf page 5
  int numSamples = 10;
  int sumSamples = 0;
  for (int i = 0; i < 10; i++) {
    sumSamples += analogRead(SENSEPIN);
    delay(5);
  }
  float invDistCM = map(sumSamples/numSamples, 266, 634, 0.05, 0.15);
  return 2.54/(invDistCM);
}

void rampLights() {
  int usPerStep = LIGHTFADETIME_MS*1000L/255;
  for (int i = 0; i <= 255; i++) {
    analogWrite(LIGHTCTRLPIN, i);
    delayMicroseconds(usPerStep);
  }
}

void dimLights() {
  int usPerStep = LIGHTFADETIME_MS*1000L/255;
  for (int i = 255; i >= 0; i--) {
    analogWrite(LIGHTCTRLPIN, i);
    delayMicroseconds(usPerStep);
  }
}

void triggerSound() {
  digitalWrite(SOUNDTRIGPIN, HIGH);
  delay(10);
  digitalWrite(SOUNDTRIGPIN, LOW);
}

