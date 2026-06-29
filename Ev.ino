
#include <DHT.h>
#include <WiFi.h>
#include <ThingSpeak.h>

// Define constants
const int DHT_PIN = 5;  // DHT sensor pin
const int CURRENT_SENSOR_PIN = 34;  // Current sensor pin
const int VOLTAGE_SENSOR_PIN = 35;  // Voltage sensor pin
const int BUZZER_PIN = 2;  // Buzzer pin
const int DHT_TYPE = DHT11;  // DHT sensor type
const char* username="Swetha";
const char* password="2201914321";
int channel = 2802807;
const char* apikey="QB2FFMMHRAC4SP7N";
WiFiClient client;


// Define variables
float temperature;
float humidity;
float current;
float voltage;

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(username,password);
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("WIFI CONNECTED");
  dht.begin();
  ThingSpeak.begin(client);
  
  

  dht.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(CURRENT_SENSOR_PIN, INPUT);
  pinMode(VOLTAGE_SENSOR_PIN, INPUT);
}

void loop() {
  // Read temperature and humidity
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // Read current
  current = analogRead(CURRENT_SENSOR_PIN);
  current = map(current, 0, 4095, 0, 5);

  // Read voltage
  voltage = analogRead(VOLTAGE_SENSOR_PIN);
  voltage = map(voltage, 0, 4095, 0, 5);

  // Check temperature threshold
  if (temperature > 20) {
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("Buzzer On");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("Buzzer Off");
  }

  // Print sensor readings
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  Serial.print("Current: ");
  Serial.print(current);
  Serial.println("A");
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println("V");
  ThingSpeak.setField(1,temperature);
  ThingSpeak.setField(2,humidity);
  ThingSpeak.setField(3,current);
  ThingSpeak.setField(4,voltage);
  int status = ThingSpeak.writeFields(channel,apikey);
  if(status==200){
    Serial.println("Data uploaded successfully");
  }
  else
  {
    Serial.println("Data failed to storage");
  }
  
  delay(500);
  
}
