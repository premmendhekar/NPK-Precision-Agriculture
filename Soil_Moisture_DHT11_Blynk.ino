```cpp
#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "NPK Precision Agriculture"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// Wi-Fi credentials — replace only in your private local copy
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

// Sensor configuration
#define DHTPIN 4
#define DHTTYPE DHT11
#define SOIL_PIN 34

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// Temporary calibration values.
// Calibrate using actual dry and wet sensor readings later.
const int DRY_VALUE = 3000;
const int WET_VALUE = 1200;

void sendSensorData()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int soilRaw = analogRead(SOIL_PIN);

  // Estimated soil moisture percentage
  int soilPercent = map(soilRaw, DRY_VALUE, WET_VALUE, 0, 100);
  soilPercent = constrain(soilPercent, 0, 100);

  Serial.println("---------------------------");

  if (!isnan(temperature) && !isnan(humidity))
  {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    Blynk.virtualWrite(V3, temperature);
    Blynk.virtualWrite(V4, humidity);
  }
  else
  {
    Serial.println("DHT11 reading failed!");
  }

  Serial.print("Soil Raw Value: ");
  Serial.println(soilRaw);

  Serial.print("Estimated Soil Moisture: ");
  Serial.print(soilPercent);
  Serial.println(" %");

  Blynk.virtualWrite(V5, soilPercent);
}

void setup()
{
  Serial.begin(115200);
  analogReadResolution(12);
  dht.begin();

  Serial.println("NPK Precision Agriculture");
  Serial.println("Connecting to Wi-Fi and Blynk...");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  Serial.println("Blynk connected!");

  timer.setInterval(3000L, sendSensorData);
}

void loop()
{
  Blynk.run();
  timer.run();
}
```
