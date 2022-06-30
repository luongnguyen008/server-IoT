#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

char * ssid = "HungNguyen";
char * password = "23456789";

WebSocketsClient webSocket;

void setup(void)
{
  
  if (!accel.begin())
  {
    Serial.println("No ADXL345 sensor detected.");
    while (1);
  }
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(2000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
  connectWebSocket();
}
void loop(void)
{
  webSocket.loop();
}
void connectWebSocket() {
  webSocket.begin("13.229.104.201", 5000, "/");          // Địa chỉ websocket server, port và URL
  webSocket.onEvent(webSocketEvent);
  // webSocket.setAuthorization("user", "password");        // Sử dụng thông tin chứng thực nếu cần
  webSocket.setReconnectInterval(5000);                     // Thử lại sau 5s nếu kết nối không thành công
}
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:                // Sự kiện khi client ngắt kết nối
      Serial.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED: {                 // Sự kiện khi client kết nối
      Serial.printf("[WSc] Connected to url: %s\n", payload);
    }
    case WStype_TEXT: {
        // Sự kiện khi nhận được thông điệp dạng TEXT
        Serial.println("[WSc] Text");
        sensors_event_t event;
        accel.getEvent(&event);
        String Json = "{";
        Serial.println(Json);
        Json += "\"x\":";
        Json += event.acceleration.x;
        Json += ",\"y\":";
        Json += event.acceleration.y;
        Json += ",\"z\":";
        Json += event.acceleration.z;
        Json += "}";
        webSocket.sendTXT(Json);
        break;
      }
  }
}
