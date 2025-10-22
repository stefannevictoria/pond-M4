#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variáveis do Circuito RC
int pinoNoRC=A0; 
int valorLido = 0;
float tensaoCapacitor = 0, tensaoResistor;
unsigned long time;

void setup(){ 
	Serial.begin(9600); 
  
    // Configura o visor
    lcd.init(); 
    lcd.backlight(); 
    
    lcd.print("Iniciando...");
    delay(2000);
    // Limpar após a mensagem de início
    lcd.clear(); 
} 

void loop() { 
    time=millis(); 
    valorLido=analogRead(pinoNoRC); 
    tensaoResistor=(valorLido*5.0/1023.0); 
    
    tensaoCapacitor = (5.0 - tensaoResistor);
    
    //Impressão no LCD 
    
    lcd.setCursor(0, 0);
    lcd.print("VR: "); // Tensão do resistor
    lcd.print(tensaoResistor, 2); 
    lcd.print("V");
    
    lcd.setCursor(0, 1);
    lcd.print("VC: "); // Tensão do Capacitor
    lcd.print(tensaoCapacitor, 2);
    lcd.print("V");
    
    // Impressão no Monitor Serial 
    
    Serial.print(time);
    Serial.print(" ms | V_R: ");
    Serial.print(tensaoResistor, 2);
    Serial.print(" | V_C: ");
    Serial.println(tensaoCapacitor, 2); 
    
    delay(400); 
}