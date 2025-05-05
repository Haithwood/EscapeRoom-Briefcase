int relay1 = 2;
int relay2 = 3;

void setup() {

  Serial.begin(9600);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
}

void loop() {

  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);

  Serial.println("All relays ON");

  delay(2000);

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);

  Serial.println("All relays OFF");

  delay(2000);
}