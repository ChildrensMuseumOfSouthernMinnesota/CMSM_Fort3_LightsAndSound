#define SENSEPIN A3
#define SOUNDTRIGPIN 10
#define LIGHTCTRLPIN 3
#define SPACINGTIME_SEC 30
#define LIGHTFADETIME_MS 1000

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
    delay(10000);
    dimLights();
    delay(SPACINGTIME_SEC*1000L);
  }
  delay(10);
}

int readSensorIn() {
  // estimated from https://www.exploringarduino.com/wp-content/uploads/2013/06/GP2Y0A-datasheet.pdf page 5
  int numSamples = 10;
  int sumSamples = 0;
  for (int i = 0; i < 10; i++) {
    sumSamples += analogRead(SENSEPIN);
    delay(5);
  }
  float invDist = map(sumSamples/numSamples, 266, 634, 50, 150);
  return 254.0/(invDist);
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
  digitalWrite(SOUNDTRIGPIN, LOW);
  delay(10);
  digitalWrite(SOUNDTRIGPIN, HIGH);
}

