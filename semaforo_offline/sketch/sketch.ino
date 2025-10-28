#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Classe dos LEDs
class Led {
  int pino;

public:
  Led(int p) {
    pino = p;
    pinMode(pino, OUTPUT);
  }

  void ligar() { digitalWrite(pino, HIGH); }
  void desligar() { digitalWrite(pino, LOW); }

  void piscar(int tempo) {
    ligar();
    delay(tempo);
    desligar();
    delay(tempo);
  }
};

// Pino dos LEDs
Led vermelho(13);
Led amarelo(10);
Led verde(7);

// Ponteiro para o pino do botão
int botao = A2;
int* ptrBotao = &botao;

void setup() {
  Serial.begin(9600);
  pinMode(*ptrBotao, INPUT_PULLUP);

  // Configura o LCD
  lcd.init();
  lcd.backlight();
  lcd.print("Semaforo Offline");
  delay(2000);
  lcd.clear();
}

void loop() {
  if (digitalRead(*ptrBotao) == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Semaforo quebrado!");
    lcd.setCursor(0, 1);
    lcd.print("CUIDADO!");
    vermelho.desligar();
    verde.desligar();

    // pisca o amarelo enquanto o botão estiver pressionado
    while (digitalRead(*ptrBotao) == LOW) {
      amarelo.piscar(300);
    }
    lcd.clear();
    return;
  }

  // modo normal

  // Vermelho 6s
  lcd.clear();
  lcd.print("PARE!");
  vermelho.ligar();
  for (int i = 0; i < 60; i++) {
    if (digitalRead(*ptrBotao) == LOW) return;
    delay(100);
  }
  vermelho.desligar();

  // Verde 4s
  lcd.clear();
  lcd.print("SIGA!");
  verde.ligar();
  for (int i = 0; i < 40; i++) {
    if (digitalRead(*ptrBotao) == LOW) return;
    delay(100);
  }
  verde.desligar();

  // Amarelo 2s
  lcd.clear();
  lcd.print("ATENCAO!");
  amarelo.ligar();
  for (int i = 0; i < 20; i++) {
    if (digitalRead(*ptrBotao) == LOW) return;
    delay(100);
  }
  amarelo.desligar();
}
