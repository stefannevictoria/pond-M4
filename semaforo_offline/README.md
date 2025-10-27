# 🚦 Semáforo Offline

## 📘 Descrição do Projeto 

O objetivo do projeto é montar e programar um **semáforo funcional** com LEDs representando as luzes vermelha, amarela e verde, controladas por um microcontrolador.  

O sistema respeita os tempos reais de um semáforo:  
- 🔴 **Vermelho:** 6 segundos  
- 🟢 **Verde:** 4 segundos  
- 🟡 **Amarelo:** 2 segundos  

O ciclo se repete continuamente, garantindo segurança e organização no fluxo de veículos e pedestres.  


## 🧠 Objetivos de Aprendizagem  

- Aplicar conceitos de **circuitos eletrônicos** utilizando protoboard, LEDs e resistores.  
- Implementar **lógica sequencial e temporização** em programação.  
- Simular o funcionamento de um **sistema de controle real** de trânsito urbano.  


## ⚙️ Materiais Utilizados  

| Componente | Quantidade | Função | Especificações |
|-------------|-------------|--------|----------------|
| LED vermelho | 1 | Indica "Pare" | 5mm, 2V |
| LED amarelo | 1 | Indica "Atenção" | 5mm, 2V |
| LED verde | 1 | Indica "Siga" | 5mm, 2V |
| Resistores | 3 | Proteção dos LEDs | 220Ω |
| Fios jumper | 4 | Conexões | Macho-macho |
| Protoboard | 1 | Montagem do circuito | — |
| Arduino Uno | 1 | Controle do sistema | — |
| Cabo USB | 1 | Comunicação e energia | — |


## 🔌 Esquema de Montagem  

Um esquema de montagem foi realizado usando o Tinkercad para simular e testar o circuito antes de sua montagem. Abaixo é possível visualizar a simulação:

![Circuito no Tinkercad](assets/circuito.png)



## 🛠️ Etapas da Montagem  

1. **Conecte os LEDs** na protoboard nas cores vermelho, amarelo e verde.  
2. **Ligue resistores de 220Ω** em série com cada LED para evitar sobrecarga.  
3. **Conecte os fios** de cada LED às portas digitais do Arduino (por exemplo, 13, 12 e 11).  
4. **Conecte o GND** do Arduino à barra negativa da protoboard.  
5. Verifique o circuito antes de energizar.  



## 💻 Código-Fonte  

📂 [Clique aqui para acessar o código utilizado.](#)  


## 🎥 Demonstração  

Um vídeo documentando o processo de montagem do proketo foi realizado. [Clique aqui para assistir ao vídeo de funcionamento](#). 

O vídeo demonstra:  
- A montagem física na protoboard.  
- O funcionamento correto dos tempos (6s vermelho, 4s verde, 2s amarelo).  



## 🧑‍🤝‍🧑 Avaliação por Pares  

| Avaliador | Curso | Comentários | Nota |
|------------|--------|-------------|------|
| Nome do Aluno 1 | — | — | — |
| Nome do Aluno 2 | — | — | — |
