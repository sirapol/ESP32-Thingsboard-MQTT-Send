#include<WiFi.h>
#include <PubSubClient.h>

#define MQTT_SERVER   "demo.thingsboard.io"
#define MQTT_PORT     1883
#define MQTT_USERNAME "xxxxxxxxxx"      // Access token
#define MQTT_PASSWORD "123456"          // whatever u want
#define MQTT_NAME     "Client_Name"

WiFiClient client;
PubSubClient mqtt(client);

const char* ssid ="YourWifiName";
const char* password =  "Password";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.println("Connecting to WiFi..");
}
  Serial.println("IP ad :");
  Serial.println(WiFi.localIP());

  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
}

void loop() {
  float volt = random (220.0, 240.0);
  float current = random (1.0, 5.0);

  Serial.print("volt = ");
  Serial.print(volt);
  Serial.print("\tCurrent = ");
  Serial.println(current);
  
  if (mqtt.connected() == false) {
    Serial.print("MQTT connection... ");
    if (mqtt.connect(MQTT_NAME, MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("connected");
    } else {
      Serial.println("failed");
      delay(5000);
    }
  } else {
    mqtt.loop();
    String dataJS = "{\"Voltage\":" + String(volt,3) + ",\"current\":" +String(current, 3) + "}";
    char json[100];
    dataJS.toCharArray(json,dataJS.length()+1);
    mqtt.publish("v1/devices/me/telemetry", json);
  }

}
