
#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "NPK Precision Agriculture"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

#define DPIN 4
#define DTYPE DHT11

DHT dht(DPIN, DTYPE);
BlynkTimer timer;

void sendSensorData()
{
  float hu = dht.readHumidity();
  float tc = dht.readTemperature();

  if (isnan(hu) || isnan(tc))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(tc);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(hu);
  Serial.println(" %");

  Blynk.virtualWrite(V3, tc);
  Blynk.virtualWrite(V4, hu);
}

void setup()
{
  Serial.begin(115200);

  dht.begin();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(5000L, sendSensorData);
}

void loop()
{
  Blynk.run();
  timer.run();
}
