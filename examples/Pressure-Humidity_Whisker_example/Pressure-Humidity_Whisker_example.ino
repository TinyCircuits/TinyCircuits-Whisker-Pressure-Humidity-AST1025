/*************************************************************************
 * BME280 WhiskerBoard Tutorial: 
 * Print the temperature(C), altitude, pressure(hPa), and humidity(%) 
 * readings the BME280 sensor is capable of reading
 * 
 * Hardware by: TinyCircuits
 * BME280 Library by: Adafruit
 * Code by: Laverena Wienclaw for TinyCircuits
 * 
 * NOTE: Adafruit Sensor Library Modified: change device address from 0x77 
 *       to 0x76 to work with TinyCircuits hardware
 *
 * Initiated: Mon. 11/29/2017 
 * Updated: Tue. 07/03/2018
 ************************************************************************/

// This library is used for communication with I2C devices, such as the BME280 board
#include <Wire.h>

// This library is used to print sensor values to a TinyScreen
#include <TinyScreen.h>

// These libraries are used to interface with the BME280 Sensor
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"

// Global Sensor Variables 
#define SEALEVELPRESSURE_HPA (1013.25) // used to find approximate altitude 
Adafruit_BME280 bme; // I2C

// TinyScreen Global Variables
TinyScreen display = TinyScreen(TinyScreenPlus);
int background = TS_8b_Black; // sets the background color to black

// Used to control how often sensor values are updated in the main loop()
unsigned long delayTime = 1000;

// The power pin for our board, used for digitally writing to output
const int powerPin = 4;

void setup() {
  Serial.begin(9600); // Bandwidth for our communication
  // Print to Serial Monitor
  // You can pass flash-memory based strings to Serial.print() by wrapping them with F(). 
  // This means you're using flash memory instead of RAM to print stuff
  Serial.println(F("BME280 test"));

  // We want to see Digital Output from the sensor
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, HIGH);

  Wire.begin();
  selectPort(0); // The adapter board has 4 different ports (0-3),
                 // make sure your software matches the setup!

  // This is the setup used to initialize the TinyScreen's appearance
  display.begin();
  display.setBrightness(15);
  display.setFlip(true);
  display.setFont(thinPixel7_10ptFontInfo); 
  display.fontColor(TS_8b_White, background);

  // Set the cursor to the following coordinates before it prints "BME280 Test" 
  display.setCursor(0, 0);
  display.print("BME280 Test");

  // If the bme sensor is not found, throw statement and stop program
  // If you end up here, check to make sure your value in selectPort() is correct!
  if (!bme.begin()) {
    display.setCursor(12, 12); 
    display.print("No Sensor!");  // Printed to TinyScreen
    Serial.println("Could not find a valid BME280 sensor, check wiring!"); // Printed to Serial Monitor
    while (1); // loop forever, because the rest of the program means nothing without the sensor
  }

  Serial.println("-- Default Test --");
  Serial.println();
}

// Forever looping the following logic
void loop() {
  printValues(); // Print to serial monitor
  printScreen(); // Print to TinyScreen
  delay(delayTime); // How often values are updated and printed
}

// This function prints out the values from the sensor to the Serial Monitor
void printValues() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
}

// This function prints out the values from the sensor to a TinyScreen screen
void printScreen() {
  display.setCursor(0, 12);
  display.print("Temp: ");
  display.print(bme.readTemperature());
  display.println(" C");

  display.setCursor(0, 24);
  display.print("Pres: ");
  display.print(bme.readPressure() / 100.0F);
  display.println(" hPa");

  display.setCursor(0, 36);
  display.print(" Alt: ");
  display.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  display.println(" m");

  display.setCursor(0, 48);
  display.print(" Hum: ");
  display.print(bme.readHumidity());
  display.println(" %");

  display.println();
}

// **This function is necessary for all Whisker boards attached through an Adapter board**
// Selects the correct address of the port being used in the Adapter board
void selectPort(int port) {
  Wire.beginTransmission(0x70); //I2C
  Wire.write(0x04 + port);
  Wire.endTransmission();
}
