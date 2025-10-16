// projeto pisca led

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT); // Define pino 13 como saída
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13, HIGH); // Liga o LED
  delay(1000); // Aguarda o tempo determinado
  digitalWrite(13, LOW); // Desliga o LED
  delay(150);
  digitalWrite(13, HIGH);
  delay(150);
  digitalWrite(13, LOW);
  delay(150);
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(500);
}
