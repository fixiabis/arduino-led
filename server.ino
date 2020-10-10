#include <ESP8266WiFi.h>

const char* ssid = "fixiabis";
const char* password = "00000000";

static int LED_PIN_1 = D5;
static int LED_PIN_2 = D4;
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  WiFi.begin(ssid, password);

  Serial.print("Server Connecting to WiFi: '");
  Serial.print(ssid);
  Serial.println("' ");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }

  Serial.println("Server Connected to WiFi");

  server.begin();
  Serial.print("Server Started In: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
 
void loop() {
  WiFiClient client = server.available();
  if (!client) return;
  Serial.println("Client Connected to Server");
  while(!client.available()) delay(1);
  String requestUrl = client.readStringUntil('\r');
  Serial.println(requestUrl);
  client.flush();
  if (requestUrl.indexOf("/light-1/on") != -1) { LED_STATES[0] = HIGH; digitalWrite(LED_PINS[0], LED_STATES[0]); }
  if (requestUrl.indexOf("/light-1/off") != -1) { LED_STATES[0] = LOW; digitalWrite(LED_PINS[0], LED_STATES[0]); }
  if (requestUrl.indexOf("/light-2/on") != -1) { LED_STATES[1] = HIGH; digitalWrite(LED_PINS[1], LED_STATES[1]); }
  if (requestUrl.indexOf("/light-2/off") != -1) { LED_STATES[1] = LOW; digitalWrite(LED_PINS[1], LED_STATES[1]); }
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<script>location = \"https://fixiabis.github.io/\arduino-led?ip=");
  client.print(WiFi.localIP());
  client.print("&led1=");
  client.print(LED_STATES[0] ? "1" : "0");
  client.print("&led2=");
  client.print(LED_STATES[1] ? "1" : "0");
  client.println("\"</script>");
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}
