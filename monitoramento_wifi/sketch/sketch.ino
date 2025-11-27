/* ESP32 -> Adafruit IO (MQTT)
   Publica o valor do RSSI (WiFi.RSSI()) em dBm para um feed no Adafruit IO.
   Bibliotecas requeridas: WiFi, WiFiClientSecure, PubSubClient
*/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

#define WIFI_SSID    "SuaRedeWiFi"
#define WIFI_PASS    "SuaSenhaWiFi"

#define AIO_USERNAME "seu_usuario_adafruit"
#define AIO_KEY      "sua_chave_adafruit"
#define FEED_NAME    "seu_feed"  // nome do feed no Adafruit IO

// intervalo seguro de publicação (ms)
const unsigned long PUBLISH_INTERVAL = 3000UL;

// MQTT host/porta da Adafruit IO
const char* mqtt_server = "io.adafruit.com";
const uint16_t mqtt_port = 8883;

WiFiClientSecure secureClient;
PubSubClient client(secureClient);

unsigned long lastPublish = 0;

// monta tópico: <username>/feeds/<feedname>
String topic;

void setup() {
  Serial.begin(115200);
  delay(100);

  topic = String(AIO_USERNAME) + "/feeds/" + String(FEED_NAME);

  Serial.println("\n=== ESP32 RSSI -> Adafruit IO (MQTT) ===");

  connectWiFi();

  secureClient.setInsecure(); // desativa verificação do certificado (ok para projeto acadêmico)
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi desconectado. Tentando reconectar...");
    connectWiFi();
  }

  if (!client.connected()) {
    reconnectMQTT();
  }

  client.loop();

  unsigned long now = millis();
  if (now - lastPublish >= PUBLISH_INTERVAL) {
    lastPublish = now;
    publishRSSI();
  }
}

void connectWiFi() {
  Serial.print("Conectando na rede: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
    if (millis() - start > 20000) {
      Serial.println("\nNão conectou ao WiFi. Reiniciando...");
      ESP.restart();
    }
  }

  Serial.println("\nWiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void reconnectMQTT() {
  Serial.print("Conectando no MQTT...");
  while (!client.connected()) {
    Serial.print(".");
    if (client.connect(AIO_USERNAME, AIO_USERNAME, AIO_KEY)) {
      Serial.println("\nMQTT conectado!");
    } else {
      Serial.print(" falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente...");
      delay(2000);
    }
  }
}

void publishRSSI() {
  long rssi = WiFi.RSSI();
  char payload[12];
  snprintf(payload, sizeof(payload), "%ld", rssi);

  bool ok = client.publish(topic.c_str(), payload);

  Serial.print("RSSI enviado: ");
  Serial.print(rssi);
  Serial.print(" dBm  -> ");
  Serial.println(ok ? "OK" : "FAIL");
}