#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include "wifi_connect.h"
#include "ca_cert.h"
#include <DHT.h>

#define SOIL 34
#define DHTPIN 27
#define RELAY 26
#define LDR 35

// WiFi và MQTT thông tin
const char *ssid = "JuliansiPhone";
const char *password = "kd123456";
const char *mqttServer = "47a5763167324520891dd080bd6321d7.s1.eu.hivemq.cloud";
const int mqttPort = 8883;
const char *mqttUser = "DuyPham";
const char *mqttPassword = "Kd12345678";
const char *Soil_topic = "soil_humidity";
const char *Relay_status_topic = "relay_status";
const char *Temp_topic = "temperature";
const char *Bright_topic = "brightness";
const char *AirHum_topic = "air_humidity";

// Khởi tạo MQTT và WiFi
WiFiClientSecure tlsClient;
PubSubClient mqttClient(tlsClient);
Ticker mqttPublishTicker;

// Khởi tạo cảm biến DHT
const int DHTTYPE = DHT11; // Kiểu cảm biến DHT
DHT dht(DHTPIN, DHTTYPE);

// Đọc độ ẩm đất
int readSoilMoisture()
{
    int doc_cambien = 0;
    for (int i = 0; i < 10; i++)
    {
        doc_cambien += analogRead(SOIL);
        delay(10);
    }
    int TBcb = doc_cambien / 10;
    int phantramao = map(TBcb, 0, 1023, 0, 100);
    return 100 - phantramao;
}

// Hàm đăng tải dữ liệu lên MQTT
void publishData()
{
    // Đọc độ ẩm đất
    int Soil_moisture = readSoilMoisture();
    Serial.print("Soil Moisture (%): ");
    Serial.println(Soil_moisture);
    mqttClient.publish(Soil_topic, String(Soil_moisture).c_str());

    // Đọc cường độ sáng
    int LDR_value = analogRead(LDR);
    int brightnessPercentage = map(LDR_value, 0, 4095, 0, 100);
    Serial.print("Brightness (%): ");
    Serial.println(brightnessPercentage);
    mqttClient.publish(Bright_topic, String(brightnessPercentage).c_str());

    // Đọc nhiệt độ và độ ẩm không khí từ DHT
    float Hum = dht.readHumidity();
    float Temp = dht.readTemperature();
    if (isnan(Hum) || isnan(Temp))
    {
        Serial.println("Không có giá trị trả về từ cảm biến DHT");
        return;
    }
    Serial.print("Air Humidity (%): ");
    Serial.println(Hum);
    Serial.print("Temperature (°C): ");
    Serial.println(Temp);
    mqttClient.publish(AirHum_topic, String(Hum).c_str());
    mqttClient.publish(Temp_topic, String(Temp).c_str());

    // Điều khiển relay tự động
    if (Soil_moisture < 60 && brightnessPercentage < 30)
    {
        digitalWrite(RELAY, HIGH); // Bật relay
        mqttClient.publish(Relay_status_topic, "ON");
    }
    else
    {
        digitalWrite(RELAY, LOW); // Tắt relay
        mqttClient.publish(Relay_status_topic, "OFF");
    }
}

// Hàm xử lý tin nhắn MQTT nhận được
void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    payload[length] = '\0';
    String command = String((char *)payload);
    if (strcmp(topic, Relay_status_topic) == 0)
    {
        if (command == "ON")
            digitalWrite(RELAY, HIGH);
        else if (command == "OFF")
            digitalWrite(RELAY, LOW);
    }
}

// Hàm kết nối lại MQTT
void reconnectMQTT()
{
    while (!mqttClient.connected())
    {
        Serial.print("Attempting MQTT connection...");
        String clientId = "esp32-client-" + String(WiFi.macAddress());
        if (mqttClient.connect(clientId.c_str(), mqttUser, mqttPassword))
        {
            Serial.println("connected");
            mqttClient.subscribe(Relay_status_topic);
        }
        else
        {
            Serial.print("Failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 2 seconds");
            delay(2000);
        }
    }
}

void setup()
{
    Serial.begin(115200);
    pinMode(SOIL, INPUT);
    pinMode(LDR, INPUT);
    pinMode(RELAY, OUTPUT);
    digitalWrite(RELAY, LOW);

    // Kết nối WiFi
    setup_wifi(ssid, password);
    Serial.println(WiFi.status() == WL_CONNECTED ? "WiFi Connected" : "WiFi Not Connected");
    // Cài đặt chứng chỉ SSL và MQTT
    tlsClient.setCACert(ca_cert);
    mqttClient.setServer(mqttServer, mqttPort);
    mqttClient.setCallback(mqttCallback);

    // Khởi tạo DHT
    dht.begin();

    // Đặt lịch đăng tải dữ liệu mỗi giây
    mqttPublishTicker.attach(1, publishData);
}

void loop()
{
    if (!mqttClient.connected())
    {
        reconnectMQTT();
    }
    mqttClient.loop();
}
