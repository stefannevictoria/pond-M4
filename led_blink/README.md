# 💡 Atividade Arduino — Blink LED

Repositório criado para as atividades práticas de **pisca-pisca (Blink)** com Arduino, realizadas durante o módulo de eletrônica e programação embarcada.

---

## 🔹 Parte 1: Blink LED Interno

Nesta parte, foi realizado o clássico **Blink** utilizando o **LED interno do Arduino (pino 13)**.  
O programa faz o LED acender por um tempo X, apagar e aguardar Y segundos, repetindo esse ciclo continuamente.

### 🧠 Objetivo
- Configurar o ambiente de desenvolvimento (Arduino IDE);
- Compreender o funcionamento básico de um loop no Arduino;
- Fazer o LED interno piscar com um intervalo personalizado.

### ⚙️ Código Utilizado
```c
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
```

O arquivo com o código também pode ser encontrado em: [código](led_blink.ino).

### 📸 Evidências

##### 💻 Screenshot do código na Arduino IDE
![imagem codigo](assets/codigo.png)

##### 🔌 Foto do Arduino conectado e LED aceso
![imagem led](assets/arduino_led.jpg)

##### 🎥 Vídeo demonstrando o funcionamento
O vídeo pode ser acessado em: [gif](assets/gif.mp4).
