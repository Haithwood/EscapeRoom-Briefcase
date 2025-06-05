int tiltPin = 26;
int tiltValue = 0;
void setup() {
  Serial.begin(9600);
  pinMode(tiltPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  tiltValue = digitalRead(tiltPin);
  Serial.println(tiltValue);
}