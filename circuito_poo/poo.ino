// Classe dos LEDs
class Led {
  int pino;

public:
  Led(int p) {
    pino = p;
    pinMode(pino, OUTPUT);
  }

  void ligar() {
    digitalWrite(pino, HIGH);
  }

  void desligar() {
    digitalWrite(pino, LOW);
  }

  void piscar(int tempo) {
    ligar();
    delay(tempo);
    desligar();
    delay(tempo);
  }
};

Led led1(13);
Led led2(10);
Led led3(7);
Led led4(12);
Led led5(5);

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando sequência de LEDs...");
}

void loop() {
  // LEDs piscam um de cada vez
  led1.piscar(150);
  led2.piscar(150);
  led3.piscar(150);
  led4.piscar(150);
  led5.piscar(150);

  // Piscam todos juntos
  led1.ligar();
  led2.ligar();
  led3.ligar();
  led4.ligar();
  led5.ligar();
  delay(300);
  led1.desligar();
  led2.desligar();
  led3.desligar();
  led4.desligar();
  led5.desligar();
  delay(300);
}