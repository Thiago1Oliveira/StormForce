

// Include the necessary headers/libraries
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <WiFi.h>

// SSID and password for the Wi-Fi network
const char *ssid = "STUX_NET2.4";
const char *password = "Hydra-gtk1208@";

// MQTT broker address and port
const char *mqttServer = "10.0.42.169";
const int mqttPort = 1883;

// Number of readings to calculate the moving average
const int numReadings = 5;
int readingsIndex = 0;

/*
 * These two lines of code are declaring and initializing arrays of floats
 * with all elements set to -1.0. It is a placeholder value to indicate that
 * the array element has not been set yet. Later, when we read the temperature
 * and humidity from the DHT11 sensor, we will update the array with the new
 * values.
 */
float temperatureReadings[numReadings] = {-1.0};
float humidityReadings[numReadings] = {-1.0};

// Defines the DHT pin and type
#define DHTPIN 26
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

/* This line creates an instance of the WiFiClient class which represents a TCP
   client that can connect to a specified internet IP address and port. */
WiFiClient espClient;
PubSubClient client(espClient);

// Function to setup the Wi-Fi connection
void setupWifi() 
{
    delay(10);
    Serial.println("Connecting to " + String(ssid) + "...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting...");
    }
    Serial.println("Connected to " + String(ssid) + " network!");
}

// Function to reconnect to the MQTT broker
void reconnect() 
{
    while (!client.connected()) {
        Serial.print("Trying to connect to MQTT broker...");
        if (client.connect("ESP32Client")) {
            Serial.println("Connected!");
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" Retrying in 5 seconds...");
            delay(5000);
        }
    }
}

// Function to update the readings array
void updateReadings(float *readings, float newValue)
 {
    readings[readingsIndex] = newValue;
    readingsIndex = (readingsIndex + 1) % numReadings;
 }

/*
 * Function to calculate the moving average of the last 5 readings.
 * It takes an array of floats as an argument and returns a float.
 * The function iterates over the array and calculates the sum of all
 * the values that are not equal to -1.0. Then it divides the sum by
 * the number of values that are not equal to -1.0 and returns the result.
 */
float calculateMovingAverage(float *readings)
 {
    float sum = 0;
    int count = 0;

    for (int i = 0; i < numReadings; i++) {
        if (readings[i] != -1.0) {
            sum += readings[i];
            count++;
        }
    }

    if (count > 0) {
        return sum / count;
    } else {
        return -1.0;
    }
}

// Main function
void setup() 
{
    Serial.begin(115200);  // Initialize serial communication
    dht.begin();           // Initialize DHT sensor
    setupWifi();          // Setup Wi-Fi connection
    client.setServer(mqttServer, mqttPort);  // Setup MQTT broker
}

// Loop function
void loop() 
{
    // Check if the client is connected to the MQTT broker
    if (!client.connected()) {
        reconnect();
    }

    // Read temperature and humidity from the DHT11 sensor
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    // Check if any reading is invalid
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Failed to read from DHT sensor!");
        delay(2000);
        return;
    }

    // Update the readings array
    updateReadings(temperatureReadings, temperature);
    updateReadings(humidityReadings, humidity);

    // Calculate the moving average of the last 5 readings
    float avgTemperature = calculateMovingAverage(temperatureReadings);
    float avgHumidity = calculateMovingAverage(humidityReadings);

    // Convert the float values to strings
    char tempStr[8];
    dtostrf(temperature, 2, 2, tempStr);
    char humStr[8];
    dtostrf(humidity, 2, 2, humStr);
    char avgTempStr[8];
    dtostrf(avgTemperature, 2, 2, avgTempStr);
    char avgHumStr[8];
    dtostrf(avgHumidity, 2, 2, avgHumStr);

    // Name of the topics to publish the values
    char tempTopic[] = "esp32/temperature";
    char humTopic[] = "esp32/humidity";
    char avgTempTopic[] = "esp32/moving_average_temperature";
    char avgHumTopic[] = "esp32/moving_average_humidity";

    // Publish the values to the MQTT broker
    client.publish(tempTopic, tempStr);
    client.publish(humTopic, humStr);
    client.publish(avgTempTopic, avgTempStr);
    client.publish(avgHumTopic, avgHumStr);

    // Print the values and topics to the serial monitor for debugging
    Serial.println();
    Serial.println("+-------------------+-------------------+");
    Serial.println("| Parameter         | Value             |");
    Serial.println("+-------------------+-------------------+");
    Serial.printf("| Temperature       | %-17.2f |\n", temperature);
    Serial.printf("| Humidity          | %-17.2f |\n", humidity);
    Serial.printf("| Moving Avg Temp.  | %-17.2f |\n", avgTemperature);
    Serial.printf("| Moving Avg Humid. | %-17.2f |\n", avgHumidity);
    Serial.println("+-------------------+-------------------+");
    Serial.println("|            Published Topic            |");
    Serial.println("+-------------------+-------------------+");
    Serial.println("| " + String(tempTopic) + "                     |");
    Serial.println("| " + String(humTopic) + "                        |");
    Serial.println("| " + String(avgTempTopic) + "      |");
    Serial.println("| " + String(avgHumTopic) + "         |");
    Serial.println("+-------------------+-------------------+");

    // Wait 3 seconds before reading the sensor again
    delay(3000);
}
