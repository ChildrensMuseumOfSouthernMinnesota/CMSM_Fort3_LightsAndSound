#define SENSEPIN A4
#define SOUNDTRIGPIN 10
#define LIGHTCTRLPIN 3
#define SPACINGTIME_SEC 30
#define LIGHTFADETIME_MS 5000L
#define LIGHTONTIME_SEC 60

void setup() {
  pinMode(SOUNDTRIGPIN, OUTPUT);
  pinMode(LIGHTCTRLPIN, OUTPUT);
  Serial.begin(9600);
  digitalWrite(SOUNDTRIGPIN, HIGH);
}

void loop() {
  Serial.println(readSensorIn());
  if (readSensorIn() < 30 && readSensorIn() > 0) {
    rampLights();
    triggerSound();
    delay(LIGHTONTIME_SEC*1000L);
    dimLights();
    delay(SPACINGTIME_SEC*1000L);
  }
  delay(10);
}

int readSensorIn() {
  // estimated from https://www.exploringarduino.com/wp-content/uploads/2013/06/GP2Y0A-datasheet.pdf page 5
  int numSamples = 20;
  int sumSamples = 0;
  for (int i = 0; i < numSamples; i++) {
    sumSamples += analogRead(SENSEPIN);
    delay(5);
  }
  float invDist = map(sumSamples/numSamples, 266, 634, 50, 150);
  return 254.0/(invDist);
}

void rampLights() {
  long usPerStep = LIGHTFADETIME_MS/255L;
  for (int i = 0; i <= 255; i++) {
    analogWrite(LIGHTCTRLPIN, i);
    delay(usPerStep);
  }
}

void dimLights() {
  long usPerStep = LIGHTFADETIME_MS/255L;
  for (int i = 255; i >= 0; i--) {
    analogWrite(LIGHTCTRLPIN, i);
    delay(usPerStep);
  }
}

void triggerSound() {
  digitalWrite(SOUNDTRIGPIN, LOW);
  delay(10);
  digitalWrite(SOUNDTRIGPIN, HIGH);
}

